#include "profile.hpp"

#include "assert.hpp"

namespace batprotocol
{

std::shared_ptr<Profile> Profile::make_delay(double delay)
{
    BAT_ENFORCE(delay >= 0.0, "invalid delay (non-positive) received: %g", delay);

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_DelayProfile;
    profile->_delay = delay;

    return profile;
}

std::shared_ptr<Profile> Profile::make_parallel_task(
    const std::shared_ptr<std::vector<double> > & computation_vector,
    const std::shared_ptr<std::vector<double> > & communication_matrix)
{
    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_ParallelTaskProfile;
    profile->_computation_vector = computation_vector;
    profile->_communication_matrix = communication_matrix;

    return profile;
}

std::shared_ptr<Profile> Profile::make_parallel_task_homogeneous(
    fb::HomogeneousParallelTaskGenerationStrategy generation_strategy,
    double computation_amount,
    double communication_amount)
{
    BAT_ENFORCE(generation_strategy >= fb::HomogeneousParallelTaskGenerationStrategy_MIN &&
                generation_strategy <= fb::HomogeneousParallelTaskGenerationStrategy_MAX,
                "invalid (unknown) generation_strategy received: %d", generation_strategy);
    BAT_ENFORCE(computation_amount >= 0.0, "invalid (non-positive) computation_amount received: %g", computation_amount);
    BAT_ENFORCE(communication_amount >= 0.0, "invalid (non-positive) communication_amount received: %g", communication_amount);

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_ParallelTaskHomogeneousProfile;
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
    BAT_ENFORCE(bytes_to_transfer >= 0.0, "invalid (non-positive) bytes_to_transfer received: %g", bytes_to_transfer);
    BAT_ENFORCE(!emitter_storage_name.empty(), "invalid (empty) emitter_storage_name received");
    BAT_ENFORCE(!receiver_storage_name.empty(), "invalid (empty) emitter_storage_name received");

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_ParallelTaskDataStagingBetweenStoragesProfile;
    profile->_bytes_to_transfer = bytes_to_transfer;
    profile->_emitter_storage_name = emitter_storage_name;
    profile->_receiver_storage_name = receiver_storage_name;

    return profile;
}

std::shared_ptr<Profile> Profile::make_parallel_task_on_storage_homogeneous(
    const std::string & storage_name,
    fb::HomogeneousParallelTaskGenerationStrategy generation_strategy,
    double bytes_to_read,
    double bytes_to_write)
{
    BAT_ENFORCE(!storage_name.empty(), "invalid (empty) storage_name received");
    BAT_ENFORCE(generation_strategy >= fb::HomogeneousParallelTaskGenerationStrategy_MIN &&
                generation_strategy <= fb::HomogeneousParallelTaskGenerationStrategy_MAX,
                "invalid (unknown) generation_strategy received: %d", generation_strategy);
    BAT_ENFORCE(bytes_to_read >= 0.0, "invalid (non-positive) bytes_to_read received: %g", bytes_to_read);
    BAT_ENFORCE(bytes_to_write >= 0.0, "invalid (non-positive) bytes_to_write received: %g", bytes_to_write);

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_ParallelTaskOnStorageHomogeneousProfile;
    profile->_storage_name = storage_name;
    profile->_bytes_to_read = bytes_to_read;
    profile->_bytes_to_write = bytes_to_write;

    return profile;
}

std::shared_ptr<Profile> Profile::make_trace_replay_smpi(const std::string & filename)
{
    BAT_ENFORCE(!filename.empty(), "invalid (empty) filename received");

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_TraceReplayProfile;
    profile->_trace_type = fb::TraceType_SMPI;
    profile->_filename = filename;

    return profile;
}

std::shared_ptr<Profile> Profile::make_trace_replay_fractional_computation(
    const std::string & filename)
{
    BAT_ENFORCE(!filename.empty(), "invalid (empty) filename received");

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_TraceReplayProfile;
    profile->_trace_type = fb::TraceType_FractionalComputation;
    profile->_filename = filename;

    return profile;
}

std::shared_ptr<Profile> Profile::make_sequential_composition(
    const std::shared_ptr<std::vector<std::string> > & sub_profiles,
    uint32_t repetition_count)
{
    BAT_ENFORCE(sub_profiles.get() != nullptr, "invalid (null) sub_profiles received");

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_SequentialCompositionProfile;
    profile->_sub_profiles = sub_profiles;
    profile->_repetition_count = repetition_count;

    return profile;
}

std::shared_ptr<Profile> Profile::make_forkjoin_composition(
    const std::shared_ptr<std::vector<std::string> > & sub_profiles)
{
    BAT_ENFORCE(sub_profiles.get() != nullptr, "invalid (null) sub_profiles received");

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_ForkJoinCompositionProfile;
    profile->_sub_profiles = sub_profiles;

    return profile;
}

std::shared_ptr<Profile> Profile::make_parallel_task_merge_composition(
    const std::shared_ptr<std::vector<std::string> > & sub_profiles)
{
    BAT_ENFORCE(sub_profiles.get() != nullptr, "invalid (null) sub_profiles received");

    std::shared_ptr<Profile> profile(new Profile());

    profile->_profile_type = fb::Profile_ParallelTaskMergeCompositionProfile;
    profile->_sub_profiles = sub_profiles;

    return profile;
}

} // end of namespace batprotocol
