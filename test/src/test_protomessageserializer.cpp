#include <gtest/gtest.h>
#include <protofileloader.h>
#include <protomessageserializer.h>
#include "addressbook.pb.h"
#include "prototestutils.h"
#include <json.hpp>

#include <iostream>

using namespace tbm;

TEST(ProtoMessageSerializer, validateIncorrectMessage)
{
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
#ifdef TBM_TEST_LINUX
    paths.emplace_back("/usr/include/");
#endif

    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];

    try
    {
        //here name is an object instead of a string
        std::string jsonMessage = R"({"name":{},"email":"person@example.com",
        "id":11,"last_updated":{"seconds":10,"nanos":0},
        "phones":[{"number":"1234567890","type":"HOME"},{"number":"0987654321","type":"MOBILE"}]})"
        ;

        ProtoMessageSerializer messageSerializer(&person);
        messageSerializer.serializeMessage(jsonMessage);
        //should throw, not reach here
       GTEST_FAIL();
    }
    catch(const InvalidMessageException& ex)
    {
    }
}
TEST(ProtoMessageSerializer, validateCorrectMessage)
{
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
#ifdef TBM_TEST_LINUX
    paths.emplace_back("/usr/include/");
#endif

    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];

    std::string jsonMessage = R"({"name":"Example Person","email":"person@example.com",
    "id":11,"last_updated":{"seconds":10,"nanos":0},
    "phones":[{"number":"1234567890","type":"HOME"},
    {"number":"0987654321","type":"MOBILE"}], "new_int64":12, "new_double":0.3,
    "new_float":1.3,"new_uint32":11,"new_uint64":12,"new_sint32":-12,"new_sint64":-20,
    "new_fixed32":12,"new_fixed64":13,"new_sfixed32":-12,"new_sfixed64":-2,"new_bool":true})"
    ;

    ProtoMessageSerializer messageSerializer(&person);
    messageSerializer.serializeMessage(jsonMessage);
}

TEST(ProtoMessageSerializer, serializePersonMessage)
{  
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
#ifdef TBM_TEST_LINUX
    paths.emplace_back("/usr/include/");
#endif

    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    
    std::string serialized_message = ProtoTestUtils::createSerializedPersonMessage();

    std::string jsonMessage = ProtoTestUtils::personMessageJsonRepresentation();

    ProtoMessageSerializer messageSerializer(&person);    
    auto ourSerializedMessage = messageSerializer.serializeMessage(jsonMessage);
        
    ASSERT_EQ(ourSerializedMessage,serialized_message);    
}

TEST(ProtoMessageSerializer, readPersonMessage)
{
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
#ifdef TBM_TEST_LINUX
    paths.emplace_back("/usr/include/");
#endif

    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    std::string serialized_message = ProtoTestUtils::createSerializedPersonMessage();

    tutorial::Person pp;
    pp.ParseFromString(serialized_message);

    ProtoMessageSerializer messageSerializer(&person);
    auto jsonMessage = messageSerializer.readMessage(serialized_message);

    std::string referenceJsonMessage = ProtoTestUtils::personMessageJsonRepresentation();

    nlohmann::json referenceJson = nlohmann::json::parse(referenceJsonMessage);
    nlohmann::json deserliazedJson = nlohmann::json::parse(jsonMessage);

    ASSERT_EQ(referenceJson,deserliazedJson);
}
