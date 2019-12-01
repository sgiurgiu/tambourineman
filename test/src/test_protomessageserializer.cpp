#include <gtest/gtest.h>
#include <protofileloader.h>
#include <protomessageserializer.h>
#include <google/protobuf/util/time_util.h>
#include "addressbook.pb.h"

#include <iostream>

using namespace tbm;

TEST(ProtoMessageSerializer, serializeMessage)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    
    std::string serialized_message;
    {
        tutorial::AddressBook addressbook;    
        tutorial::Person* person_message = addressbook.add_people();
        person_message->set_email("person@example.com");
        person_message->set_name("Example Person");
        person_message->set_id(11);
        *person_message->mutable_last_updated() = google::protobuf::util::TimeUtil::SecondsToTimestamp(10);
        auto phone_home = person_message->add_phones();
        phone_home->set_number("1234567890");
        phone_home->set_type(tutorial::Person_PhoneType::Person_PhoneType_HOME);    
        auto phone_mobile = person_message->add_phones();
        phone_mobile->set_number("0987654321");
        phone_mobile->set_type(tutorial::Person_PhoneType::Person_PhoneType_MOBILE);
        serialized_message = addressbook.SerializeAsString();
    }
    std::string jsonMessage = "{'people':[{'name':'Example Person','email':'person@example.com',"
    "'id':11,'last_updated':{'seconds':10,'nanos':0},'phones':[{'number':'1234567890','type':'HOME'},{'number':'0987654321','type':'MOBILE'}]"
    "}]}";
    
    ProtoMessageSerializer messageSerializer(&person);    
    auto ourSerializedMessage = messageSerializer.serializeMessage(jsonMessage);
    
    
    ASSERT_EQ(ourSerializedMessage,serialized_message);
    
}
