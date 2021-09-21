#include "util.hpp"

#include "message_builder.hpp"
#include "batprotocol_generated.h"

namespace batprotocol
{

const fb::Message * deserialize_message(
    MessageBuilder & mb,
    bool format_json,
    const uint8_t * input_buffer)
{
    // If the message is formatted in binary, directly use the input_buffer pointer
    // (which is a flatbuffers Message byte buffer in this case).
    uint8_t * buffer = const_cast<uint8_t *>(input_buffer);

    // If the message is in JSON, first generate a binary buffer from the input_buffer
    // (which is a null-terminated C string in this case).
    if (format_json)
        mb.parse_json_message((const char *)input_buffer, buffer);

    return fb::GetMessage(buffer);
}

void serialize_message(
    MessageBuilder & mb,
    bool format_json,
    const uint8_t *& output_buffer,
    uint32_t & output_buffer_size)
{
    serialize_message(mb, format_json, &output_buffer, &output_buffer_size);
}

void serialize_message(
    MessageBuilder & mb,
    bool format_json,
    const uint8_t ** output_buffer,
    uint32_t * output_buffer_size)
{
    if (!format_json)
    {
        // If the message must be formatted in binary,
        // just return MessageBuilder pointer/size.
        *output_buffer = mb.buffer_pointer();
        *output_buffer_size = mb.buffer_size();
    }
    else
    {
        // If the message must be formatted in JSON,
        // generate a JSON flatbuffers Message byte buffer then return its pointer/size.
        auto * buffer_str = mb.buffer_as_json();
        *output_buffer = (uint8_t*) buffer_str->c_str();
        *output_buffer_size = buffer_str->size();
    }
}

} // end of namespace batprotocol
