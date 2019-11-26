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
    _name = fileDescriptor->name();
    _package = fileDescriptor->package();
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
    return _name;
}
std::string ProtoFile::package() const
{
    return _package;
}
ProtoMessage::ProtoMessage(const google::protobuf::Descriptor* messageDescriptor):
    messageDescriptor(messageDescriptor),_name(messageDescriptor->name()),_fullName(messageDescriptor->full_name())
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
    return _name;
}
std::string ProtoMessage::fullName() const
{
    return _fullName;
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
    fieldDescriptor(fieldDescriptor),_name(fieldDescriptor->name()),_fullName(fieldDescriptor->full_name()),
    _jsonName(fieldDescriptor->json_name()),_number(fieldDescriptor->number()),
    _label((Label)(int)fieldDescriptor->label()),_type((Type)(int)fieldDescriptor->type()),
    _isPackable(fieldDescriptor->is_packable()),_isPacked(fieldDescriptor->is_packed()),
    _isMap(fieldDescriptor->is_map()),_typeName(fieldDescriptor->type_name()),
    _containingMessageName(fieldDescriptor->containing_type()->name())
{    
        auto msgt = fieldDescriptor->message_type();
        if(msgt)
        {
            LOG(INFO) << "Message type "<<msgt->name();
        }
}
std::string MessageField::name() const
{
    return _name;
}
std::string MessageField::fullName() const
{
    return _fullName;
}
std::string MessageField::jsonName() const
{
    return _jsonName;
}
std::string MessageField::containingMessageType() const
{
    return _containingMessageName;
}
int MessageField::number() const
{
    return _number;
}
bool MessageField::isPackable() const
{
    return _isPackable;
}
bool MessageField::isPacked() const
{
    return _isPacked;
}
bool MessageField::isMap() const
{
    return _isMap;
}
MessageField::Type MessageField::type() const
{
    return _type;
}
std::string MessageField::typeName() const
{
    return _typeName;
}
MessageField::Label MessageField::label() const 
{
    return _label;
}

ProtoEnum::ProtoEnum(const google::protobuf::EnumDescriptor* enumDescriptor):
    enumDescriptor(enumDescriptor),_name(enumDescriptor->name()),_fullName(enumDescriptor->full_name()),
    _values(enumDescriptor->value_count())
{
    for(int i=0;i<enumDescriptor->value_count();i++)
    {
        _values[i] = enumDescriptor->value(i)->name();
    }    
}
std::string ProtoEnum::name() const
{
    return _name;
}
std::string ProtoEnum::fullName() const
{
    return _fullName;
}
std::vector<std::string> ProtoEnum::values()
{
    return _values;
}

}//namespace
