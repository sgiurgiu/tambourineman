#ifndef PROTOTESTUTILS_H
#define PROTOTESTUTILS_H


#include <string>

namespace tbm {

class ProtoTestUtils
{
public:
    static std::string createSerializedPersonMessage();
    static std::string personMessageJsonRepresentation();
};


} //namespace tbm

#endif // PROTOTESTUTILS_H
