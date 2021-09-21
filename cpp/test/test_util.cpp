#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

void test_util_serial(bool format_json)
{
    using namespace batprotocol;
    MessageBuilder mb(format_json);
    mb.add_simulation_ends();
    mb.finish_message(0.0);

    const uint8_t * serialized_buffer = nullptr;
    uint32_t serialized_buffer_size = 0u;
    serialize_message(mb, format_json, serialized_buffer, serialized_buffer_size);

    MessageBuilder mb2(format_json);
    auto * parsed = deserialize_message(mb2, format_json, serialized_buffer);
    EXPECT_NE(parsed, nullptr);
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_NE(parsed->events(), nullptr);
    EXPECT_EQ(parsed->events()->size(), 1);

    EXPECT_EQ((*parsed->events())[0]->timestamp(), 0.0);
    EXPECT_NE((*parsed->events())[0]->event_as_SimulationEndsEvent(), nullptr);
}

TEST(util_serial, binary)
{
    test_util_serial(false);
}

TEST(util_serial, json)
{
    test_util_serial(true);
}
