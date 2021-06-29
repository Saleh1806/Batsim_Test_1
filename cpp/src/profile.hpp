#pragma once

namespace batprotocol
{
    class Profile
    {
    public:
        std::shared_ptr<Profile> make_delay(
            double delay
        );

        std::shared_ptr<Profile> make_parallel_task(
            const std::vector<double> & computation_vector = std::vector<double>(),
            const std::vector<double> & communication_matrix = std::vector<double>()
        );

        std::shared_ptr<Profile> make_parallel_task_homogeneous(
            HomogeneousParallelTaskGenerationStrategy generation_strategy,
            double computation_amount = 0,
            double communication_amount = 0
        );

        std::shared_ptr<Profile> make_parallel_task_data_staging_between_storages(
            double bytes_to_transfer,
            const std::string & emitter_storage_name,
            const std::string & receiver_storage_name
        );

        std::shared_ptr<Profile> make_trace_replay_smpi(
            const std::string & filename
        );

        std::shared_ptr<Profile> make_trace_replay_fractional_computation(
            const std::string & filename
        );

        std::shared_ptr<Profile> make_sequential_composition(
            const std::vector<std::string> & sub_profiles,
            uint32_t repetition_count = 1
        );

        std::shared_ptr<Profile> make_forkjoin_composition(
            const std::vector<std::string> & sub_profiles
        );

        std::shared_ptr<Profile> make_parallel_task_merge_composition(
            const std::vector<std::string> & sub_profiles
        );

    private:
        Profile() = default;
        TODO
    };
} // end of namespace batprotocol
