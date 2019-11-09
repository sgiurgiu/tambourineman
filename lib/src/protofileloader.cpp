#include "protofileloader.h"
#include "easylogging++.h"

#include <string>
#include <google/protobuf/compiler/importer.h>
#include <vector>

INITIALIZE_EASYLOGGINGPP

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

class ProtoFileLoaderErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
    virtual ~ProtoFileLoaderErrorCollector() override {}
    virtual void AddError(const std::string& filename, int line, int column,
                          const std::string& message) override;
    virtual void AddWarning(const std::string& filename, int line, int column,
                            const std::string& message) override;
public:
    std::vector<ProtoFileLoaderError> errors;
    std::vector<ProtoFileLoaderError> warnings;
};

ProtoFileLoader::ProtoFileLoader()
{
}

void ProtoFileLoader::loadFile(const std::string& file,std::vector<std::string>& includePaths)
{
    LOG(INFO) << "Loading file "<<file<<std::endl;

    google::protobuf::compiler::DiskSourceTree diskSourceTree;
    ProtoFileLoaderErrorCollector errorCollector;
    google::protobuf::compiler::Importer importer(&diskSourceTree,&errorCollector);    
    diskSourceTree.MapPath("",".");

    std::string virtualFile,shadowingDiskFile;
    auto result = diskSourceTree.DiskFileToVirtualFile(file,&virtualFile,&shadowingDiskFile);
    switch (result) {
        case google::protobuf::compiler::DiskSourceTree::SUCCESS:
        break;
        case google::protobuf::compiler::DiskSourceTree::SHADOWED:
        break;
        case google::protobuf::compiler::DiskSourceTree::NO_MAPPING:
        break;
        case google::protobuf::compiler::DiskSourceTree::CANNOT_OPEN:
        break;
    }
    for(const auto& path : includePaths)
    {
        diskSourceTree.MapPath("",path);
    }
    auto descriptor = importer.Import(file);
    if(descriptor)
    {

    }
    /*for(int i=0;i<descriptor->message_type_count();i++)
    {
        auto message = descriptor->message_type(i);
        std::cout << "message:"<<message->name() <<std::endl;
        for(int j=0;j<message->field_count();j++)
        {
            auto field  = message->field(j);
            std::cout << "field:"<<field->name() <<", type:"<<field->type()<<", label:"<<field->label()<<std::endl;
        }
    }
    for(int i=0;i<descriptor->service_count();i++)
    {
        auto message = descriptor->service(i);
        std::cout << "service:"<<message->name() <<std::endl;
    }
    for(int i=0;i<descriptor->service_count();i++)
    {
        auto message = descriptor->service(i);
        std::cout << "service:"<<message->name() <<std::endl;
    }*/
}

void ProtoFileLoaderErrorCollector::AddWarning(const std::string& filename, int line, int column,
                                      const std::string& message)
{
   LOG(WARNING) << "Warning: "<< filename<< " at line "<<line<<
                 " and col "<<column<<". Message:"<<message <<std::endl;
    warnings.emplace_back(filename,line,column,message);

}
void ProtoFileLoaderErrorCollector::AddError(const std::string& filename, int line, int column,
                                      const std::string& message)
{
    errors.emplace_back(filename,line,column,message);
    LOG(ERROR)  << "Error: "<< filename<< " at line "<<line<<
                 " and col "<<column<<". Message: "<<message <<std::endl;
}

}//namespace tbm
