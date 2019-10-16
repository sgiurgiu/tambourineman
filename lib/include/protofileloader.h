#ifndef PROTOFILELOADER_H
#define PROTOFILELOADER_H

#include <string_view>

class ProtoFileLoader
{

public:
    ProtoFileLoader();
    void loadFile(std::string_view dir,std::string_view file);
};

#endif // PROTOFILELOADER_H
