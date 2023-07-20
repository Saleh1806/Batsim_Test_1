#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_job_submitted_simple(MessageBuilder &);
void example_job_submitted_extra_data(MessageBuilder &);
void example_job_submitted_ghost(MessageBuilder &);

TEST(example_job_submitted, simple)
{
    MessageBuilder mb(true);
    example_job_submitted_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobSubmittedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!0");
    EXPECT_EQ(event0->submission_time(), 0.0);
    EXPECT_EQ(event0->job()->resource_request(), 4);
    EXPECT_EQ(event0->job()->walltime(), 3600.0);
    EXPECT_EQ(event0->job()->profile_id()->str(), "w0!prof");
    EXPECT_EQ(event0->job()->rigid(), true);
    EXPECT_EQ(event0->job()->extra_data()->str(), "");

    write_test_mb(mb);
}

TEST(example_job_submitted, extra_data)
{
    MessageBuilder mb(true);
    example_job_submitted_extra_data(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobSubmittedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!1");
    EXPECT_EQ(event0->submission_time(), 1.0);
    EXPECT_EQ(event0->job()->resource_request(), 16);
    EXPECT_EQ(event0->job()->walltime(), -1);
    EXPECT_EQ(event0->job()->profile_id()->str(), "w0!prof");
    EXPECT_EQ(event0->job()->rigid(), false);
    EXPECT_EQ(event0->job()->extra_data()->str(), R"("application": "npb-lu")");

    write_test_mb(mb);
}

TEST(example_job_submitted, ghost)
{
    MessageBuilder mb(true);
    example_job_submitted_ghost(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobSubmittedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!2");
    EXPECT_EQ(event0->submission_time(), 2.0);
    EXPECT_EQ(event0->job()->walltime(), -1);
    EXPECT_EQ(event0->job()->profile_id()->str(), "");
    EXPECT_EQ(event0->job()->rigid(), true);
    EXPECT_EQ(event0->job()->extra_data()->str(), "... (some data to do dynamic registrations)");
    EXPECT_EQ(event0->job()->resource_request(), 0);

    write_test_mb(mb);
}
