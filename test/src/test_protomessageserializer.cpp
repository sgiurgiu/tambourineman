#include <gtest/gtest.h>
#include <protofileloader.h>
#include <protomessageserializer.h>
#include <google/protobuf/util/time_util.h>
#include "addressbook.pb.h"
#include <json.hpp>

#include <iostream>

using namespace tbm;

TEST(ProtoMessageSerializer, validateIncorrectMessage)
{
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
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

std::string personToString()
{
    tutorial::Person person_message;
    person_message.set_email("person@example.com");
    person_message.set_name("Example Person");
    person_message.set_id(11);
    *person_message.mutable_last_updated() = google::protobuf::util::TimeUtil::SecondsToTimestamp(10);
    auto phone_home = person_message.add_phones();
    phone_home->set_number("1234567890");
    phone_home->set_type(tutorial::Person_PhoneType::Person_PhoneType_HOME);
    auto phone_mobile = person_message.add_phones();
    phone_mobile->set_number("0987654321");
    phone_mobile->set_type(tutorial::Person_PhoneType::Person_PhoneType_MOBILE);
    person_message.set_new_bool(true);
    person_message.set_new_double(0.3);
    person_message.set_new_fixed32(12);
    person_message.set_new_fixed64(13);
    person_message.set_new_float(1.3f);
    person_message.set_new_int64(12);
    person_message.set_new_sfixed32(-12);
    person_message.set_new_sfixed64(-2);
    person_message.set_new_sint32(-12);
    person_message.set_new_sint64(-20);
    person_message.set_new_uint32(11);
    person_message.set_new_uint64(12);

    return person_message.SerializeAsString();
}

TEST(ProtoMessageSerializer, serializePersonMessage)
{  
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    
    std::string serialized_message = personToString();

    std::string jsonMessage = R"({"name":"Example Person","email":"person@example.com",
    "id":11,"last_updated":{"seconds":10,"nanos":0},
    "phones":[{"number":"1234567890","type":"HOME"},
    {"number":"0987654321","type":"MOBILE"}], "new_int64":12, "new_double":0.3,
    "new_float":1.3,"new_uint32":11,"new_uint64":12,"new_sint32":-12,"new_sint64":-20,
    "new_fixed32":12,"new_fixed64":13,"new_sfixed32":-12,"new_sfixed64":-2,"new_bool":true})"
    ;

    ProtoMessageSerializer messageSerializer(&person);    
    auto ourSerializedMessage = messageSerializer.serializeMessage(jsonMessage);

    //std::cout << serialized_message <<std::endl;
    //std::cout << "protobuf serliazed size:"<<serialized_message.size() <<"\n";
    //std::cout << "our serliazed size:"<<ourSerializedMessage.size() <<"\n";
        
    ASSERT_EQ(ourSerializedMessage,serialized_message);    
}

TEST(ProtoMessageSerializer, readPersonMessage)
{
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    std::string serialized_message = personToString();

    tutorial::Person pp;
    pp.ParseFromString(serialized_message);

    ProtoMessageSerializer messageSerializer(&person);
    auto jsonMessage = messageSerializer.readMessage(serialized_message);

    std::string referenceJsonMessage = R"({"name":"Example Person","email":"person@example.com",
    "id":11,"last_updated":{"seconds":10,"nanos":0},
    "phones":[{"number":"1234567890","type":"HOME"},
    {"number":"0987654321","type":"MOBILE"}], "new_int64":12, "new_double":0.3,
    "new_float":1.3,"new_uint32":11,"new_uint64":12,"new_sint32":-12,"new_sint64":-20,
    "new_fixed32":12,"new_fixed64":13,"new_sfixed32":-12,"new_sfixed64":-2,"new_bool":true})"
    ;

    nlohmann::json referenceJson = nlohmann::json::parse(referenceJsonMessage);
    nlohmann::json deserliazedJson = nlohmann::json::parse(jsonMessage);

    ASSERT_EQ(referenceJson,deserliazedJson);
}
