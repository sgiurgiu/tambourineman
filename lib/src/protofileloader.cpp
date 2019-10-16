#include "protofileloader.h"

#include <string>
#include <iostream>
#include <google/protobuf/compiler/importer.h>

class StdOutErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
    virtual ~StdOutErrorCollector() override {}
    virtual void AddError(const std::string& filename, int line, int column,
                          const std::string& message) override;
    virtual void AddWarning(const std::string& filename, int line, int column,
                            const std::string& message) override;
};

ProtoFileLoader::ProtoFileLoader()
{
}

void ProtoFileLoader::loadFile(std::string_view dir,std::string_view file)
{
    std::cerr << "Loading file "<<file<<std::endl;

    google::protobuf::compiler::DiskSourceTree diskSourceTree;
    StdOutErrorCollector errorCollector;
    google::protobuf::compiler::Importer importer(&diskSourceTree,&errorCollector);
    std::string fileStr(file);
    diskSourceTree.MapPath(fileStr,fileStr);
    diskSourceTree.MapPath("",std::string(dir));
    auto descriptor = importer.Import(fileStr);
    for(int i=0;i<descriptor->message_type_count();i++)
    {
        auto message = descriptor->message_type(i);
        std::cout << message->name() <<std::endl;
    }
}

void StdOutErrorCollector::AddWarning(const std::string& filename, int line, int column,
                                      const std::string& message)
{
    std::cerr << "Warning: "<< filename<< " at line "<<line<<
                 " and col "<<column<<". Message:"<<message <<std::endl;
}
void StdOutErrorCollector::AddError(const std::string& filename, int line, int column,
                                      const std::string& message)
{
    std::cerr << "Error: "<< filename<< " at line "<<line<<
                 " and col "<<column<<". Message: "<<message <<std::endl;
}
