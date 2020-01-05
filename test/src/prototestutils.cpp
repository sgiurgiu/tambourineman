#include "prototestutils.h"

#include "addressbook.pb.h"
#include <google/protobuf/util/time_util.h>

namespace tbm {

std::string ProtoTestUtils::createSerializedPersonMessage()
{
    tutorial::Person person_message;
    person_message.set_email("person@example.com");
    person_message.set_name("Example Person");
    person_message.set_id(11);
    *person_message.mutable_last_updated() = google::protobuf::util::TimeUtil::SecondsToTimestamp(10);
    auto phone_home = person_message.add_phones();
    phone_home->set_number("1234567890");
    phone_home->set_type(tutorial::Person_PhoneType::Person_PhoneType_HOME);
    auto phone_mobile = person_message.add_phones();
    phone_mobile->set_number("0987654321");
    phone_mobile->set_type(tutorial::Person_PhoneType::Person_PhoneType_MOBILE);
    person_message.set_new_bool(true);
    person_message.set_new_double(0.3);
    person_message.set_new_fixed32(12);
    person_message.set_new_fixed64(13);
    person_message.set_new_float(1.3f);
    person_message.set_new_int64(12);
    person_message.set_new_sfixed32(-12);
    person_message.set_new_sfixed64(-2);
    person_message.set_new_sint32(-12);
    person_message.set_new_sint64(-20);
    person_message.set_new_uint32(11);
    person_message.set_new_uint64(12);

    return person_message.SerializeAsString();
}

std::string ProtoTestUtils::personMessageJsonRepresentation()
{
    return R"({"name":"Example Person","email":"person@example.com",
    "id":11,"last_updated":{"seconds":10,"nanos":0},
    "phones":[{"number":"1234567890","type":"HOME"},
    {"number":"0987654321","type":"MOBILE"}], "new_int64":12, "new_double":0.3,
    "new_float":1.3,"new_uint32":11,"new_uint64":12,"new_sint32":-12,"new_sint64":-20,
    "new_fixed32":12,"new_fixed64":13,"new_sfixed32":-12,"new_sfixed64":-2,"new_bool":true})"
    ;

}

} // namespace tbm
