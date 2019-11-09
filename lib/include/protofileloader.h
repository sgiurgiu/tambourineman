#ifndef PROTOFILELOADER_H
#define PROTOFILELOADER_H

#include <string>
#include <vector>
#include <memory>

namespace tbm {

class ProtoFileLoader
{

public:
    ProtoFileLoader();
    //should this be a static method?
    void loadFile(const std::string& file,std::vector<std::string>& includePaths);
};

}

#endif // PROTOFILELOADER_H
