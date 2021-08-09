#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_job_submitted_host(MessageBuilder &);
void example_job_submitted_core(MessageBuilder &);
void example_job_submitted_ghost(MessageBuilder &);

TEST(example_job_submitted, host)
{
    MessageBuilder mb(true);
    example_job_submitted_host(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobSubmittedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!0");
    EXPECT_EQ(event0->submission_time(), 0.0);
    EXPECT_EQ(event0->job()->walltime(), 3600.0);
    EXPECT_EQ(event0->job()->profile_id()->str(), "w0!prof");
    EXPECT_EQ(event0->job()->rigid(), true);
    EXPECT_EQ(event0->job()->extra_data()->str(), "");
    EXPECT_EQ(event0->job()->computation_resource_request_type(), fb::ComputationResourceRequest_HostNumber);
    auto res_request = event0->job()->computation_resource_request_as_HostNumber();
    EXPECT_NE(res_request, nullptr);
    EXPECT_EQ(res_request->host_number(), 4);

    write_test_mb(mb);
}

TEST(example_job_submitted, core)
{
    MessageBuilder mb(true);
    example_job_submitted_core(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobSubmittedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!1");
    EXPECT_EQ(event0->submission_time(), 1.0);
    EXPECT_EQ(event0->job()->walltime(), -1);
    EXPECT_EQ(event0->job()->profile_id()->str(), "w0!prof");
    EXPECT_EQ(event0->job()->rigid(), false);
    EXPECT_EQ(event0->job()->extra_data()->str(), R"("application": "npb-lu")");
    EXPECT_EQ(event0->job()->computation_resource_request_type(), fb::ComputationResourceRequest_CoreNumber);
    auto res_request = event0->job()->computation_resource_request_as_CoreNumber();
    EXPECT_NE(res_request, nullptr);
    EXPECT_EQ(res_request->core_number(), 16);

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
    EXPECT_EQ(event0->job()->computation_resource_request_type(), fb::ComputationResourceRequest_NONE);
    EXPECT_EQ(event0->job()->computation_resource_request_as_HostNumber(), nullptr);
    EXPECT_EQ(event0->job()->computation_resource_request_as_CoreNumber(), nullptr);

    write_test_mb(mb);
}
