#pragma once

#include <memory>
#include <unordered_map>

#include "execute_job.hpp"
#include "job.hpp"
#include "kill.hpp"
#include "profile.hpp"
#include "simulation_begins.hpp"
#include "time_specifier.hpp"

namespace batprotocol
{
    class MessageBuilder
    {
    public:
        MessageBuilder();
        ~MessageBuilder();

        // Message serialization API
        void clear();
        void generate_message();
        const uint8_t * const buffer_pointer() const;
        const uint32_t buffer_size() const;

        // Job management events
        void add_job_submitted(
            const std::string & job_id,
            const std::shared_ptr<Job> & job
        );

        void add_job_completed(
            const std::string & job_id,
            fb::FinalJobState state,
            uint32_t return_code = 0
        );

        void add_reject_job(
            const std::string & job_id
        );

        void add_execute_job(
            const std::string & job_id,
            const std::string & host_allocation,
            const ExecuteJobOptions & options = ExecuteJobOptions()
        );

        void add_kill_jobs(
            const std::vector<std::string> & job_ids
        );

        void add_jobs_killed(
            const std::vector<std::string> & job_ids,
            const KillProgress & progress
        );

        void add_register_profile(
            const std::string & profile_id,
            const std::shared_ptr<Profile> & profile
        );

        void add_register_job(
            const std::string & job_id,
            const std::shared_ptr<Job> & job
        );

        // Resource management events

        // Simulation management events
        void add_call_me_later(
            const std::string & call_me_later_id,
            const std::shared_ptr<TimeSpecifier> & when
        );

        void add_requested_call(
            const std::string & call_me_later_id
        );

        void add_stop_call_me_later(
            const std::string & call_me_later_id
        );

        void add_batsim_hello(
            const std::string & batsim_version,
            const std::string & batsim_commit = ""
        );

        void add_external_decision_component_hello(
            const std::string & decision_component_name,
            const std::string & decision_component_version,
            const std::string & decision_component_commit = ""
        );

        void add_simulation_begins(
            SimulationBegins & simulation_begins
        );

        void add_simulation_ends();
        void add_all_static_jobs_have_been_submitted();
        void add_all_static_external_events_have_been_injected();
        void add_finish_registration();

    private:
        flatbuffers::FlatBufferBuilder * _builder = nullptr;
        bool _is_buffer_finished = false;
        // TODO
    };
} // end of namespace batprotocol
