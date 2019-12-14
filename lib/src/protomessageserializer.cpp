/*
 * Copyright (c) 2019, Sergiu Giurgiu <email>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Sergiu Giurgiu <email> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Sergiu Giurgiu <email> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "protomessageserializer.h"
#include "protofile.h"


#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>

#include "base64.h"

namespace tbm {

ProtoMessageSerializer::ProtoMessageSerializer(ProtoMessage* message):message(message)
{}

std::string ProtoMessageSerializer::serializeMessage(const std::string& jsonMessage, bool binary) const
{
    assert(binary);//not implemented text mode yet
    using json = nlohmann::json;
    auto msg = json::parse(jsonMessage);

    validateJsonMessage(msg);
    auto totalSize = calculateMessageSize(msg);
    if (totalSize > INT_MAX) {
      throw InvalidMessageException("Message exceeded maximum protobuf size of 2GB: " + std::to_string(totalSize));
    }
    std::string output;
    output.resize(totalSize,(char)0);
    writeMessage(msg,reinterpret_cast<uint8_t*>(&(output)[0]));
    return output;
}

uint8_t* ProtoMessageSerializer::writeMessage(const nlohmann::json& value, uint8_t* target) const
{
    auto& fields = message->fields();
    for(const auto& field : fields)
    {
        auto found = findJsonMember(field,value);
        if(found == value.end()) continue;
        auto jsonValue = found.value();

        if(jsonValue.is_array() && field.label() == MessageField::LABEL_REPEATED)
        {
            for(const auto& val : jsonValue)
            {
                target = writeMessageField(field,val,target);
            }
        }
        else
        {
            target = writeMessageField(field,jsonValue,target);
        }
    }
    return target;
}

uint8_t* ProtoMessageSerializer::writeMessageField(const MessageField& field,const nlohmann::json& value, uint8_t* target) const
{
    switch(field.type())
    {
    case MessageField::TYPE_MESSAGE:
    {
        auto msg_type = field.messageType();
        ProtoMessageSerializer ser(msg_type.get());
        target = ::google::protobuf::internal::WireFormatLite::WriteTagToArray(field.number(),
                                       ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED, target);
        target = ::google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(
                 static_cast<uint32_t>(ser.calculateMessageSize(value)), target);
        target = ser.writeMessage(value,target);
    }
        break;
    case MessageField::TYPE_STRING:
        target =
          ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
            field.number(), value.get<std::string>(), target);
        break;
    case MessageField::TYPE_BYTES:
    {
        auto val = value.get<std::string>();
        if(val.size() > 0)
        {
            std::string binaryVal = base64_encode(reinterpret_cast<const unsigned char*>(val.c_str()), val.length());
            if(binaryVal.length() > 0)
            {
            target = ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(field.number(),binaryVal,target);
            }
        }
    }
        break;
    case MessageField::TYPE_ENUM:
    {
        if(value.is_number())
        {
            auto val = value.get<int>();
            if(val != 0)
            {
            target =
              ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
                field.number(), val, target);
            }
        }
        else if (value.is_string())
        {
            auto str_val = value.get<std::string>();
            auto protoEnum = field.enumType();
            auto values = protoEnum->values();
            auto foundStrVal = std::find(values.begin(),values.end(),str_val);
            if(foundStrVal != values.end())
            {
                auto val = foundStrVal - values.begin();
                if(val != 0)
                {
                target =
                  ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
                    field.number(), val, target);
                }
            }

        }
    }
        break;
    case MessageField::TYPE_INT32:
    {
        auto val = value.get<int32_t>();
        if(val != 0)
        {
        target =
          ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(
            field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_SINT32:
    {
        auto val = value.get<int32_t>();
        if(val != 0)
        {
        target =
          ::google::protobuf::internal::WireFormatLite::WriteSInt32ToArray(                    
            field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_UINT32:
    {
        auto val = value.get<uint32_t>();
        if(val != 0)
        {
        target =
          ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(
            field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_INT64:
    {
        auto val = value.get<int64_t>();
        if(val != 0)
        {
        target =
          ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(
            field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_SINT64:
    {
        auto val = value.get<int64_t>();
        if(val != 0)
        {
        target =
          ::google::protobuf::internal::WireFormatLite::WriteSInt64ToArray(
            field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_UINT64:
    {
        auto val = value.get<uint64_t>();
        if(val != 0)
        {
        target =
          ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(
            field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_DOUBLE:
    {
        auto val = value.get<double>();
        if(val != 0)
        {
        target =
          ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(
            field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_FLOAT:
    {
        auto val = value.get<float>();
        if(val != 0)
        {
        target =
          ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(
            field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_BOOL:
    {
        auto val = value.get<bool>();
        if(val)
        {
            target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(
                        field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_FIXED32:
    {
        auto val = value.get<uint32_t>();
        if(val != 0)
        {
            target = ::google::protobuf::internal::WireFormatLite::WriteFixed32ToArray(
                        field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_SFIXED32:
    {
        auto val = value.get<int32_t>();
        if(val != 0)
        {
            target = ::google::protobuf::internal::WireFormatLite::WriteSFixed32ToArray(
                        field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_FIXED64:
    {
        auto val = value.get<uint64_t>();
        if(val != 0)
        {
            target = ::google::protobuf::internal::WireFormatLite::WriteFixed64ToArray(
                        field.number(), val, target);
        }
    }
        break;
    case MessageField::TYPE_SFIXED64:
    {
        auto val = value.get<int64_t>();
        if(val != 0)
        {
            target = ::google::protobuf::internal::WireFormatLite::WriteSFixed64ToArray(
                        field.number(), val, target);
        }
    }
        break;
    default:
        throw NotImplementedFieldException("Field of type "+field.typeName()+" is not serializable yet");
    }
    return target;
}

size_t ProtoMessageSerializer::calculateMessageSize(const nlohmann::json& msg) const
{
    size_t total_size = 0;
    auto& fields = message->fields();
    for(const auto& field : fields)
    {
        auto found = findJsonMember(field,msg);
        if(found == msg.end()) continue;
        auto jsonValue = found.value();

        if(jsonValue.is_array() && field.label() == MessageField::LABEL_REPEATED)
        {
            for(const auto& val : jsonValue)
            {
                total_size += calculateFieldSize(field,val);
            }
        }
        else
        {
            total_size += calculateFieldSize(field,jsonValue);
        }
    }

    return total_size;
}

size_t ProtoMessageSerializer::calculateFieldSize(const MessageField& field, const nlohmann::json& value) const
{
    size_t total_size = 0;
    //this means that MessageField::FieldType MUST be the same thing as ::google::protobuf::internal::WireFormatLite::FieldType
    size_t tag_size = ::google::protobuf::internal::WireFormatLite::TagSize(field.number(),
                          (::google::protobuf::internal::WireFormatLite::FieldType)field.type());
    switch(field.type())
    {
    case MessageField::TYPE_MESSAGE:
    {
        auto msg_type = field.messageType();
        ProtoMessageSerializer ser(msg_type.get());
        auto length = ser.calculateMessageSize(value);
        total_size += tag_size + length +  ::google::protobuf::io::CodedOutputStream::VarintSize32(static_cast<uint32_t>(length));
    }
        break;
    case MessageField::TYPE_STRING:
    {
        auto val = value.get<std::string>();
        if(val.size() > 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::StringSize(val);
        }
    }
        break;
    case MessageField::TYPE_BYTES:
    {
        auto val = value.get<std::string>();
        if(val.size() > 0)
        {
            std::string binaryVal = base64_encode(reinterpret_cast<const unsigned char*>(val.c_str()), val.length());
            if(binaryVal.length() > 0)
            {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::BytesSize(binaryVal);
            }
        }
    }
        break;
    case MessageField::TYPE_ENUM:
    {
        if(value.is_number())
        {
            auto val = value.get<int>();
            if(val != 0)
            {
                total_size += tag_size +
                        ::google::protobuf::internal::WireFormatLite::EnumSize(val);
            }
        }
        else if (value.is_string())
        {
            auto str_val = value.get<std::string>();
            auto protoEnum = field.enumType();
            auto values = protoEnum->values();
            auto foundStrVal = std::find(values.begin(),values.end(),str_val);
            if(foundStrVal != values.end())
            {
                auto val = foundStrVal - values.begin();
                if(val != 0)
                {
                    total_size += tag_size +
                            ::google::protobuf::internal::WireFormatLite::EnumSize(val);
                }
            }

        }
    }
        break;
    case MessageField::TYPE_INT32:
    {
        auto val = value.get<int32_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::Int32Size(val);
        }
    }
        break;
    case MessageField::TYPE_SINT32:
    {
        auto val = value.get<int32_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::SInt32Size(val);
        }
    }
        break;
    case MessageField::TYPE_UINT32:
    {
        auto val = value.get<uint32_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::UInt32Size(val);
        }
    }
        break;
    case MessageField::TYPE_INT64:
    {
        auto val = value.get<int64_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::Int64Size(val);
        }
    }
        break;
    case MessageField::TYPE_SINT64:
    {
        auto val = value.get<int64_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::SInt64Size(val);
        }
    }
        break;
    case MessageField::TYPE_UINT64:
    {
        auto val = value.get<uint64_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::UInt64Size(val);
        }
    }
        break;
    case MessageField::TYPE_DOUBLE:
    {
        auto val = value.get<double>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::kDoubleSize;
        }
    }
        break;
    case MessageField::TYPE_FLOAT:
    {
        auto val = value.get<double>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::kFloatSize;
        }
    }
        break;
    case MessageField::TYPE_BOOL:
    {
        auto val = value.get<bool>();
        if(val)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::kBoolSize;
        }
    }
        break;
    case MessageField::TYPE_FIXED32:
    {
        auto val = value.get<uint32_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::kFixed32Size;
        }
    }
        break;
    case MessageField::TYPE_SFIXED32:
    {
        auto val = value.get<int32_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::kSFixed32Size;
        }
    }
        break;
    case MessageField::TYPE_FIXED64:
    {
        auto val = value.get<uint64_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::kFixed64Size;
        }
    }
        break;
    case MessageField::TYPE_SFIXED64:
    {
        auto val = value.get<int64_t>();
        if(val != 0)
        {
            total_size += tag_size +
                    ::google::protobuf::internal::WireFormatLite::kSFixed64Size;
        }
    }
        break;

    default:
        throw NotImplementedFieldException("Field of type "+field.typeName()+" is not serializable yet");
    }

    return total_size;
}
nlohmann::json::const_iterator ProtoMessageSerializer::findJsonMember(const MessageField& field,
                                                                      const nlohmann::json& msg) const
{
    auto found = msg.find(field.jsonName());
    if(found == msg.end())
    {
        found = msg.find(field.name());
    }
    if(found == msg.end())
    {
        auto lower_name = field.name();        
        std::for_each(lower_name.begin(),lower_name.end(),[](char& c){
            c = std::tolower(c);
        });
        found = msg.find(lower_name);
    }
    return found;
}
void ProtoMessageSerializer::validateJsonMessage(const nlohmann::json& msg) const
{
    auto& fields = message->fields();

    for(const auto& field : fields)
    {
        auto found = findJsonMember(field,msg);
        if(found == msg.end())
        {
            if(field.label() == MessageField::LABEL_REQUIRED)
            {
                throw InvalidMessageException("Field "+field.name()+" cannot be found in json message");
            }
            else
            {
                continue;
            }
        }
        auto jsonValue = found.value();
        auto fieldType = field.type();
        if(jsonValue.is_boolean())
        {
            if(fieldType == MessageField::TYPE_BYTES ||
                    fieldType == MessageField::TYPE_MESSAGE ||
                    fieldType == MessageField::TYPE_GROUP ) {
                throw InvalidMessageException("Mismatch type in field "+field.name()+
                                              ". Json reports it as boolean. We have type "+field.typeName());
            }
        }
        if(jsonValue.is_number())
        {
            if(fieldType == MessageField::TYPE_BYTES ||
                    fieldType == MessageField::TYPE_MESSAGE ||
                    fieldType == MessageField::TYPE_GROUP ) {
                throw InvalidMessageException("Mismatch type in field "+field.name()+
                                              ". Json reports it as number. We have type "+field.typeName());
            }
        }
        if(jsonValue.is_string())
        {
            if(fieldType == MessageField::TYPE_MESSAGE ||
                    fieldType == MessageField::TYPE_GROUP ) {
                throw InvalidMessageException("Mismatch type in field "+field.name()+
                                              ". Json reports it as string.We have type "+field.typeName());
            }
        }
        if(jsonValue.is_object())
        {
            if(fieldType != MessageField::TYPE_MESSAGE) {
                throw InvalidMessageException("Mismatch type in field "+field.name()+
                                              ". Json reports it as object.We have type "+field.typeName());
            }
        }
        if(jsonValue.is_array())
        {
            if(field.label() != MessageField::LABEL_REPEATED) {
                throw InvalidMessageException("Mismatch type in field "+field.name()+
                                              ". Json reports it as array but it is not repeated");
            }
        }
    }
}


} //namespace tbm
