#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_trigger_probe_force(MessageBuilder &);
void example_trigger_probe_no_force(MessageBuilder &);

TEST(example_trigger_probe, force)
{
    MessageBuilder mb(true);
    example_trigger_probe_force(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_TriggerProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-all-hosts");
    EXPECT_EQ(event0->force_data_emission(), true);

    write_test_mb(mb);
}

TEST(example_trigger_probe, no_force)
{
    MessageBuilder mb(true);
    example_trigger_probe_no_force(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_TriggerProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-all-hosts");
    EXPECT_EQ(event0->force_data_emission(), false);

    write_test_mb(mb);
}
