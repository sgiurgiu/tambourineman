#include <gtest/gtest.h>
#include <protofileloader.h>

using namespace tbm;

TEST(ProtoFileLoader, loadFile)
{
    ProtoFileLoader loader;
    std::vector<std::string> paths = {"test/data/"};
    auto file = loader.loadFile("test/data/addressbook.proto", paths);
    auto messages = file.messages();
    ASSERT_EQ(messages.size(),2);
    ASSERT_EQ(file.package(),"tutorial");
    auto person = messages[0];
    auto addressbook = messages[1];
    ASSERT_EQ(person.name(),"Person");
    ASSERT_EQ(person.fullName(),"tutorial.Person");
    ASSERT_EQ(person.fields().size(),5);
    ASSERT_EQ(person.fields()[0].name(),"name");
    ASSERT_EQ(person.fields()[0].type(),MessageField::Type::TYPE_STRING);
    ASSERT_EQ(person.fields()[1].name(),"id");
    ASSERT_EQ(person.fields()[1].type(),MessageField::Type::TYPE_INT32);
    ASSERT_EQ(person.fields()[2].name(),"email");
    ASSERT_EQ(person.fields()[2].type(),MessageField::Type::TYPE_STRING);
    ASSERT_EQ(person.fields()[3].name(),"phones");
    ASSERT_EQ(person.fields()[3].type(),MessageField::Type::TYPE_MESSAGE);
    ASSERT_EQ(person.fields()[4].name(),"last_updated");
    ASSERT_EQ(person.fields()[4].type(),MessageField::Type::TYPE_MESSAGE);
    
    ASSERT_EQ(person.fields()[3].typeName(),"message");
    ASSERT_EQ(person.fields()[3].containingMessageType(),"Person");
    
    auto message_id = person.fields()[1].messageType();
    ASSERT_FALSE(message_id);
    
    auto message_phones = person.fields()[3].messageType();
    ASSERT_TRUE(message_phones);
    ASSERT_EQ(message_phones->name(),"PhoneNumber");
    ASSERT_EQ(message_phones->fullName(),"tutorial.Person.PhoneNumber");
    ASSERT_EQ(message_phones->fields().size(),2);
    
    ASSERT_EQ(message_phones->fields()[0].name(),"number");
    ASSERT_EQ(message_phones->fields()[0].type(),MessageField::Type::TYPE_STRING);
    ASSERT_EQ(message_phones->fields()[1].name(),"type");
    ASSERT_EQ(message_phones->fields()[1].type(),MessageField::Type::TYPE_ENUM);
    
    auto phone_type_enum_message = message_phones->fields()[1].enumType();
    ASSERT_TRUE(phone_type_enum_message);
    ASSERT_EQ(phone_type_enum_message->name(),"PhoneType");    
    ASSERT_EQ(phone_type_enum_message->fullName(),"tutorial.Person.PhoneType");    
    auto values = phone_type_enum_message->values();
    ASSERT_EQ(values.size(),3);
    ASSERT_EQ(values[0],"MOBILE");
    ASSERT_EQ(values[1],"HOME");
    ASSERT_EQ(values[2],"WORK");    
}

/*
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/
