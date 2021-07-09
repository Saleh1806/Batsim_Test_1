#pragma once

#include <memory>
#include <vector>

#include "execute_job.hpp"
#include "job.hpp"
#include "kill.hpp"
#include "profile.hpp"
#include "simulation_begins.hpp"
#include "temporal_trigger.hpp"

namespace batprotocol
{
    class MessageBuilder
    {
    public:
        MessageBuilder();
        ~MessageBuilder();

        // Message serialization API
        void clear(double now);
        void finish_message(double now);
        void set_current_time(double now);
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
            const std::vector<KillProgress> & progresses
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
            const std::shared_ptr<TemporalTrigger> & when
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
        std::vector<flatbuffers::Offset<flatbuffers::String> > serialize_string_vector(const std::vector<std::string> & strings);
        std::vector<flatbuffers::Offset<batprotocol::fb::Host>> serialize_host_vector(const SimulationBegins & simulation_begins, const std::vector<uint32_t> host_ids);
        std::vector<flatbuffers::Offset<batprotocol::fb::HostProperty>> serialize_host_property_vector(const std::unordered_map<std::string, std::string> & properties);
        std::vector<flatbuffers::Offset<batprotocol::fb::ProfileAndId>> serialize_profile_and_id_vector(std::unordered_map<std::string, std::shared_ptr<Profile> > & profiles);
        std::vector<flatbuffers::Offset<batprotocol::fb::WorkloadAndFilename>> serialize_workload_and_filename_vector(std::unordered_map<std::string, std::string> & workloads);
        std::vector<flatbuffers::Offset<batprotocol::fb::KillProgressWrapper>> serialize_kill_progress_vector(const std::vector<std::string> & tasks_to_serialize, const std::string & task_id, const std::string & job_id, std::unordered_map<std::string, flatbuffers::Offset<batprotocol::fb::KillProgressWrapper> > & serialized_tasks);

        flatbuffers::Offset<fb::Job> serialize_job(const std::shared_ptr<Job> & job);
        flatbuffers::Offset<fb::ProfileAndId> serialize_profile_and_id(const std::string & profile_id, const std::shared_ptr<Profile> & profile);
        flatbuffers::Offset<void> serialize_temporal_trigger(const std::shared_ptr<TemporalTrigger> & temporal_trigger);
        flatbuffers::Offset<void> serialize_placement(ExecuteJobOptions::Placement * placement);
        flatbuffers::Offset<batprotocol::fb::KillProgressWrapper> serialize_kill_progress(const std::string & task_id, const std::string & job_id, const KillProgress::KillProgressVariant * variant, std::unordered_map<std::string, flatbuffers::Offset<batprotocol::fb::KillProgressWrapper> > & serialized_tasks);

    private:
        flatbuffers::FlatBufferBuilder * _builder = nullptr;
        bool _is_buffer_finished = false;
        double _current_time = 0.0;
        std::vector<flatbuffers::Offset<fb::EventAndTimestamp>> _events;
    };
} // end of namespace batprotocol
