#include "httpserver.h"

#include <random>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <boost/utility/string_view.hpp>

#include <easylogging++.h>

//INITIALIZE_EASYLOGGINGPP

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>


namespace tbm {
HttpServer::HttpServer(const GetFunction& func):HttpServer(func,PostFunction())
{}
HttpServer::HttpServer(const PostFunction& func):HttpServer(GetFunction(),func)
{}
HttpServer::HttpServer(const GetFunction& gfunc,const PostFunction& pfunc):
    done(true),started(false),postFunction(pfunc),getFunction(gfunc),
    imp(std::make_unique<HttpServer::InternalImplementation>())
{}
HttpServer::HttpServer():done(true),started(false),imp(std::make_unique<HttpServer::InternalImplementation>())
{}
HttpServer::~HttpServer()
{
    stop();
}
int HttpServer::listeningPort() const
{
    return port;
}

struct HttpServer::InternalImplementation
{
    std::unique_ptr<boost::asio::io_context> ioc;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;
};

template<typename Request, typename Send>
void HttpServer::handle_request(Request&& req,Send&& send)
{
    // Returns a bad request response
    auto const bad_request =
    [&req](boost::beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, LIBTBM_NAME_VERSION_STRING);
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
        res.body() = why.to_string();
        res.prepare_payload();
        return res;
    };
    // Returns a not found response
   /* auto const not_found =
    [&req](boost::beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, LIBTBM_NAME_VERSION_STRING);
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + target.to_string() + "' was not found.";
        res.prepare_payload();
        return res;
    };*/

    // Returns a server error response
    auto const server_error =
    [&req](boost::beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, LIBTBM_NAME_VERSION_STRING);
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + what.to_string() + "'";
        res.prepare_payload();
        return res;
    };

    auto const ok_response =
    [&req](const std::string& body,const std::string& content_type)
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, LIBTBM_NAME_VERSION_STRING);
        res.set(http::field::content_type, content_type);
        res.keep_alive(req.keep_alive());
        res.body() = body;
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    auto method = req.method();
    std::string target (req.target().data(),req.target().size());
    if( method != http::verb::get &&
        method != http::verb::post) {
        return send(bad_request("Unsupported HTTP-method"));
    }
    HeadersMap headers;
    for(const auto& h : req)
    {
        auto name =  h.name_string();
        auto val = h.value();
        headers.insert(std::make_pair(name, val));
    }
    //handle stuff
    try
    {
        if( method == http::verb::get && getFunction)
        {
            auto result = getFunction(target,headers);
            return send(ok_response(result,"text/plain"));
        }
        else if( method == http::verb::post && postFunction)
        {
            auto result = postFunction(target,headers,req.body());
            return send(ok_response(result,"text/plain"));
        }
        else
        {
            return send(server_error("Unable to serve request, no get or post handler defined"));
        }
    }
    catch (const std::exception& ex)
    {
        return send(server_error(ex.what()));
    }

}

template <typename Stream,typename Error>
HttpServer::SendStreamContainer<Stream,Error>::SendStreamContainer(Stream& stream,bool& close,
                                                     Error& ec):
    stream_(stream),close_(close),ec_(ec)
{
}

template <typename Stream,typename Error>
template <typename Message>
void HttpServer::SendStreamContainer<Stream,Error>::operator()(Message&& msg) const
{
    // Determine if we should close the connection after
    close_ = msg.need_eof();

    // We need the serializer here because the serializer requires
    // a non-const file_body, and the message oriented version of
    // http::write only works with const messages.
    using BodyType = typename Message::body_type;
    using FieldsType = typename Message::fields_type;
    http::serializer<Message::is_request::value, BodyType, FieldsType> sr{msg};
    http::write(stream_, sr, ec_);
}

template<>
void HttpServer::handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                               const boost::system::error_code& err)
 {
    if ( err)
    {
        LOG(ERROR) << "Error handling accept request "<<err.value()<<":"<<err.message() <<"\n" ;
        return;
    }
    bool close = false;

    boost::system::error_code ec;
    // This buffer is required to persist across reads
    boost::beast::flat_buffer buffer;
    SendStreamContainer<boost::asio::ip::tcp::socket,boost::system::error_code> lambda{*socket, close, ec};
    for(;;)
    {
       // Read a request
       http::request<http::string_body> req;
       http::read(*socket, buffer, req, ec);
       if(ec == http::error::end_of_stream)
       {
           break;
       }
       if(ec)
       {
           LOG(ERROR) << "Error reading from request "<<ec.value()<<":"<<ec.message() <<"\n" ;
           return;
       }
       // Send the response
       handle_request(std::move(req), lambda);

       if(ec)
       {
           LOG(ERROR) << "Error writing to socket "<<ec.message() <<"\n" ;
           return;
       }
       if(close)
       {
           // This means we should close the connection, usually because
           // the response indicated the "Connection: close" semantic.
           break;
       }
    }

    // Send a TCP shutdown
    socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    socket->close(ec);
    start_accept();
}

void HttpServer::start_accept()
{
    std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(*(imp->ioc));
    imp->acceptor->async_accept(*socket,[socket,this](const boost::system::error_code& ec){
        handle_accept(std::move(socket),ec);
    });
    LOG(INFO) << "after acceptor.accept";
}

void HttpServer::start()
{
    started = false;    
    serverThread = std::thread([this]() {
        done = false;
        imp->ioc = std::make_unique<boost::asio::io_context>(1);
        auto const address = boost::asio::ip::make_address("127.0.0.1");
        imp->acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(*(imp->ioc));        

        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> dis(25600, 59000);//try ports in that range

        for(int i=0;i<100;i++)
        {
            try
            {
                this->port = dis(gen);
                boost::asio::ip::tcp::endpoint endpoint(address, this->port);
                if(!imp->acceptor->is_open())
                {
                    imp->acceptor->open(endpoint.protocol());
                 //   imp->acceptor->set_option(boost::asio::socket_base::reuse_address(true));
                 //   imp->acceptor->non_blocking(true);
                }
                imp->acceptor->bind(endpoint);
                imp->acceptor->listen();
                break;
            }
            catch (const boost::system::system_error& er)
            {
                LOG(WARNING) << "Cannot bind to port: "<< this->port<<"."<<er.what() <<". Retrying.\n";
                this->port = -1;
            }
        }

        if(this->port == -1)
        {
            LOG(ERROR) << "Could not find suitable port to bind to. Giving up starting the server.\n";
            return;
        }
        else
        {
            LOG(INFO) << "Starting server on port "<<this->port<<"\n";
        }


        start_accept();


        {
            started = true;
            std::lock_guard<std::mutex> lock(starting_mutex);
            starting_condition.notify_one();
        }

        imp->ioc->run();
    });

    {
        std::unique_lock<std::mutex> lock(starting_mutex);
        starting_condition.wait(lock,[&](){return (bool)started;});
    }

}
void HttpServer::stop()
{
    if(done) return;
    done = true;
   // LOG(INFO) << "destruction";
    {
        //std::lock_guard<std::mutex> lock(listening_mutex);
        //listening_condition.notify_one();
    }
    imp->acceptor->close();
    imp->ioc->stop();
    if(serverThread.joinable())
    {
        serverThread.join();
    }    
}

}
