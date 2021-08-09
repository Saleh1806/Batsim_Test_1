#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_requested_call_simple(MessageBuilder &);

TEST(example_requested_call, simple)
{
    MessageBuilder mb(true);
    example_requested_call_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RequestedCallEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->call_me_later_id()->str(), "example_one_shot");

    write_test_mb(mb);
}
