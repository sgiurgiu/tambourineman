#ifndef PROTOFILELOADER_H
#define PROTOFILELOADER_H

#include <string>
#include <vector>
#include <memory>
#include <exception>

#include "protofile.h"

namespace tbm {

struct ProtoFileLoaderError {
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


class ProtoFileLoaderException : public std::exception
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

class ProtoFileLoader
{

public:
    ProtoFileLoader();
    //should this be a static method?
    ProtoFile loadFile(const std::string& file,std::vector<std::string>& includePaths);
};

}

#endif // PROTOFILELOADER_H
