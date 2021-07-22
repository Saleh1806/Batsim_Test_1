#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_kill_jobs_simple(MessageBuilder &);
void example_kill_jobs_multiple(MessageBuilder &);

TEST(example_kill_jobs, simple)
{
    MessageBuilder mb(true);
    example_kill_jobs_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_KillJobsEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_NE(event0->job_ids(), nullptr);
    EXPECT_EQ(event0->job_ids()->size(), 1u);
    EXPECT_EQ(event0->job_ids()->Get(0)->str(), "w0!0");

    write_test_mb(mb);
}

TEST(example_kill_jobs, multiple)
{
    MessageBuilder mb(true);
    example_kill_jobs_multiple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_KillJobsEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_NE(event0->job_ids(), nullptr);
    EXPECT_EQ(event0->job_ids()->size(), 3u);
    EXPECT_EQ(event0->job_ids()->Get(0)->str(), "w0!3");
    EXPECT_EQ(event0->job_ids()->Get(1)->str(), "w0!7");
    EXPECT_EQ(event0->job_ids()->Get(2)->str(), "dyn!0");

    write_test_mb(mb);
}
