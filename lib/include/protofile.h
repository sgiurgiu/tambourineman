#ifndef PROTOFILE_H
#define PROTOFILE_H

#include <string>
#include <vector>

namespace google {
namespace protobuf {

class FileDescriptor;
class Descriptor;
class FieldDescriptor;
class OneofDescriptor;
class EnumDescriptor;
}}

namespace tbm {

class ProtoEnum
{
public:
    std::string name() const;
    std::string fullName() const;
    std::vector<std::string> values();
private:
    ProtoEnum(const google::protobuf::EnumDescriptor* enumDescriptor);
private:
    const google::protobuf::EnumDescriptor* enumDescriptor;
    friend class ProtoMessage;
};

class MessageField
{
public:
    std::string name() const;
    std::string fullName() const;
    std::string jsonName() const;
    int number() const;
    bool isRequired() const;
    bool isOptional() const;
    bool isRepeated() const;
    bool isPackable() const;
    bool isPacked() const;
    bool isMap() const;

    int type() const;
    std::string typeName() const;
private:
    MessageField(const google::protobuf::FieldDescriptor* fieldDescriptor);
private:

    const google::protobuf::FieldDescriptor* fieldDescriptor;
    friend class ProtoMessage;
};

class ProtoMessage
{
public:
    std::string name() const;
    std::string fullName() const;
    const std::vector<MessageField>& fields() const;
    const std::vector<ProtoEnum>& enums() const;
private:
    ProtoMessage(const google::protobuf::Descriptor* messageDescriptor);
private:
    const google::protobuf::Descriptor* messageDescriptor;
    std::vector<MessageField> messageFields;
    std::vector<ProtoEnum> messageEnums;
    friend class ProtoFile;
};

class ProtoFile
{
public:
    ProtoFile();
    std::string name() const;
    std::string package() const;
    const std::vector<ProtoMessage>& messages() const;
private:
    ProtoFile(const google::protobuf::FileDescriptor* fileDescriptor);
private:
    const google::protobuf::FileDescriptor* fileDescriptor;
    std::vector<ProtoMessage> protoMessages;
    friend class ProtoFileLoader;
};

} //namespace
#endif // PROTOFILE_H
