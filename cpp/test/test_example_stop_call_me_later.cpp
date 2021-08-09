#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_stop_call_me_later(MessageBuilder &);

TEST(example_stop_call_me_later, simple)
{
    MessageBuilder mb(true);
    example_stop_call_me_later(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_StopCallMeLaterEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->call_me_later_id()->str(), "example_infinite_period");

    write_test_mb(mb);
}
