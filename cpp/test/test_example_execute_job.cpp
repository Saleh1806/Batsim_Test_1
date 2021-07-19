#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void execute_job_simple(MessageBuilder &);
void execute_job_predefined_placement(MessageBuilder &);
void execute_job_custom_placement(MessageBuilder &);
void execute_job_storage_mapping(MessageBuilder &);
void execute_job_subprofile_placement(MessageBuilder &);

void all_examples_execute_job(MessageBuilder & mb)
{
    execute_job_simple(mb);
    execute_job_predefined_placement(mb);
    execute_job_custom_placement(mb);
    execute_job_storage_mapping(mb);
    execute_job_subprofile_placement(mb);
}

TEST(example_execute_job, simple)
{
    MessageBuilder mb(true);
    execute_job_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExecuteJobEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!0");
    EXPECT_EQ(event0->allocation()->host_allocation()->str(), "0-1");
    EXPECT_EQ(event0->profile_allocation_override()->size(), 0);
    EXPECT_EQ(event0->storage_placement()->size(), 0);

    auto placement = event0->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement, nullptr);
    EXPECT_EQ(placement->strategy(), fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst);

    write_test_mb(mb);
}
