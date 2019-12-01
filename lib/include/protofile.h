#ifndef PROTOFILE_H
#define PROTOFILE_H

#include <string>
#include <vector>
#include <memory>

namespace google {
namespace protobuf {

class FileDescriptor;
class Descriptor;
class FieldDescriptor;
class OneofDescriptor;
class EnumDescriptor;
}}

namespace tbm {
class ProtoMessage;

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
    std::vector<std::string> _values;
    friend class ProtoMessage;
    friend class MessageField;
};

class MessageField
{
public:
    std::string name() const;
    std::string fullName() const;
    std::string jsonName() const;
    int number() const;

    enum Label {
        LABEL_OPTIONAL      = 1,    // optional
        LABEL_REQUIRED      = 2,    // required
        LABEL_REPEATED      = 3,    // repeated

        MAX_LABEL           = 3,    // Constant useful for defining lookup tables
        // indexed by Label.
    };
    Label label() const;   
    bool isPackable() const;
    bool isPacked() const;
    bool isMap() const;

    //copied from the original. Will have to be maintained should the original add/remove any
    enum Type {
        TYPE_DOUBLE         = 1,   // double, exactly eight bytes on the wire.
        TYPE_FLOAT          = 2,   // float, exactly four bytes on the wire.
        TYPE_INT64          = 3,   // int64, varint on the wire.  Negative numbers
                                    // take 10 bytes.  Use TYPE_SINT64 if negative
                                    // values are likely.
        TYPE_UINT64         = 4,   // uint64, varint on the wire.
        TYPE_INT32          = 5,   // int32, varint on the wire.  Negative numbers
                                    // take 10 bytes.  Use TYPE_SINT32 if negative
                                    // values are likely.
        TYPE_FIXED64        = 6,   // uint64, exactly eight bytes on the wire.
        TYPE_FIXED32        = 7,   // uint32, exactly four bytes on the wire.
        TYPE_BOOL           = 8,   // bool, varint on the wire.
        TYPE_STRING         = 9,   // UTF-8 text.
        TYPE_GROUP          = 10,  // Tag-delimited message.  Deprecated.
        TYPE_MESSAGE        = 11,  // Length-delimited message.

        TYPE_BYTES          = 12,  // Arbitrary byte array.
        TYPE_UINT32         = 13,  // uint32, varint on the wire
        TYPE_ENUM           = 14,  // Enum, varint on the wire
        TYPE_SFIXED32       = 15,  // int32, exactly four bytes on the wire
        TYPE_SFIXED64       = 16,  // int64, exactly eight bytes on the wire
        TYPE_SINT32         = 17,  // int32, ZigZag-encoded varint on the wire
        TYPE_SINT64         = 18,  // int64, ZigZag-encoded varint on the wire

        MAX_TYPE            = 18,  // Constant useful for defining lookup tables
        // indexed by Type.
    };

    Type type() const;
    std::string typeName() const;
    std::string containingMessageType() const;
    std::unique_ptr<ProtoMessage> messageType() const;
    std::unique_ptr<ProtoEnum> enumType() const;
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
    std::vector<ProtoMessage> internalMessages;
    friend class ProtoFile;
    friend class MessageField;
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
