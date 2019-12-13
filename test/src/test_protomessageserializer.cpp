#include <gtest/gtest.h>
#include <protofileloader.h>
#include <protomessageserializer.h>
#include <google/protobuf/util/time_util.h>
#include "addressbook.pb.h"

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
    {"number":"0987654321","type":"MOBILE"}]})"
    ;

    ProtoMessageSerializer messageSerializer(&person);
    messageSerializer.serializeMessage(jsonMessage);

}
TEST(ProtoMessageSerializer, serializeMessage)
{  
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    
   // std::string serialized_message;

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
    auto    serialized_message = person_message.SerializeAsString();
        


    std::string jsonMessage = R"({"name":"Example Person","email":"person@example.com",
    "id":11,"last_updated":{"seconds":10,"nanos":0},
    "phones":[{"number":"1234567890","type":"HOME"},{"number":"0987654321","type":"MOBILE"}]})"
    ;
    
    ProtoMessageSerializer messageSerializer(&person);    
    auto ourSerializedMessage = messageSerializer.serializeMessage(jsonMessage);

    std::cout << serialized_message <<std::endl;
    std::cout << "protobuf serliazed size:"<<serialized_message.size() <<"\n";
    std::cout << "our serliazed size:"<<ourSerializedMessage.size() <<"\n";
        
    ASSERT_EQ(ourSerializedMessage,serialized_message);    
}
