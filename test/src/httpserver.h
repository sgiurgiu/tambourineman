#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <thread>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <map>
#include <functional>


namespace boost {
namespace asio {
    class io_context;
}
}

namespace tbm {

using HeadersMap = std::multimap<std::string,std::string>;
using PostFunction = std::function<std::string(const std::string& /*req query*/,
                        const HeadersMap&/*headers*/,const std::string& /*body*/)>;
using GetFunction = std::function<std::string(const std::string& /*req query*/,
                        const HeadersMap&/*headers*/)>;

class HttpServer
{
public:
    HttpServer();
    HttpServer(const GetFunction& func);
    HttpServer(const PostFunction& func);
    HttpServer(const GetFunction& gfunc,const PostFunction& pfunc);
    ~HttpServer();
    void start();
    void stop();
    int listeningPort() const;
private:
    template<typename Socket,typename Error>
    void handle_accept(Socket* socket, const Error& err);
    template<typename Request, typename Send>
    void handle_request(Request&& req,Send&& send);

    template <typename Stream,typename Error>
    struct SendStreamContainer {
        Stream& stream_;
        bool& close_;
        Error& ec_;

        explicit SendStreamContainer(Stream& stream,bool& close,
                                     Error& ec);
        template<typename Message>
        void operator()(Message&& msg) const;
    };

private:
    std::atomic_bool done;
    int port;
    std::thread serverThread;
    std::unique_ptr<boost::asio::io_context> ioc;
    std::condition_variable listening_condition;
    std::mutex listening_mutex;
    PostFunction postFunction;
    GetFunction getFunction;
};

} //namespace

#endif // HTTPSERVER_H
