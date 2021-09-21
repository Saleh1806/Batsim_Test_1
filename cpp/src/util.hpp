#pragma once

#include <cstdint>

namespace batprotocol
{
    class MessageBuilder;
    namespace fb
    {
        struct Message;
    }

    const fb::Message * deserialize_message(
        MessageBuilder & mb,
        bool format_json,
        const uint8_t * input_buffer
    );

    void serialize_message(
        MessageBuilder & mb,
        bool format_json,
        const uint8_t *& output_buffer,
        uint32_t & output_buffer_size
    );

    void serialize_message(
        MessageBuilder & mb,
        bool format_json,
        const uint8_t ** output_buffer,
        uint32_t * output_buffer_size
    );
}
