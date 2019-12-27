#include "protomessagehttpclient.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <iostream>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace tbm {

ProtoMessageHttpClient::ProtoMessageHttpClient(const std::string &url):url(url)
{
}
ProtoMessageHttpClient::ProtoMessageHttpClient(const std::string& host, int port,
                                               const std::string& path, int version):
    host(host),port(port),defaultPath(path),version(version)
{
}
std::string ProtoMessageHttpClient::performGet(const std::string& path)
{
    auto localPath = path;
    if(path == "") {
        localPath = defaultPath;
    }

    // The io_context is required for all I/O
    boost::asio::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver{ioc};
    tcp::socket socket{ioc};

    // Look up the domain name
    boost::system::error_code ec;
    auto const results = resolver.resolve(host, std::to_string(port),ec);

    if(ec)
    {
        throw ConnectionException("Unable to resolve host "+host+". "+ec.message());
    }

    // Make the connection on the IP address we get from a lookup
    boost::asio::connect(socket, results.begin(), results.end(), ec);
    if(ec)
    {
        throw ConnectionException("Unable to connect to host "+host+". "+ec.message());
    }

    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, localPath, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, LIBTBM_NAME_VERSION_STRING);

    // Send the HTTP request to the remote host
    http::write(socket, req);

    // This buffer is used for reading and must be persisted
    boost::beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(socket, buffer, res);

    // Write the message to standard out
    std::cout << res << std::endl;

    // Gracefully close the socket
    socket.shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes
    // so don't bother reporting it.
    //
    if(ec && ec != boost::system::errc::not_connected)
       throw boost::system::system_error{ec};


    return "";
}

}//namespace tbm
