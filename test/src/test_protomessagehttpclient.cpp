#include <gtest/gtest.h>
#include <protofileloader.h>
#include <protomessagehttpclient.h>

#include "httpserver.h"

using namespace tbm;


TEST(ProtoMessageHttpClient, getRequest)
{
   /* ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    auto addressbook = messages[1];*/

    HttpServer server([](const std::string& query,
                      const HeadersMap& headers) -> std::string {
        return "";
    });

    server.start();

   // std::this_thread::sleep_for(std::chrono::seconds(30));
    auto port = server.listeningPort();

    ProtoMessageHttpClient client("127.0.0.1",port,"/");
    auto result = client.performGet();
    //std::this_thread::sleep_for(std::chrono::seconds(30));

    ASSERT_EQ(result,"tutorial");


}

