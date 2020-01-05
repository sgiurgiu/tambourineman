#ifndef PROTOMESSAGEHTTPCLIENT_H
#define PROTOMESSAGEHTTPCLIENT_H

#include <string>
#include <stdexcept>

namespace tbm {

class ConnectionException : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class ProtoMessageHttpClient
{
public:
    ProtoMessageHttpClient(const std::string& url);
    ProtoMessageHttpClient(const std::string& host, int port,
                           const std::string& path, int version = 11);
    std::string performGet(const std::string& path = "");
    std::string performPost(const std::string& body,const std::string& path = "");
private:
    template<typename Method>
    std::string perform(const std::string& body,Method m,const std::string& path = "");
private:
    std::string url;
    std::string host;
    int port;
    std::string defaultPath;
    int version = 11;
};
}//namespace tbm

#endif // PROTOMESSAGEHTTPCLIENT_H
