#include <gtest/gtest.h>
#include <protomessagehttpclient.h>

#include "httpserver.h"

#include <easylogging++.h>

using namespace tbm;


TEST(ProtoMessageHttpClient, getRequest)
{
    HttpServer server([](const std::string& query,
                      const HeadersMap& headers) -> std::string {
        LOG(INFO) << "GET called with query string "<<query;
        for(const auto& p : headers)
        {
            LOG(INFO) << "header "<<p.first<<"="<<p.second;
        }
        return "test string";
    });

    server.start();

    auto port = server.listeningPort();

    ProtoMessageHttpClient client("127.0.0.1",port,"/");
    auto result = client.performGet();

    ASSERT_EQ(result,"test string");
}

TEST(ProtoMessageHttpClient, postRequest)
{
    const std::string requestBody = "test body";
    PostFunction postFunc = [requestBody](const std::string& query,
            const HeadersMap& headers,const std::string& body) -> std::string {
            LOG(INFO) << "POST called with query string "<<query;
            for(const auto& p : headers)
            {
              LOG(INFO) << "header "<<p.first<<"="<<p.second;
            }
            LOG(INFO) << "POST called with body '"<<body<<"' of length:"<<body.size();

            EXPECT_EQ(body,requestBody);
            if(::testing::AssertionFailure())
            {
                ADD_FAILURE();
            }
            return "test string";
    };

    HttpServer server(postFunc);

    server.start();

    auto port = server.listeningPort();

    ProtoMessageHttpClient client("127.0.0.1",port,"/");
    auto result = client.performPost(requestBody);

    ASSERT_EQ(result,"test string");
}
