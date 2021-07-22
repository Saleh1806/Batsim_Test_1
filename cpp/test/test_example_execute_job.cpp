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

TEST(example_execute_job, predefined_placement)
{
    MessageBuilder mb(true);
    execute_job_predefined_placement(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 2u);

    auto event0 = (*parsed->events())[0]->event_as_ExecuteJobEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!1");
    EXPECT_EQ(event0->allocation()->host_allocation()->str(), "0-4");
    EXPECT_EQ(event0->profile_allocation_override()->size(), 0);
    EXPECT_EQ(event0->storage_placement()->size(), 0);
    EXPECT_EQ(event0->allocation()->executor_placement_as_CustomExecutorToHostMapping(), nullptr);
    auto placement = event0->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement, nullptr);
    EXPECT_EQ(placement->strategy(), fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst);

    auto event1 = (*parsed->events())[1]->event_as_ExecuteJobEvent();
    EXPECT_NE(event1, nullptr);
    EXPECT_EQ(event1->job_id()->str(), "w0!2");
    EXPECT_EQ(event1->allocation()->host_allocation()->str(), "0-4");
    EXPECT_EQ(event1->profile_allocation_override()->size(), 0);
    EXPECT_EQ(event1->storage_placement()->size(), 0);
    EXPECT_EQ(event1->allocation()->executor_placement_as_CustomExecutorToHostMapping(), nullptr);
    placement = event1->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement, nullptr);
    EXPECT_EQ(placement->strategy(), fb::PredefinedExecutorPlacementStrategy_FillOneHostCoresFirst);

    write_test_mb(mb);
}

TEST(example_execute_job, custom_placement)
{
    MessageBuilder mb(true);
    execute_job_custom_placement(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 2u);

    auto event0 = (*parsed->events())[0]->event_as_ExecuteJobEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!3");
    EXPECT_EQ(event0->allocation()->host_allocation()->str(), "13 17");
    EXPECT_EQ(event0->profile_allocation_override()->size(), 0);
    EXPECT_EQ(event0->storage_placement()->size(), 0);
    EXPECT_EQ(event0->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper(), nullptr);
    auto placement = event0->allocation()->executor_placement_as_CustomExecutorToHostMapping();
    EXPECT_NE(placement, nullptr);
    check_identical_vectors<uint32_t>(placement->mapping(), std::vector<uint32_t>({0,0,0,0}));

    auto event1 = (*parsed->events())[1]->event_as_ExecuteJobEvent();
    EXPECT_NE(event1, nullptr);
    EXPECT_EQ(event1->job_id()->str(), "w0!4");
    EXPECT_EQ(event1->allocation()->host_allocation()->str(), "37 51");
    EXPECT_EQ(event1->profile_allocation_override()->size(), 0);
    EXPECT_EQ(event1->storage_placement()->size(), 0);
    EXPECT_EQ(event1->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper(), nullptr);
    placement = event1->allocation()->executor_placement_as_CustomExecutorToHostMapping();
    EXPECT_NE(placement, nullptr);
    check_identical_vectors<uint32_t>(placement->mapping(), std::vector<uint32_t>({0,1,1,0}));

    write_test_mb(mb);
}

TEST(example_execute_job, storage_mapping)
{
    MessageBuilder mb(true);
    execute_job_storage_mapping(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 2u);

    auto event0 = (*parsed->events())[0]->event_as_ExecuteJobEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!5");
    EXPECT_EQ(event0->allocation()->host_allocation()->str(), "0-3 19");
    EXPECT_EQ(event0->profile_allocation_override()->size(), 0);
    EXPECT_EQ(event0->storage_placement()->size(), 1);
    EXPECT_EQ(event0->storage_placement()->Get(0)->storage_name()->str(), "pfs");
    EXPECT_EQ(event0->storage_placement()->Get(0)->host_id(), 19);
    EXPECT_EQ(event0->allocation()->executor_placement_as_CustomExecutorToHostMapping(), nullptr);
    auto placement = event0->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement, nullptr);
    EXPECT_EQ(placement->strategy(), fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst);

    auto event1 = (*parsed->events())[1]->event_as_ExecuteJobEvent();
    EXPECT_NE(event1, nullptr);
    EXPECT_EQ(event1->job_id()->str(), "w0!6");
    EXPECT_EQ(event1->allocation()->host_allocation()->str(), "0-3 13 21");
    EXPECT_EQ(event1->profile_allocation_override()->size(), 0);
    EXPECT_EQ(event1->storage_placement()->size(), 2);
    EXPECT_EQ(event1->storage_placement()->Get(0)->storage_name()->str(), "burst_buffer");
    EXPECT_EQ(event1->storage_placement()->Get(0)->host_id(), 13);
    EXPECT_EQ(event1->storage_placement()->Get(1)->storage_name()->str(), "pfs");
    EXPECT_EQ(event1->storage_placement()->Get(1)->host_id(), 19);
    EXPECT_EQ(event1->allocation()->executor_placement_as_CustomExecutorToHostMapping(), nullptr);
    placement = event1->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement, nullptr);
    EXPECT_EQ(placement->strategy(), fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst);

    write_test_mb(mb);
}

TEST(example_execute_job, subprofile_placement)
{
    MessageBuilder mb(true);
    execute_job_subprofile_placement(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 3u);

    auto event0 = (*parsed->events())[0]->event_as_ExecuteJobEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!7");
    EXPECT_EQ(event0->allocation()->host_allocation()->str(), "0-3");
    EXPECT_EQ(event0->profile_allocation_override()->size(), 1);
    auto override = event0->profile_allocation_override()->Get(0);
    EXPECT_EQ(override->profile_id()->str(), "w0!ptask1");
    EXPECT_EQ(override->host_allocation()->str(), "0-3");
    EXPECT_EQ(override->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper(), nullptr);
    auto mapping = override->executor_placement_as_CustomExecutorToHostMapping();
    EXPECT_NE(mapping, nullptr);
    check_identical_vectors<uint32_t>(mapping->mapping(), std::vector<uint32_t>({0,0,0,0}));
    EXPECT_EQ(event0->storage_placement()->size(), 0);
    auto placement = event0->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement, nullptr);
    EXPECT_EQ(placement->strategy(), fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst);

    auto event1 = (*parsed->events())[1]->event_as_ExecuteJobEvent();
    EXPECT_NE(event1, nullptr);
    EXPECT_EQ(event1->job_id()->str(), "w0!8");
    EXPECT_EQ(event1->allocation()->host_allocation()->str(), "13 21");
    EXPECT_EQ(event1->profile_allocation_override()->size(), 2);
    override = event1->profile_allocation_override()->Get(0);
    EXPECT_EQ(override->profile_id()->str(), "w0!ptask1!0");
    EXPECT_EQ(override->host_allocation()->str(), "13");
    EXPECT_EQ(override->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper(), nullptr);
    mapping = override->executor_placement_as_CustomExecutorToHostMapping();
    EXPECT_NE(mapping, nullptr);
    check_identical_vectors<uint32_t>(mapping->mapping(), std::vector<uint32_t>({0,0,0,0}));
    override = event1->profile_allocation_override()->Get(1);
    EXPECT_EQ(override->profile_id()->str(), "w0!ptask1!1");
    EXPECT_EQ(override->host_allocation()->str(), "21");
    EXPECT_EQ(override->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper(), nullptr);
    mapping = override->executor_placement_as_CustomExecutorToHostMapping();
    EXPECT_NE(mapping, nullptr);
    check_identical_vectors<uint32_t>(mapping->mapping(), std::vector<uint32_t>({0,0,0,0}));
    EXPECT_EQ(event1->storage_placement()->size(), 0);
    placement = event1->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement, nullptr);
    EXPECT_EQ(placement->strategy(), fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst);

    auto event2 = (*parsed->events())[2]->event_as_ExecuteJobEvent();
    EXPECT_NE(event2, nullptr);
    EXPECT_EQ(event2->job_id()->str(), "w0!9");
    EXPECT_EQ(event2->allocation()->host_allocation()->str(), "13 21");
    EXPECT_EQ(event2->profile_allocation_override()->size(), 1);
    override = event2->profile_allocation_override()->Get(0);
    EXPECT_EQ(override->profile_id()->str(), "w0!ptask1!0");
    EXPECT_EQ(override->host_allocation()->str(), "13 21");
    EXPECT_EQ(override->executor_placement_as_CustomExecutorToHostMapping(), nullptr);
    auto placement_override = override->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement_override, nullptr);
    EXPECT_EQ(placement_override->strategy(), fb::PredefinedExecutorPlacementStrategy_FillOneHostCoresFirst);
    EXPECT_EQ(event2->storage_placement()->size(), 0);
    placement = event2->allocation()->executor_placement_as_PredefinedExecutorPlacementStrategyWrapper();
    EXPECT_NE(placement, nullptr);
    EXPECT_EQ(placement->strategy(), fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst);

    write_test_mb(mb);
}
