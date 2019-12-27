#include <gtest/gtest.h>
#include <protofileloader.h>
#include <protomessagehttpclient.h>


using namespace tbm;


TEST(ProtoMessageHttpClient, getRequest)
{
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    auto addressbook = messages[1];



}

