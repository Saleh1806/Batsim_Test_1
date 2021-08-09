#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_register_profile_delay(MessageBuilder &);
void example_register_profile_ptask(MessageBuilder &);
void example_register_profile_ptask_no_comm(MessageBuilder &);
void example_register_profile_ptask_no_comp(MessageBuilder &);
void example_register_profile_ptask_empty(MessageBuilder &);
void example_register_profile_ptask_hg(MessageBuilder &);
void example_register_profile_ptask_hg_total(MessageBuilder &);
void example_register_profile_ptask_hg_storage(MessageBuilder &);
void example_register_profile_ptask_hg_storage_total(MessageBuilder &);
void example_register_profile_ptask_staging_between_storages(MessageBuilder &);
void example_register_profile_trace_replay_smpi(MessageBuilder &);
void example_register_profile_trace_replay_fractional_comp(MessageBuilder &);
void example_register_profile_sequential(MessageBuilder &);
void example_register_profile_forkjoin(MessageBuilder &);
void example_register_profile_ptask_merge(MessageBuilder &);

TEST(example_register_profile, delay)
{
    MessageBuilder mb(true);
    example_register_profile_delay(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!delay0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_DelayProfile);
    auto delay = event0->profile()->profile_as_DelayProfile();
    EXPECT_NE(delay, nullptr);
    EXPECT_EQ(delay->delay(), 10.0);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask)
{
    MessageBuilder mb(true);
    example_register_profile_ptask(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskProfile);
    auto ptask = event0->profile()->profile_as_ParallelTaskProfile();
    EXPECT_NE(ptask, nullptr);

    auto compute = std::vector<double>({1e7, 1e7, 1e7, 1e7});
    auto comm = std::vector<double>({
           0, 8192, 8192,    0,
        8192,    0,    0, 8192,
        8192,    0,    0, 8192,
           0, 8192, 8192,    0
    });
    check_identical_vectors<double>(ptask->computation_vector(), compute);
    check_identical_vectors<double>(ptask->communication_matrix(), comm);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_no_comm)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_no_comm(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask1");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskProfile);
    auto ptask = event0->profile()->profile_as_ParallelTaskProfile();
    EXPECT_NE(ptask, nullptr);

    auto compute = std::vector<double>({1e7, 1e7, 1e7, 1e7});
    check_identical_vectors<double>(ptask->computation_vector(), compute);
    EXPECT_EQ(ptask->communication_matrix(), nullptr);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_no_comp)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_no_comp(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask2");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskProfile);
    auto ptask = event0->profile()->profile_as_ParallelTaskProfile();
    EXPECT_NE(ptask, nullptr);

    auto comm = std::vector<double>({
           0, 8192, 8192,    0,
        8192,    0,    0, 8192,
        8192,    0,    0, 8192,
           0, 8192, 8192,    0
    });
    check_identical_vectors<double>(ptask->communication_matrix(), comm);
    EXPECT_EQ(ptask->computation_vector(), nullptr);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_empty)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_empty(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask3");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskProfile);
    auto ptask = event0->profile()->profile_as_ParallelTaskProfile();
    EXPECT_NE(ptask, nullptr);

    EXPECT_EQ(ptask->computation_vector(), nullptr);
    EXPECT_EQ(ptask->communication_matrix(), nullptr);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_hg)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_hg(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask_hg0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskHomogeneousProfile);
    auto ptask_hg = event0->profile()->profile_as_ParallelTaskHomogeneousProfile();
    EXPECT_NE(ptask_hg, nullptr);
    EXPECT_EQ(ptask_hg->generation_strategy(), fb::HomogeneousParallelTaskGenerationStrategy_DefinedAmountsUsedForEachValue);
    EXPECT_EQ(ptask_hg->computation_amount(), 1e7);
    EXPECT_EQ(ptask_hg->communication_amount(), 8192);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_hg_total)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_hg_total(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask_hg1");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskHomogeneousProfile);
    auto ptask_hg = event0->profile()->profile_as_ParallelTaskHomogeneousProfile();
    EXPECT_NE(ptask_hg, nullptr);
    EXPECT_EQ(ptask_hg->generation_strategy(), fb::HomogeneousParallelTaskGenerationStrategy_DefinedAmountsSpreadUniformly);
    EXPECT_EQ(ptask_hg->computation_amount(), 1e7);
    EXPECT_EQ(ptask_hg->communication_amount(), 8192);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_hg_storage)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_hg_storage(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask_hg_storage0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskOnStorageHomogeneousProfile);
    auto ptask_hg_storage = event0->profile()->profile_as_ParallelTaskOnStorageHomogeneousProfile();
    EXPECT_NE(ptask_hg_storage, nullptr);
    EXPECT_EQ(ptask_hg_storage->generation_strategy(), fb::HomogeneousParallelTaskGenerationStrategy_DefinedAmountsUsedForEachValue);
    EXPECT_EQ(ptask_hg_storage->storage_name()->str(), "pfs");
    EXPECT_EQ(ptask_hg_storage->bytes_to_read(), 0.0);
    EXPECT_EQ(ptask_hg_storage->bytes_to_write(), 4e6);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_hg_storage_total)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_hg_storage_total(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask_hg_storage1");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskOnStorageHomogeneousProfile);
    auto ptask_hg_storage = event0->profile()->profile_as_ParallelTaskOnStorageHomogeneousProfile();
    EXPECT_NE(ptask_hg_storage, nullptr);
    EXPECT_EQ(ptask_hg_storage->generation_strategy(), fb::HomogeneousParallelTaskGenerationStrategy_DefinedAmountsSpreadUniformly);
    EXPECT_EQ(ptask_hg_storage->storage_name()->str(), "pfs");
    EXPECT_EQ(ptask_hg_storage->bytes_to_read(), 1e9);
    EXPECT_EQ(ptask_hg_storage->bytes_to_write(), 0.0);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_staging_between_storages)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_staging_between_storages(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask_staging_storages0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskDataStagingBetweenStoragesProfile);
    auto replay = event0->profile()->profile_as_ParallelTaskDataStagingBetweenStoragesProfile();
    EXPECT_NE(replay, nullptr);
    EXPECT_EQ(replay->bytes_to_transfer(), 1e6);
    EXPECT_EQ(replay->emitter_storage_name()->str(), "pfs");
    EXPECT_EQ(replay->receiver_storage_name()->str(), "burst_buffer");

    write_test_mb(mb);
}

TEST(example_register_profile, trace_replay_smpi)
{
    MessageBuilder mb(true);
    example_register_profile_trace_replay_smpi(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!replay_smpi0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_TraceReplayProfile);
    auto replay = event0->profile()->profile_as_TraceReplayProfile();
    EXPECT_NE(replay, nullptr);
    EXPECT_EQ(replay->trace_type(), fb::TraceType_SMPI);
    EXPECT_EQ(replay->filename()->str(), "npb-lu/traces-smpi.txt");

    write_test_mb(mb);
}

TEST(example_register_profile, trace_replay_fractional_comp)
{
    MessageBuilder mb(true);
    example_register_profile_trace_replay_fractional_comp(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!replay_frac_comp0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_TraceReplayProfile);
    auto replay = event0->profile()->profile_as_TraceReplayProfile();
    EXPECT_NE(replay, nullptr);
    EXPECT_EQ(replay->trace_type(), fb::TraceType_FractionalComputation);
    EXPECT_EQ(replay->filename()->str(), "npb-lu/traces-fractional-comp.txt");

    write_test_mb(mb);
}

TEST(example_register_profile, sequential)
{
    MessageBuilder mb(true);
    example_register_profile_sequential(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!sequential0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_SequentialCompositionProfile);
    auto sequential = event0->profile()->profile_as_SequentialCompositionProfile();
    EXPECT_NE(sequential, nullptr);
    EXPECT_EQ(sequential->repetition_count(), 3);

    auto sub_profiles = std::vector<std::string>({
        "w0!ptask",
        "dyn!ptask_hg0"
    });
    check_identical_str_vectors(sequential->profile_ids(), sub_profiles);

    write_test_mb(mb);
}

TEST(example_register_profile, forkjoin)
{
    MessageBuilder mb(true);
    example_register_profile_forkjoin(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!forkjoin0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ForkJoinCompositionProfile);
    auto forkjoin = event0->profile()->profile_as_ForkJoinCompositionProfile();
    EXPECT_NE(forkjoin, nullptr);

    auto sub_profiles = std::vector<std::string>({
        "w0!ptask",
        "dyn!ptask_hg0"
    });
    check_identical_str_vectors(forkjoin->profile_ids(), sub_profiles);

    write_test_mb(mb);
}

TEST(example_register_profile, ptask_merge)
{
    MessageBuilder mb(true);
    example_register_profile_ptask_merge(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterProfileEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->profile()->id()->str(), "dyn!ptask_merge0");
    EXPECT_EQ(event0->profile()->profile_type(), fb::Profile_ParallelTaskMergeCompositionProfile);
    auto ptask_merge = event0->profile()->profile_as_ParallelTaskMergeCompositionProfile();
    EXPECT_NE(ptask_merge, nullptr);

    auto sub_profiles = std::vector<std::string>({
        "w0!ptask",
        "dyn!ptask_hg0"
    });
    check_identical_str_vectors(ptask_merge->profile_ids(), sub_profiles);

    write_test_mb(mb);
}
