#include "protofile.h"

#include <google/protobuf/descriptor.h>

namespace tbm {
ProtoFile::ProtoFile()
{

}
ProtoFile::ProtoFile(const google::protobuf::FileDescriptor* fileDescriptor):
    fileDescriptor(fileDescriptor)
{
}
ProtoEnum::ProtoEnum(const google::protobuf::EnumDescriptor* enumDescriptor):
    enumDescriptor(enumDescriptor)
{
}
MessageField::MessageField(const google::protobuf::FieldDescriptor* fieldDescriptor):
    fieldDescriptor(fieldDescriptor)
{
}
ProtoMessage::ProtoMessage(const google::protobuf::Descriptor* messageDescriptor):
    messageDescriptor(messageDescriptor)
{
}




}//namespace
