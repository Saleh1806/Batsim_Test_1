#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;

    class Profile
    {
    public:
        static std::shared_ptr<Profile> make_delay(
            double delay
        );

        static std::shared_ptr<Profile> make_parallel_task(
            const std::shared_ptr<std::vector<double> > & computation_vector = nullptr,
            const std::shared_ptr<std::vector<double> > & communication_matrix = nullptr
        );

        static std::shared_ptr<Profile> make_parallel_task_homogeneous(
            fb::HomogeneousParallelTaskGenerationStrategy generation_strategy,
            double computation_amount = 0,
            double communication_amount = 0
        );

        static std::shared_ptr<Profile> make_parallel_task_data_staging_between_storages(
            double bytes_to_transfer,
            const std::string & emitter_storage_name,
            const std::string & receiver_storage_name
        );

        static std::shared_ptr<Profile> make_parallel_task_on_storage_homogeneous(
            const std::string & storage_name,
            fb::HomogeneousParallelTaskGenerationStrategy generation_strategy,
            double bytes_to_read = 0,
            double bytes_to_write = 0
        );

        static std::shared_ptr<Profile> make_trace_replay_smpi(
            const std::string & filename
        );

        static std::shared_ptr<Profile> make_trace_replay_fractional_computation(
            const std::string & filename
        );

        static std::shared_ptr<Profile> make_sequential_composition(
            const std::shared_ptr<std::vector<std::string> > & sub_profiles,
            uint32_t repetition_count = 1
        );

        static std::shared_ptr<Profile> make_forkjoin_composition(
            const std::shared_ptr<std::vector<std::string> > & sub_profiles
        );

        static std::shared_ptr<Profile> make_parallel_task_merge_composition(
            const std::shared_ptr<std::vector<std::string> > & sub_profiles
        );

    private:
        friend class MessageBuilder;
        Profile() = default;
        fb::Profile _profile_type = fb::Profile_NONE;
        std::shared_ptr<std::vector<std::string> > _sub_profiles = nullptr;
        uint32_t _repetition_count = 1;

        double _delay = 0;

        std::shared_ptr<std::vector<double> > _computation_vector = nullptr;
        std::shared_ptr<std::vector<double> > _communication_matrix = nullptr;

        fb::HomogeneousParallelTaskGenerationStrategy _generation_strategy;
        double _computation_amount = 0;
        double _communication_amount = 0;

        double _bytes_to_transfer = 0;
        std::string _emitter_storage_name;
        std::string _receiver_storage_name;

        std::string _storage_name;
        double _bytes_to_read = 0;
        double _bytes_to_write = 0;

        fb::TraceType _trace_type;
        std::string _filename;
    };
} // end of namespace batprotocol
