namespace batprotocol
{
    class ExecuteJobOptions
    {
    public:
        static ExecuteJobOptions make();

        ExecuteJobOptions & set_predefined_placement_strategy(ExecutorPlacementStrategy strategy);
        ExecuteJobOptions & set_custom_placement(const std::vector<uint32_t> & mapping);

        ExecuteJobOptions & override_profile_alloc_predefined_placement(const std::string & profile_id, const std::string & host_allocation, ExecutorPlacementStrategy strategy);
        ExecuteJobOptions & override_profile_alloc_custom_placement(const std::string & profile_id, const std::string & host_allocation, const std::vector<uint32_t> & mapping);

        ExecuteJobOptions & override_storage_placement(const std::string & storage_name, uint32_t host_id);
    private:
        ExecuteJobOptions() = default;
    };


    class KillProgress
    {
    public:
        static KillProgress make();
        KillProgress & add_atomic(const std::string & task_id, const std::string & profile_id, double progress);
        KillProgress & add_sequential(const std::string & task_id, const std::string & profile_id, uint32_t current_repetition, uint32_t current_task_index, const std::string & current_subtask_id);
        KillProgress & add_forkjoin(const std::string & task_id, const std::string & profile_id, const std::vector<std::string> & subtasks_id);
        KillProgress & set_root_task(const std::string & task_id);

    private:
        KillProgress() = default;
    };


    class Profile
    {
    public:
        static Profile make();

        Profile & set_delay(
            double delay
        );

        Profile & set_parallel_task(
            const std::vector<double> & computation_vector = std::vector<double>(),
            const std::vector<double> & communication_matrix = std::vector<double>()
        );

        Profile & set_parallel_task_homogeneous(
            HomogeneousParallelTaskGenerationStrategy generation_strategy,
            double computation_amount = 0,
            double communication_amount = 0
        );

        Profile & set_parallel_task_data_staging_between_storages(
            double bytes_to_transfer,
            const std::string & emitter_storage_name,
            const std::string & receiver_storage_name
        );

        Profile & set_trace_replay_smpi(
            const std::string & filename
        );

        Profile & set_trace_replay_fractional_computation(
            const std::string & filename
        );

        Profile & set_sequential_composition(
            const std::vector<std::string> & sub_profiles,
            uint32_t repetition_count = 1
        );

        Profile & set_forkjoin_composition(
            const std::vector<std::string> & sub_profiles
        );

        Profile & set_parallel_task_merge_composition(
            const std::vector<std::string> & sub_profiles
        );

    private:
        Profile() = default;
    };


    class Job
    {
    public:
        static Job make();

        Job & set_host_number(uint32_t host_number);
        Job & set_core_number(uint32_t core_number);

        Job & set_walltime(double walltime);
        Job & set_extra_data(const std::string extra_data);

        Job & set_profile(const std::string & profile_id);
        Job & set_non_rigid();
    private:
        Job() = default;
    };


    class TimeSpecifier
    {
        static TimeSpecifier make();
        TimeSpecifier & set_one_time(double time_point);
        TimeSpecifier & set_periodic(double start_time_point, double period);

    private:
        TimeSpecifier() = default;
    };


    class SimulationBegins
    {
        static SimulationBegins make();

        SimulationBegins & add_host(uint32_t id, const std::string & name, uint32_t pstate, uint32_t pstate_count, HostState state, uint32_t core_count, const std::vector<double> computation_speed);
        SimulationBegins & set_host_property(uint32_t id, const std::string & key, const std::string & value);
        SimulationBegins & set_host_as_storage(uint32_t id);

        SimulationBegins & add_workload(const std::string & workload_id, const std::string & filename);

        SimulationBegins & add_profile(const std::string & profile_id, const Profile & profile);

        SimulationBegins & set_batsim_execution_context(const std::string & json);
    private:
        SimulationBegins() = default;
    };


    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();

        // Queue manipulation and serialization API
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
            const ExecuteJobOptions & options = ExecuteJobOptions::make()
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
            const Profile & profile
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
    };
}
