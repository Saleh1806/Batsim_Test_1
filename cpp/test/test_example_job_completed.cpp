#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_job_completed_success(MessageBuilder &);
void example_job_completed_failure(MessageBuilder &);
void example_job_completed_killed(MessageBuilder &);

TEST(example_job_completed, success)
{
    MessageBuilder mb(true);
    example_job_completed_success(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobCompletedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!0");
    EXPECT_EQ(event0->state(), fb::FinalJobState_COMPLETED_SUCCESSFULLY);
    EXPECT_EQ(event0->return_code(), 0);

    write_test_mb(mb);
}

TEST(example_job_completed, failure)
{
    MessageBuilder mb(true);
    example_job_completed_failure(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobCompletedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!1");
    EXPECT_EQ(event0->state(), fb::FinalJobState_COMPLETED_FAILED);
    EXPECT_EQ(event0->return_code(), 1);

    write_test_mb(mb);
}

TEST(example_job_completed, killed)
{
    MessageBuilder mb(true);
    example_job_completed_killed(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobCompletedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!2");
    EXPECT_EQ(event0->state(), fb::FinalJobState_COMPLETED_KILLED);
    EXPECT_EQ(event0->return_code(), 1);

    write_test_mb(mb);
}
