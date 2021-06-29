#pragma once

#include <memory>
#include <unordered_map>

#include "execute_job.hpp"
#include "job.hpp"
#include "profile.hpp"
//#include "batprotocol_generated.h"

namespace batprotocol
{
    class TimeSpecifier
    {
        static TimeSpecifier make();
        TimeSpecifier & set_one_time(double time_point);
        TimeSpecifier & set_periodic(double start_time_point, double period);

    private:
        TimeSpecifier() = default;
        TODO
    };


    class SimulationBegins
    {
        static SimulationBegins make();

        SimulationBegins & add_host(uint32_t id, const std::string & name, uint32_t pstate, uint32_t pstate_count, HostState state, uint32_t core_count, const std::vector<double> computation_speed);
        SimulationBegins & set_host_property(uint32_t id, const std::string & key, const std::string & value);
        SimulationBegins & set_host_as_storage(uint32_t id);

        SimulationBegins & add_workload(const std::string & workload_id, const std::string & filename);

        SimulationBegins & add_profile(const std::string & profile_id, const std::shared_ptr<Profile> & profile);

        SimulationBegins & set_batsim_execution_context(const std::string & json);
    private:
        SimulationBegins() = default;
        TODO
    };


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
            const Job & job
        );

        void add_job_completed(
            const std::string & job_id,
            batprotocol::FinalJobState state,
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

        void add_register_job_event(
            const std::string & job_id,
            const Job & job
        );

        // Resource management events

        // Simulation management events
        void add_call_me_later_event(
            const std::string & call_me_later_id,
            const TimeSpecifier & when
        );

        void add_requested_call_event(
            const std::string & call_me_later_id
        );

        void add_stop_call_me_later_event(
            const std::string & call_me_later_id
        );

        void add_batsim_hello_event(
            const std::string & batsim_version,
            const std::string & batsim_commit = ""
        );

        void add_external_decision_component_hello_event(
            const std::string & decision_component_name,
            const std::string & decision_component_version,
            const std::string & decision_component_commit = ""
        );

        void add_simulation_begins_event(
            SimulationBegins & simulation_begins
        );

        void add_simulation_ends_event();
        void add_all_static_jobs_have_been_submitted_event();
        void add_all_static_external_events_have_been_injected_event();
        void add_finish_registration();

    private:
        flatbuffers::FlatBufferBuilder * _builder = nullptr;
        bool _is_buffer_finished = false;
        TODO
    };
} // end of namespace batprotocol
