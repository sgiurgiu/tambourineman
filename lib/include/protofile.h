#ifndef PROTOFILE_H
#define PROTOFILE_H


namespace google {
namespace protobuf {

class FileDescriptor;

}}

class ProtoFile
{
public:
    ProtoFile();
private:
    google::protobuf::FileDescriptor* fileDescriptor;
};

#endif // PROTOFILE_H
