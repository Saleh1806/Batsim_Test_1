#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_call_me_later_one_shot(MessageBuilder &);
void example_call_me_later_periodic(MessageBuilder &);
void example_call_me_later_periodic_finite(MessageBuilder &);

TEST(example_call_me_later, one_shot)
{
    MessageBuilder mb(true);
    example_call_me_later_one_shot(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CallMeLaterEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->call_me_later_id()->str(), "example_one_shot");
    auto when = event0->when_as_OneShot();
    EXPECT_NE(when, nullptr);
    EXPECT_EQ(when->time(), 42.0);

    write_test_mb(mb);
}

TEST(example_call_me_later, periodic)
{
    MessageBuilder mb(true);
    example_call_me_later_periodic(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CallMeLaterEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->call_me_later_id()->str(), "example_infinite_period");
    auto when = event0->when_as_Periodic();
    EXPECT_NE(when, nullptr);
    EXPECT_EQ(when->start_time(), 42.0);
    EXPECT_EQ(when->period(), 10.0);
    EXPECT_EQ(when->mode_type(), fb::PeriodicMode_Infinite);

    write_test_mb(mb);
}

TEST(example_call_me_later, periodic_finite)
{
    MessageBuilder mb(true);
    example_call_me_later_periodic_finite(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CallMeLaterEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->call_me_later_id()->str(), "example_finite_period");
    auto when = event0->when_as_Periodic();
    EXPECT_NE(when, nullptr);
    EXPECT_EQ(when->start_time(), 42.0);
    EXPECT_EQ(when->period(), 10.0);
    EXPECT_EQ(when->mode_type(), fb::PeriodicMode_FinitePeriodNumber);
    auto finite = when->mode_as_FinitePeriodNumber();
    EXPECT_NE(finite, nullptr);
    EXPECT_EQ(finite->nb_periods(), 5);

    write_test_mb(mb);
}
