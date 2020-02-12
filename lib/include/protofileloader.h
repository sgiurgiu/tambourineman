#ifndef PROTOFILELOADER_H
#define PROTOFILELOADER_H

#include <string>
#include <vector>
#include <memory>
#include <exception>

#include "protofile.h"
#include "tbm_exports.h"

namespace google {
namespace protobuf {
namespace compiler {
    
class DiskSourceTree;
class Importer;
 
}}}

namespace tbm {
class ProtoFileLoaderErrorCollector;
struct TBM_EXPORT ProtoFileLoaderError {
    ProtoFileLoaderError(){}
    ProtoFileLoaderError(const std::string& filename, int line, int column,
                         const std::string& message):
        filename(filename),line(line),column(column),message(message)
    {}
    std::string filename;
    int line = -1;
    int column = -1;
    std::string message;
};


class TBM_EXPORT ProtoFileLoaderException : public std::exception
{
public:
    ProtoFileLoaderException(const std::vector<ProtoFileLoaderError>& errors,
                             const std::vector<ProtoFileLoaderError>& warnings);
    virtual const char* what() const noexcept;
    std::vector<ProtoFileLoaderError> getErrors() const;
    std::vector<ProtoFileLoaderError> getWarnings() const;

private:
    std::vector<ProtoFileLoaderError> errors;
    std::vector<ProtoFileLoaderError> warnings;
};

class TBM_EXPORT ProtoFileLoader
{

public:
    ProtoFileLoader();
    ~ProtoFileLoader();
    ProtoFile loadFile(const std::string& file,std::vector<std::string>& includePaths);
private:
    std::unique_ptr<google::protobuf::compiler::DiskSourceTree> diskSourceTree;
    std::unique_ptr<ProtoFileLoaderErrorCollector> errorCollector;
    std::unique_ptr<google::protobuf::compiler::Importer> importer;
};

}

#endif // PROTOFILELOADER_H
