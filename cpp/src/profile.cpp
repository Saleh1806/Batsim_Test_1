#include "profile.hpp"

namespace batprotocol
{

std::shared_ptr<Profile> Profile::make_delay(double delay)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_DelayProfile;
    profile->_delay = delay;

    return profile;
}

std::shared_ptr<Profile> Profile::make_parallel_task(
    const std::shared_ptr<std::vector<double> > & computation_vector,
    const std::shared_ptr<std::vector<double> > & communication_matrix)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_ParallelTaskProfile;
    profile->_computation_vector = computation_vector;
    profile->_communication_matrix = communication_matrix;

    return profile;
}

std::shared_ptr<Profile> Profile::make_parallel_task_homogeneous(
    fb::HomogeneousParallelTaskGenerationStrategy generation_strategy,
    double computation_amount,
    double communication_amount)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_ParallelTaskHomogeneousProfile;
    profile->_generation_strategy = generation_strategy;
    profile->_computation_amount = computation_amount;
    profile->_communication_amount = communication_amount;

    return profile;
}

std::shared_ptr<Profile> Profile::make_parallel_task_data_staging_between_storages(
    double bytes_to_transfer,
    const std::string & emitter_storage_name,
    const std::string & receiver_storage_name)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_ParallelTaskDataStagingBetweenStoragesProfiles;
    profile->_bytes_to_transfer = bytes_to_transfer;
    profile->_emitter_storage_name = emitter_storage_name;
    profile->_receiver_storage_name = receiver_storage_name;

    return profile;
}

std::shared_ptr<Profile> Profile::make_trace_replay_smpi(
    const std::string & filename)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_TraceReplayProfile;
    profile->_trace_type = fb::TraceType_SMPI;
    profile->_filename = filename;

    return profile;
}

std::shared_ptr<Profile> Profile::make_trace_replay_fractional_computation(
    const std::string & filename)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_TraceReplayProfile;
    profile->_trace_type = fb::TraceType_FractionalComputation;
    profile->_filename = filename;

    return profile;
}

std::shared_ptr<Profile> Profile::make_sequential_composition(
    const std::shared_ptr<std::vector<std::string> > & sub_profiles,
    uint32_t repetition_count)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_SequentialCompositionProfile;
    profile->_sub_profiles = sub_profiles;
    profile->_repetition_count = repetition_count;

    return profile;
}

std::shared_ptr<Profile> Profile::make_forkjoin_composition(
    const std::shared_ptr<std::vector<std::string> > & sub_profiles)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_ForkJoinCompositionProfile;
    profile->_sub_profiles = sub_profiles;

    return profile;
}

std::shared_ptr<Profile> Profile::make_parallel_task_merge_composition(
    const std::shared_ptr<std::vector<std::string> > & sub_profiles)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::ProfileUnion_ParallelTaskMergeCompositionProfile;
    profile->_sub_profiles = sub_profiles;

    return profile;
}

} // end of namespace batprotocol
