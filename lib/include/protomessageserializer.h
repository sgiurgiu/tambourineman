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

#ifndef TBM_PROTOMESSAGESERIALIZER_H
#define TBM_PROTOMESSAGESERIALIZER_H

#include <string>
#include <stdexcept>
#include "json.hpp"
#include "protofile.h"

namespace tbm {

class ProtoMessage;

class InvalidMessageException : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};
class NotImplementedFieldException : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/**
 * Seriliazes a proto message
 */
class ProtoMessageSerializer {
public:
    /**
    * Constructs a serializer for the specified  proto message.
    * This serializer can be used to serialize multiple messages of this type.
    */
    ProtoMessageSerializer(ProtoMessage* message);
    /**
     * Receives a representation of the message and its data as json format, and it returns
     * the protobuf serialized message. Can be binary or text.
     */
    std::string serializeMessage(const std::string& jsonMessage, bool binary = true) const;
  
private:
    void validateJsonMessage(const nlohmann::json& msg) const;
    size_t calculateMessageSize(const nlohmann::json& msg) const;
    size_t calculateFieldSize(const MessageField& field, const nlohmann::json& value) const;
    nlohmann::json::const_iterator findJsonMember(const MessageField& field,const nlohmann::json& msg) const;
    uint8_t* writeMessageField(const MessageField& field,const nlohmann::json& value, uint8_t* target) const;
    uint8_t* writeMessage(const nlohmann::json& value, uint8_t* target) const;
private:
    ProtoMessage* message;
};

} // namespace tbm

#endif // TBM_PROTOMESSAGESERIALIZER_H
