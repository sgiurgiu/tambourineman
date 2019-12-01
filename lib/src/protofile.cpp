#include "protofile.h"
#include "easylogging++.h"

#include <google/protobuf/descriptor.h>

namespace tbm {
ProtoFile::ProtoFile()
{
}
ProtoFile::ProtoFile(const google::protobuf::FileDescriptor* fileDescriptor):
    fileDescriptor(fileDescriptor)
{
    for(int i=0;i<fileDescriptor->message_type_count();i++)
    {
        protoMessages.push_back(ProtoMessage(fileDescriptor->message_type(i)));
    }    
}
const std::vector<ProtoMessage>& ProtoFile::messages() const
{
    return protoMessages;
}
std::string ProtoFile::name() const
{
    return fileDescriptor->name();
}
std::string ProtoFile::package() const
{
    return fileDescriptor->package();
}
ProtoMessage::ProtoMessage(const google::protobuf::Descriptor* messageDescriptor):
    messageDescriptor(messageDescriptor)
{
    for(int i=0;i<messageDescriptor->field_count();i++)
    {
        messageFields.push_back(MessageField(messageDescriptor->field(i)));
    }
    for(int i=0;i<messageDescriptor->enum_type_count();i++)
    {
        messageEnums.push_back(ProtoEnum(messageDescriptor->enum_type(i)));
    }
    for(int i=0;i<messageDescriptor->nested_type_count();i++)
    {
        internalMessages.push_back(ProtoMessage(messageDescriptor->nested_type(i)));
    }
}
std::string ProtoMessage::name() const
{
    return messageDescriptor->name();
}
std::string ProtoMessage::fullName() const
{
    return messageDescriptor->full_name();
}
const std::vector<MessageField>& ProtoMessage::fields() const
{
    return messageFields;
}
const std::vector<ProtoEnum>& ProtoMessage::enums() const
{
    return messageEnums;
}
MessageField::MessageField(const google::protobuf::FieldDescriptor* fieldDescriptor):
    fieldDescriptor(fieldDescriptor)
{    
        auto msgt = fieldDescriptor->message_type();
        if(msgt)
        {
            LOG(INFO) << "Message type "<<msgt->name();
        }
}
std::unique_ptr<ProtoMessage> MessageField::messageType() const
{
    auto descriptor = fieldDescriptor->message_type();
    if(descriptor)
    {
        return std::unique_ptr<ProtoMessage>(new ProtoMessage(descriptor));
    }
    else 
    {
        return std::unique_ptr<ProtoMessage>();
    }
}
std::unique_ptr<ProtoEnum> MessageField::enumType() const
{
    auto enumType = fieldDescriptor->enum_type();
    if(enumType)
    {
        return std::unique_ptr<ProtoEnum>(new ProtoEnum(enumType));
    }
    else
    {
        return std::unique_ptr<ProtoEnum>();
    }     
}

std::string MessageField::name() const
{
    return fieldDescriptor->name();
}
std::string MessageField::fullName() const
{
    return fieldDescriptor->full_name();
}
std::string MessageField::jsonName() const
{
    return fieldDescriptor->json_name();
}
std::string MessageField::containingMessageType() const
{
    return fieldDescriptor->containing_type()->name();
}
int MessageField::number() const
{
    return fieldDescriptor->number();
}
bool MessageField::isPackable() const
{
    return fieldDescriptor->is_packable();
}
bool MessageField::isPacked() const
{
    return fieldDescriptor->is_packed();
}
bool MessageField::isMap() const
{
    return fieldDescriptor->is_map();
}
MessageField::Type MessageField::type() const
{
    return static_cast<MessageField::Type>(fieldDescriptor->type());
}
std::string MessageField::typeName() const
{
    return fieldDescriptor->type_name();
}
MessageField::Label MessageField::label() const 
{
    return static_cast<MessageField::Label>(fieldDescriptor->label());
}

ProtoEnum::ProtoEnum(const google::protobuf::EnumDescriptor* enumDescriptor):
    enumDescriptor(enumDescriptor),_values(enumDescriptor->value_count())
{
    for(int i=0;i<enumDescriptor->value_count();i++)
    {
        _values[i] = enumDescriptor->value(i)->name();
    }    
}
std::string ProtoEnum::name() const
{
    return enumDescriptor->name();
}
std::string ProtoEnum::fullName() const
{
    return enumDescriptor->full_name();
}
std::vector<std::string> ProtoEnum::values()
{
    return _values;
}

}//namespace
