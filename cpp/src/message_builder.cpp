#include "message_builder.hpp"

#include <unordered_set>
#include <stack>

#include "assert.hpp"
#include "schema.hpp"
#include "version.hpp"

namespace batprotocol
{

MessageBuilder::MessageBuilder(bool enable_json)
{
    _is_json_enabled = enable_json;
    _builder = new flatbuffers::FlatBufferBuilder();

    if (enable_json)
    {
        flatbuffers::IDLOptions parser_options;
        parser_options.strict_json = true;
        parser_options.output_default_scalars_in_json = true;
        parser_options.indent_step = -1; // < 0 means no spaces nor line breaks
        _parser = new flatbuffers::Parser(parser_options);

        bool ret = _parser->Parse(batprotocol::fb::schema.c_str());
        BAT_ENFORCE(ret, "Could not parse batprotocol's flatbuffers schema");
    }
}

MessageBuilder::~MessageBuilder()
{
    delete _builder;
    _builder = nullptr;

    delete _parser;
    _parser = nullptr;
}


void MessageBuilder::clear(double now)
{
    _current_time = now;
    _events.clear();
    _builder->Clear();
    _json_buffer.clear();
    _is_buffer_finished = false;
}

void MessageBuilder::finish_message(double now)
{
    BAT_ENFORCE(now >= _current_time, "New time (%g) is before current time (%g), which is forbidden. The time at which your message is sent cannot be before the last event you added. Also make sure that you called clear() with a correct `now` value.", now, _current_time);

    auto msg = CreateMessageDirect(*_builder, now, &_events);
    _builder->Finish(msg);
    _is_buffer_finished = true;
}

void MessageBuilder::set_current_time(double now)
{
    BAT_ENFORCE(now >= _current_time, "New time (%g) is before current time (%g), which is forbidden. Please add your events in chronological order. Also make sure that you called clear() with a correct `now` value.", now, _current_time);
    _current_time = now;
}

const uint8_t * MessageBuilder::buffer_pointer() const
{
    BAT_ENFORCE(_is_buffer_finished, "Cannot call buffer_pointer() while buffer is not finished. Please call finish_message() first.");
    return _builder->GetBufferPointer();
}

uint32_t MessageBuilder::buffer_size() const
{
    BAT_ENFORCE(_is_buffer_finished, "Cannot call buffer_size() while buffer is not finished. Please call finish_message() first.");
    return static_cast<uint32_t>(_builder->GetSize());
}

int MessageBuilder::nb_events() const
{
    return _events.size();
}

bool MessageBuilder::has_events() const
{
    return !_events.empty();
}

const std::string * MessageBuilder::buffer_as_json()
{
    BAT_ENFORCE(_is_json_enabled, "Cannot call buffer_as_json() while json is disabled. Please enable json when constructing your MessageBuilder.");
    BAT_ENFORCE(_is_buffer_finished, "Cannot call buffer_as_json() while buffer is not finished. Please call finish_message() first.");
    GenerateText(*_parser, _builder->GetBufferPointer(), &_json_buffer);
    return &_json_buffer;
}

void MessageBuilder::parse_json_message(
    const std::string & json_msg,
    uint8_t *& buffer_pointer)
{
    parse_json_message(json_msg.c_str(), buffer_pointer);
}

void MessageBuilder::parse_json_message(
    const char * json_msg,
    uint8_t *& buffer_pointer)
{
    BAT_ENFORCE(_is_json_enabled, "Cannot call parse_json_message() while json is disabled. Please enable json when constructing your MessageBuilder.");

    bool ret = _parser->Parse(json_msg);
    BAT_ENFORCE(ret, "Could not parse the provided json message with batprotocol's flatbuffers schema: %s", _parser->error_.c_str());
    buffer_pointer = _parser->builder_.GetBufferPointer();
}

void MessageBuilder::add_job_submitted(
    const std::string & job_id,
    const std::shared_ptr<Job> & job,
    double submission_time)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_job_submitted() while buffer is finished. Please call clear() first.");

    auto job_id_s = _builder->CreateString(job_id);
    auto job_s = serialize_job(job);

    auto job_submitted = fb::CreateJobSubmittedEvent(*_builder, job_id_s, job_s, submission_time);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_JobSubmittedEvent, job_submitted.Union());
    _events.push_back(event);
}

void MessageBuilder::add_job_completed(
    const std::string & job_id,
    fb::FinalJobState state,
    uint32_t return_code)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_job_completed() while buffer is finished. Please call clear() first.");

    auto job_id_s = _builder->CreateString(job_id);

    auto job_completed = fb::CreateJobCompletedEvent(*_builder, job_id_s, state, return_code);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_JobCompletedEvent, job_completed.Union());
    _events.push_back(event);
}

void MessageBuilder::add_reject_job(const std::string & job_id)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_reject_job() while buffer is finished. Please call clear() first.");

    auto job_id_s = _builder->CreateString(job_id);

    auto reject_job = fb::CreateRejectJobEvent(*_builder, job_id_s);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_RejectJobEvent, reject_job.Union());
    _events.push_back(event);
}

void MessageBuilder::add_execute_job(
    const std::string & job_id,
    const std::string & host_allocation,
    const ExecuteJobOptions & options)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_execute_job() while buffer is finished. Please call clear() first.");

    // (default) general placement
    auto placement_type = fb::ExecutorPlacement_PredefinedExecutorPlacementStrategyWrapper;
    if (options._placement != nullptr)
        placement_type = options._placement->_type;
    auto placement_s = serialize_placement(options._placement);

    auto alloc_placement_s = fb::CreateAllocationPlacementDirect(*_builder, host_allocation.c_str(), placement_type, placement_s);

    // placement override for specific profiles
    std::vector<std::string> profile_ids_to_override;
    std::vector<flatbuffers::Offset<fb::ProfileAllocationPlacement>> profile_overrides_s;
    profile_ids_to_override.reserve(options._profile_overrides.size());
    profile_overrides_s.reserve(options._profile_overrides.size());

    for (const auto & kv : options._profile_overrides)
    {
        const auto & profile_id = kv.first;
        profile_ids_to_override.push_back(profile_id);
    }
    std::sort(profile_ids_to_override.begin(), profile_ids_to_override.end());

    for (const auto & profile_id : profile_ids_to_override)
    {
        const auto * profile_placement = options._profile_overrides.at(profile_id);
        BAT_ASSERT(profile_placement != nullptr, "Internal inconsistency: null profile placement");
        BAT_ASSERT(profile_placement->placement != nullptr, "Internal inconsistency: null placement for a profile");

        auto placement_s = serialize_placement(profile_placement->placement);
        auto profile_placement_s = fb::CreateProfileAllocationPlacementDirect(*_builder, profile_id.c_str(), profile_placement->host_allocation.c_str(), profile_placement->placement->_type, placement_s);
        profile_overrides_s.push_back(profile_placement_s);
    }

    // placement override for storages
    std::vector<std::string> storage_names_to_override;
    std::vector<flatbuffers::Offset<batprotocol::fb::StorageHost>> storage_overrides_s;
    storage_names_to_override.reserve(options._storage_overrides.size());
    storage_overrides_s.reserve(options._storage_overrides.size());

    for (const auto & kv : options._storage_overrides)
    {
        const auto & storage_name = kv.first;
        storage_names_to_override.push_back(storage_name);
    }
    std::sort(storage_names_to_override.begin(), storage_names_to_override.end());

    for (const auto & storage_name : storage_names_to_override)
    {
        const auto & host_id = options._storage_overrides.at(storage_name);

        auto storage_placement_s = fb::CreateStorageHostDirect(*_builder, storage_name.c_str(), host_id);
        storage_overrides_s.push_back(storage_placement_s);
    }

    auto execute_job = fb::CreateExecuteJobEventDirect(*_builder, job_id.c_str(), alloc_placement_s, &profile_overrides_s, &storage_overrides_s);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_ExecuteJobEvent, execute_job.Union());
    _events.push_back(event);
}

void MessageBuilder::add_kill_jobs(const std::vector<std::string> & job_ids)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_kill_jobs() while buffer is finished. Please call clear() first.");

    auto job_ids_s = serialize_string_vector(job_ids);

    auto kill_jobs = fb::CreateKillJobsEventDirect(*_builder, &job_ids_s);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_KillJobsEvent, kill_jobs.Union());
    _events.push_back(event);
}

void MessageBuilder::add_jobs_killed(
    const std::vector<std::string> & job_ids,
    const std::map<std::string, std::shared_ptr<KillProgress> > & progresses)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_jobs_killed() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!job_ids.empty(), "Invalid (empty) job_ids received.");
    BAT_ENFORCE(job_ids.size() >= progresses.size(), "size of job_ids and progresses is invalid (job_ids=%lu, progresses=%lu)", job_ids.size(), progresses.size());

    auto job_ids_s = serialize_string_vector(job_ids);

    std::vector<flatbuffers::Offset<batprotocol::fb::JobAndProgress>> progress_vector;
    progress_vector.reserve(progresses.size());

    // fun part is here.
    // a tree must be created for every job killed (their progress is independent).
    // the following code traverse the task tree of each job with unwinding to serialize all tasks in depth-first order.
    for (auto it : progresses)
    {
        const std::string & job_id = it.first;
        BAT_ENFORCE(it.second.get() != nullptr, "Invalid (null) progress received for job '%s'", job_id.c_str());
        const auto & progress = *it.second;

        std::unordered_map<std::string, flatbuffers::Offset<batprotocol::fb::KillProgressWrapper> > serialized_tasks;

        // task_ids already known are stored so that cycles (which are forbidden) can be detected.
        // this is also used to serialize composed tasks while unwinding the stack.
        std::unordered_set<std::string> visited_task_ids;

        // serialization is done by traversing the progresses in depth-first order.
        std::stack<std::string> tasks_to_explore;
        tasks_to_explore.push(progress._root_task_id);

        while (!tasks_to_explore.empty())
        {
            auto task_id = tasks_to_explore.top();
            auto task_it = progress._tasks_progress.find(task_id);
            BAT_ENFORCE(task_it != progress._tasks_progress.end(), "invalid KillProgress for job_id='%s': progress of task_id='%s' has not been set", job_id.c_str(), task_id.c_str());
            const KillProgress::KillProgressVariant * variant = task_it->second;

            BAT_ASSERT(variant->type != fb::KillProgress_NONE, "inconsistency KillProgress for task_id='%s' (job_id='%s'): creating untyped progresses should not be possible", task_id.c_str(), job_id.c_str());
            BAT_ASSERT(variant->type >= fb::KillProgress_MIN && variant->type <= fb::KillProgress_MAX, "invalid KillProgress for task_id='%s' (job_id='%s'): kill progress type is invalid (%d)", task_id.c_str(), job_id.c_str(), variant->type);

            if (variant->type == fb::KillProgress_KillProgressAtomicProfile)
            {
                // current task is a leaf (atomic progress). serialize it right away.
                BAT_ENFORCE(visited_task_ids.find(task_id) == visited_task_ids.end(), "invalid KillProgress for job_id='%s': task_id='%s' is used several times while the graph is expected to contain no cycle.", job_id.c_str(), task_id.c_str());

                tasks_to_explore.pop();
                serialize_kill_progress(task_id, job_id, variant, serialized_tasks);
            }
            else
            {
                // current task is composed.
                if (visited_task_ids.find(task_id) == visited_task_ids.end())
                {
                    // this is the first time this composed task is encountered.
                    visited_task_ids.insert(task_id);

                    // do not remove the task from the stack. it will be serialized when all its children have been serialized (stack unwinding).
                    BAT_ASSERT(variant->data != nullptr, "inconsistent progress data (null) of task_id='%s' (job_id='%s')", task_id.c_str(), job_id.c_str());

                    switch(variant->type)
                    {
                    case fb::KillProgress_NONE: // not breaking is not a mistake here.
                    case fb::KillProgress_KillProgressAtomicProfile: {
                        BAT_ASSERT(false, "inconsistency: kill progress type should not be %s", fb::EnumNamesKillProgress()[variant->type]);
                    } break;
                    case fb::KillProgress_KillProgressSequentialProfile: {
                        const auto * kp = static_cast<KillProgress::Sequential*>(variant->data);
                        tasks_to_explore.push(kp->current_subtask_id);
                    } break;
                    case fb::KillProgress_KillProgressForkJoinProfile: {
                        const auto * kp = static_cast<KillProgress::ForkJoin*>(variant->data);
                        for (const auto & child_task_id : kp->subtasks_id)
                            tasks_to_explore.push(child_task_id);
                    } break;
                    }
                }
                else
                {
                    // this composed task has already been encountered.
                    // this should be because we are unwinding the stack.
                    // if not (invalid graph defined, with cycles) the serialization will fail because all children have not been serialized.
                    serialize_kill_progress(task_id, job_id, variant, serialized_tasks);
                    tasks_to_explore.pop();
                }
            }
        }

        // all tasks should have been serialized
        auto job_and_progress_s = fb::CreateJobAndProgressDirect(*_builder, job_id.c_str(), serialized_tasks[progress._root_task_id]);
        progress_vector.push_back(job_and_progress_s);
    }

    auto jobs_killed = fb::CreateJobsKilledEventDirect(*_builder, &job_ids_s, &progress_vector);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_JobsKilledEvent, jobs_killed.Union());
    _events.push_back(event);
}

void MessageBuilder::add_register_profile(
    const std::string & profile_id,
    const std::shared_ptr<Profile> & profile)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_register_profile() while buffer is finished. Please call clear() first.");

    auto profile_and_id = serialize_profile_and_id(profile_id, profile);
    auto register_profile = fb::CreateRegisterProfileEvent(*_builder, profile_and_id);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_RegisterProfileEvent, register_profile.Union());
    _events.push_back(event);
}

void MessageBuilder::add_register_job(
    const std::string & job_id,
    const std::shared_ptr<Job> & job)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_register_job() while buffer is finished. Please call clear() first.");

    auto job_id_s = _builder->CreateString(job_id);
    auto job_s = serialize_job(job);

    auto register_job = fb::CreateRegisterJobEvent(*_builder, job_id_s, job_s);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_RegisterJobEvent, register_job.Union());
    _events.push_back(event);
}

void MessageBuilder::add_create_probe(
    const std::string & probe_id,
    fb::Metrics metrics,
    const std::shared_ptr<CreateProbe> & create_probe)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_create_probe() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!probe_id.empty(), "Invalid (empty) probe_id received");

    auto resources_s = serialize_resources(
        create_probe->_resources_type,
        create_probe->_hosts_resources,
        create_probe->_links_resources
    );

    flatbuffers::Offset<void> data_accumulation_strategy_s = 0;
    switch(create_probe->_data_accumulation_strategy)
    {
    case fb::ProbeDataAccumulationStrategy_NONE: {
        BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped probe accumulation strategy");
    } break;
    case fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation: {
        data_accumulation_strategy_s = fb::CreateNoProbeDataAccumulation(*_builder).Union();
    } break;
    case fb::ProbeDataAccumulationStrategy_ProbeDataAccumulation: {
        flatbuffers::Offset<void> reset_mode = 0;
        switch(create_probe->_data_accumulation_reset_mode) {
            case fb::ResetMode_NONE: {
                BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped reset mode");
            } break;
            case fb::ResetMode_NoReset: {
                reset_mode = fb::CreateNoReset(*_builder).Union();
            } break;
            case fb::ResetMode_ProbeAccumulationReset: {
                reset_mode = fb::CreateProbeAccumulationReset(*_builder, create_probe->_data_accumulation_reset_value).Union();
            } break;
        }

        data_accumulation_strategy_s = fb::CreateProbeDataAccumulation(*_builder,
            create_probe->_data_accumulation_reset_mode,
            reset_mode,
            create_probe->_data_accumulation_cumulative_function,
            create_probe->_data_accumulation_temporal_normalization
        ).Union();
    } break;
    }

    flatbuffers::Offset<void> measurement_triggering_policy_s = 0;
    switch(create_probe->_measurement_triggering_policy) {
        case fb::ProbeMeasurementTriggeringPolicy_NONE: {
            BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped measurement triggering policy");
        } break;
        case fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper: {
            auto temporal_trigger_s = serialize_temporal_trigger(create_probe->_temporal_trigger);
            measurement_triggering_policy_s = fb::CreateTemporalTriggerWrapper(*_builder, create_probe->_temporal_trigger->_type, temporal_trigger_s).Union();
        } break;
    }

    flatbuffers::Offset<void> resources_aggregation_function_s = 0;
    switch(create_probe->_resource_aggregation_function) {
        case fb::ResourcesAggregationFunction_NONE: {
            BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped resource aggregation function");
        } break;
        case fb::ResourcesAggregationFunction_NoResourcesAggregation: {
            resources_aggregation_function_s = fb::CreateNoResourcesAggregation(*_builder).Union();
        } break;
        case fb::ResourcesAggregationFunction_Sum: {
            resources_aggregation_function_s = fb::CreateSum(*_builder).Union();
        } break;
        case fb::ResourcesAggregationFunction_ArithmeticMean: {
            resources_aggregation_function_s = fb::CreateArithmeticMean(*_builder).Union();
        } break;
        case fb::ResourcesAggregationFunction_QuantileFunction: {
            resources_aggregation_function_s = fb::CreateQuantileFunction(*_builder, create_probe->_resource_aggregation_quantile_threshold).Union();
        } break;
    }

    flatbuffers::Offset<void> temporal_aggregation_function_s = 0;
    switch(create_probe->_temporal_aggregation_function) {
        case fb::TemporalAggregationFunction_NONE: {
            BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped temporal aggregation function");
        } break;
        case fb::TemporalAggregationFunction_NoTemporalAggregation: {
            temporal_aggregation_function_s = fb::CreateNoTemporalAggregation(*_builder).Union();
        } break;
        case fb::TemporalAggregationFunction_ExponentialMovingAverage: {
            BAT_ENFORCE(false, "NOT IMPLEMENTED");
        } break;
        case fb::TemporalAggregationFunction_SlidingWindowOperation: {
            BAT_ENFORCE(false, "NOT IMPLEMENTED");
        } break;
    }

    flatbuffers::Offset<void> emission_filtering_policy_s = 0;
    switch(create_probe->_emission_filtering_policy) {
        case fb::ProbeEmissionFilteringPolicy_NONE: {
            BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped probe emission filtering policy");
        } break;
        case fb::ProbeEmissionFilteringPolicy_NoFiltering: {
            emission_filtering_policy_s = fb::CreateNoFiltering(*_builder).Union();
        } break;
        case fb::ProbeEmissionFilteringPolicy_ThresholdFilteringFunction: {
            emission_filtering_policy_s = fb::CreateThresholdFilteringFunction(*_builder,
                create_probe->_emission_filtering_threshold,
                create_probe->_emission_filtering_threshold_operator
            ).Union();
        } break;
    }

    auto create_probe_s = fb::CreateCreateProbeEventDirect(*_builder,
        probe_id.c_str(),
        metrics,
        create_probe->_resources_type,
        resources_s,
        create_probe->_data_accumulation_strategy,
        data_accumulation_strategy_s,
        create_probe->_measurement_triggering_policy,
        measurement_triggering_policy_s,
        create_probe->_resource_aggregation_function,
        resources_aggregation_function_s,
        create_probe->_temporal_aggregation_function,
        temporal_aggregation_function_s,
        create_probe->_emission_filtering_policy,
        emission_filtering_policy_s
    );

    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_CreateProbeEvent, create_probe_s.Union());
    _events.push_back(event);
}

void MessageBuilder::add_stop_probe(
    const std::string & probe_id)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_stop_probe() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!probe_id.empty(), "Invalid (empty) probe_id received");

    auto stop_probe = fb::CreateStopProbeEventDirect(*_builder, probe_id.c_str());
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_StopProbeEvent, stop_probe.Union());

    _events.push_back(event);
}

void MessageBuilder::add_reset_probe(
    const std::string & probe_id,
    double reset_value)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_reset_probe() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!probe_id.empty(), "Invalid (empty) probe_id received");

    auto reset_probe = fb::CreateResetProbeEventDirect(*_builder, probe_id.c_str(), reset_value);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_ResetProbeEvent, reset_probe.Union());

    _events.push_back(event);
}

void MessageBuilder::add_trigger_probe(
    const std::string & probe_id,
    bool force_data_emission)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_trigger_probe() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!probe_id.empty(), "Invalid (empty) probe_id received");

    auto trigger_probe = fb::CreateTriggerProbeEventDirect(*_builder, probe_id.c_str(), force_data_emission);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_TriggerProbeEvent, trigger_probe.Union());

    _events.push_back(event);
}

void MessageBuilder::add_probe_data_emitted(
    const std::string & probe_id,
    fb::Metrics metrics,
    const std::shared_ptr<ProbeData> & probe_data,
    bool manually_triggered,
    uint32_t nb_emitted,
    uint32_t nb_triggered)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_probe_data_emitted() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!probe_id.empty(), "Invalid (empty) probe_id received");

    auto resources_s = serialize_resources(
        probe_data->_resources_type,
        probe_data->_hosts_resources,
        probe_data->_links_resources
    );

    flatbuffers::Offset<void> data_s = 0;
    switch(probe_data->_type) {
        case fb::ProbeData_NONE: {
            BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped probe data");
        } break;
        case fb::ProbeData_AggregatedProbeData: {
            data_s = fb::CreateAggregatedProbeData(*_builder, probe_data->_aggregated_data).Union();
        } break;
        case fb::ProbeData_VectorialProbeData: {
            data_s = fb::CreateVectorialProbeDataDirect(*_builder, probe_data->_vectorial_data.get()).Union();
        } break;
    }

    auto probe_data_emitted = CreateProbeDataEmittedEventDirect(*_builder,
        probe_id.c_str(),
        probe_data->_resources_type,
        resources_s,
        metrics,
        probe_data->_type,
        data_s,
        manually_triggered,
        nb_emitted,
        nb_triggered
    );
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_ProbeDataEmittedEvent, probe_data_emitted.Union());

    _events.push_back(event);
}

void MessageBuilder::add_call_me_later(
    const std::string & call_me_later_id,
    const std::shared_ptr<TemporalTrigger> & when)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_call_me_later() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!call_me_later_id.empty(), "Invalid (empty) call_me_later_id received");
    BAT_ENFORCE(when.get() != nullptr, "Invalid (null) TemporalTrigger received");

    auto ts_s = serialize_temporal_trigger(when);

    auto call_me_later = fb::CreateCallMeLaterEventDirect(*_builder, call_me_later_id.c_str(), when->_type, ts_s);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_CallMeLaterEvent, call_me_later.Union());
    _events.push_back(event);
}

void MessageBuilder::add_requested_call(const std::string & call_me_later_id, bool last_periodic_call)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_requested_call() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!call_me_later_id.empty(), "Invalid (empty) call_me_later_id received");

    auto call_me_later_id_s = _builder->CreateString(call_me_later_id);

    auto requested_call = fb::CreateRequestedCallEvent(*_builder, call_me_later_id_s, last_periodic_call);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_RequestedCallEvent, requested_call.Union());
    _events.push_back(event);
}

void MessageBuilder::add_stop_call_me_later(const std::string & call_me_later_id)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_stop_call_me_later() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!call_me_later_id.empty(), "Invalid (empty) call_me_later_id received");

    auto call_me_later_id_s = _builder->CreateString(call_me_later_id);

    auto stop_call_me_later = fb::CreateStopCallMeLaterEvent(*_builder, call_me_later_id_s);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_StopCallMeLaterEvent, stop_call_me_later.Union());
    _events.push_back(event);
}

void MessageBuilder::add_batsim_hello(
    const std::string & batsim_version,
    const std::string & batsim_commit)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_batsim_hello() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!batsim_version.empty(), "Invalid (empty) batsim_version received");

    auto batsim_hello = fb::CreateBatsimHelloEventDirect(*_builder, version().c_str(), batsim_version.c_str(), batsim_commit.c_str());
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_BatsimHelloEvent, batsim_hello.Union());
    _events.push_back(event);
}

void MessageBuilder::add_external_decision_component_hello(
    const std::string & decision_component_name,
    const std::string & decision_component_version,
    const std::string & decision_component_commit,
    const EDCHelloOptions & options)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_external_decision_component_hello() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!decision_component_name.empty(), "Invalid (empty) decision_component_name received");
    BAT_ENFORCE(!decision_component_version.empty(), "Invalid (empty) decision_component_version received");

    auto requested_simulation_features = fb::CreateEDCRequestedSimulationFeatures(*_builder,
        options._dynamic_registration,
        options._profile_reuse,
        options._acknowledge_dynamic_jobs,
        options._forward_profiles_on_job_submission,
        options._forward_profiles_on_jobs_killed,
        options._forward_profiles_on_simulation_begins,
        options._forward_unknown_external_events
    );

    auto scheduling_constraints = fb::CreateEDCSchedulingConstraints(*_builder,
        options._compute_sharing,
        options._storage_sharing,
        options._job_allocation_validation_strategy
    );

    auto dc_hello = fb::CreateEDCHelloEventDirect(*_builder,
        version().c_str(),
        decision_component_name.c_str(),
        decision_component_version.c_str(),
        decision_component_commit.c_str(),
        requested_simulation_features,
        scheduling_constraints
    );
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_EDCHelloEvent, dc_hello.Union());
    _events.push_back(event);
}

void MessageBuilder::add_simulation_begins(SimulationBegins & simulation_begins)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_simulation_begins() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(simulation_begins._host_number >= simulation_begins._hosts.size(), "host_number=%u inconsistency: %lu hosts have been declared", simulation_begins._host_number, simulation_begins._hosts.size());

    // Prepare send orders of computation and storage hosts
    std::vector<uint32_t> computation_hosts, storage_hosts;
    for (auto & kv : simulation_begins._hosts)
    {
        if (kv.second->is_storage)
            storage_hosts.push_back(kv.first);
        else
            computation_hosts.push_back(kv.first);
    }

    std::sort(computation_hosts.begin(), computation_hosts.end());
    std::sort(storage_hosts.begin(), storage_hosts.end());

    auto computation_hosts_s = serialize_host_vector(simulation_begins, computation_hosts);
    auto storage_hosts_s = serialize_host_vector(simulation_begins, storage_hosts);
    auto profile_and_ids_s = serialize_profile_and_id_vector(simulation_begins._profiles);
    auto workload_and_filenames_s = serialize_workload_and_filename_vector(simulation_begins._workloads);

    BAT_ENFORCE(simulation_begins._batsim_arguments.get() != nullptr, "Invalid (null) batsim arguments. Please set them via SimulationBegins::set_batsim_arguments");
    auto batsim_arguments_s = serialize_string_vector(*(simulation_begins._batsim_arguments.get()));

    auto simulation_begins_s = fb::CreateSimulationBeginsEventDirect(*_builder, simulation_begins._host_number, computation_hosts.size(), &computation_hosts_s, storage_hosts.size(), &storage_hosts_s, &batsim_arguments_s, simulation_begins._batsim_execution_context.c_str(), &workload_and_filenames_s, &profile_and_ids_s);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_SimulationBeginsEvent, simulation_begins_s.Union());
    _events.push_back(event);
}

void MessageBuilder::add_simulation_ends()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_simulation_ends() while buffer is finished. Please call clear() first.");

    auto simulation_ends = fb::CreateSimulationEndsEvent(*_builder);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_SimulationEndsEvent, simulation_ends.Union());
    _events.push_back(event);
}

void MessageBuilder::add_all_static_jobs_have_been_submitted()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_all_static_jobs_have_been_submitted() while buffer is finished. Please call clear() first.");

    auto all_submit = fb::CreateAllStaticJobsHaveBeenSubmittedEvent(*_builder);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_AllStaticJobsHaveBeenSubmittedEvent, all_submit.Union());
    _events.push_back(event);
}

void MessageBuilder::add_all_static_external_events_have_been_injected()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_all_static_external_events_have_been_injected() while buffer is finished. Please call clear() first.");

    auto all_injected = fb::CreateAllStaticExternalEventsHaveBeenInjectedEvent(*_builder);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_AllStaticExternalEventsHaveBeenInjectedEvent, all_injected.Union());
    _events.push_back(event);
}

void MessageBuilder::add_finish_registration()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_finish_registration() while buffer is finished. Please call clear() first.");

    auto finish_registration = fb::CreateFinishRegistrationEvent(*_builder);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_FinishRegistrationEvent, finish_registration.Union());
    _events.push_back(event);
}

void MessageBuilder::add_force_simulation_stop()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_force_simulation_stop() while buffer is finished. Please call clear() first.");

    auto force_stop = fb::CreateForceSimulationStopEvent(*_builder);
    auto event = fb::CreateEventAndTimestamp(*_builder, _current_time, fb::Event_ForceSimulationStopEvent, force_stop.Union());
    _events.push_back(event);
}

std::vector<flatbuffers::Offset<flatbuffers::String> > MessageBuilder::serialize_string_vector(const std::vector<std::string> & strings)
{
    std::vector<flatbuffers::Offset<flatbuffers::String> > vec_s;
    vec_s.reserve(strings.size());
    for (const auto & str : strings)
    {
        auto str_s = _builder->CreateString(str);
        vec_s.push_back(str_s);
    }
    return vec_s;
}

std::vector<flatbuffers::Offset<batprotocol::fb::Host>> MessageBuilder::serialize_host_vector(const SimulationBegins & simulation_begins, const std::vector<uint32_t> host_ids)
{
    std::vector<flatbuffers::Offset<batprotocol::fb::Host>> hosts_s;
    hosts_s.reserve(host_ids.size());
    for (const auto & host_id : host_ids)
    {
        auto host_it = simulation_begins._hosts.find(host_id);
        BAT_ASSERT(host_it != simulation_begins._hosts.end(), "Internal inconsistency: non-existing host_id");
        const auto * host = host_it->second;

        auto host_properties_s = serialize_host_property_vector(host->properties);
        auto host_zone_properties_s = serialize_host_property_vector(host->zone_properties);

        auto host_s = fb::CreateHostDirect(*_builder, host_id, host->name.c_str(), host->pstate, host->pstate_count, host->state, &host_properties_s, &host_zone_properties_s, host->core_count, host->computation_speed.get());
        hosts_s.push_back(host_s);
    }

    return hosts_s;
}

std::vector<flatbuffers::Offset<batprotocol::fb::HostProperty>> MessageBuilder::serialize_host_property_vector(const std::unordered_map<std::string, std::string> & properties)
{
    std::vector<flatbuffers::Offset<batprotocol::fb::HostProperty>> property_vec_s;
    std::vector<std::string> keys;
    property_vec_s.reserve(properties.size());
    keys.reserve(properties.size());

    for (const auto & kv : properties)
    {
        keys.push_back(kv.first);
    }
    std::sort(keys.begin(), keys.end());

    for (auto key : keys)
    {
        const auto & value = properties.at(key);
        auto property = fb::CreateHostPropertyDirect(*_builder, key.c_str(), value.c_str());
        property_vec_s.push_back(property);
    }

    return property_vec_s;
}

std::vector<flatbuffers::Offset<batprotocol::fb::ProfileAndId>> MessageBuilder::serialize_profile_and_id_vector(std::unordered_map<std::string, std::shared_ptr<Profile> > & profiles)
{
    std::vector<flatbuffers::Offset<batprotocol::fb::ProfileAndId>> profile_vec_s;
    profile_vec_s.reserve(profiles.size());

    // Generate deterministic order
    std::vector<std::string> profile_ids_vector;
    profile_ids_vector.reserve(profiles.size());
    for (const auto & kv : profiles)
        profile_ids_vector.push_back(kv.first);
    std::sort(profile_ids_vector.begin(), profile_ids_vector.end());

    for (const auto & profile_id : profile_ids_vector)
    {
        auto profile = profiles[profile_id];
        BAT_ENFORCE(profile != nullptr, "Invalid (null) profile received for profile_id='%s'", profile_id.c_str());
        auto profile_and_id = serialize_profile_and_id(profile_id, profile);
        profile_vec_s.push_back(profile_and_id);
    }

    return profile_vec_s;
}

std::vector<flatbuffers::Offset<batprotocol::fb::WorkloadAndFilename>> MessageBuilder::serialize_workload_and_filename_vector(std::unordered_map<std::string, std::string> & workloads)
{
    std::vector<flatbuffers::Offset<batprotocol::fb::WorkloadAndFilename>> workload_vec_s;
    workload_vec_s.reserve(workloads.size());

    // Generate deterministic order
    std::vector<std::string> workload_ids_vector;
    workload_ids_vector.reserve(workloads.size());
    for (const auto & kv : workloads)
        workload_ids_vector.push_back(kv.first);
    std::sort(workload_ids_vector.begin(), workload_ids_vector.end());

    for (const auto & workload_id : workload_ids_vector)
    {
        const auto & workload = workloads[workload_id];
        auto workload_and_filename = fb::CreateWorkloadAndFilenameDirect(*_builder, workload_id.c_str(), workload.c_str());
        workload_vec_s.push_back(workload_and_filename);
    }

    return workload_vec_s;
}

std::vector<flatbuffers::Offset<batprotocol::fb::KillProgressWrapper>> MessageBuilder::serialize_kill_progress_vector(const std::vector<std::string> & tasks_to_serialize, const std::string & task_id, const std::string & job_id, std::unordered_map<std::string, flatbuffers::Offset<batprotocol::fb::KillProgressWrapper> > & serialized_tasks)
{
    std::vector<flatbuffers::Offset<batprotocol::fb::KillProgressWrapper>> kp_vec_s;
    kp_vec_s.reserve(tasks_to_serialize.size());

    for (const auto & task_id_to_serialize : tasks_to_serialize)
    {
        auto it = serialized_tasks.find(task_id_to_serialize);
        BAT_ENFORCE(it != serialized_tasks.end(), "invalid KillProgress of job_id='%s': while trying to serialize task_id='%s', its child task_id='%s' has not been already serialized: cycles are not allowed in your progress graph.", job_id.c_str(), task_id.c_str(), task_id_to_serialize.c_str());
        kp_vec_s.push_back(it->second);
    }

    return kp_vec_s;
}


flatbuffers::Offset<fb::Job> MessageBuilder::serialize_job(const std::shared_ptr<Job> & job)
{
    auto job_s = fb::CreateJobDirect(*_builder, job->_resource_number, job->_walltime, job->_extra_data.c_str(), job->_rigid, job->_profile_id.c_str());
    return job_s;
}


flatbuffers::Offset<fb::ProfileAndId> MessageBuilder::serialize_profile_and_id(const std::string & profile_id, const std::shared_ptr<Profile> & profile)
{
    auto profile_id_s = _builder->CreateString(profile_id);

    switch(profile->_profile_type)
    {
    case fb::Profile_NONE: {
        BAT_ENFORCE(false, "Internal inconsistency: should not be possible to create untyped profiles");
    } break;
    case fb::Profile_DelayProfile: {
        auto delay = fb::CreateDelayProfile(*_builder, profile->_delay);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, delay.Union());
    } break;
    case fb::Profile_ParallelTaskProfile: {
        auto ptask = fb::CreateParallelTaskProfileDirect(*_builder, profile->_computation_vector.get(), profile->_communication_matrix.get());
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask.Union());
    } break;
    case fb::Profile_ParallelTaskHomogeneousProfile: {
        auto ptask_hg = fb::CreateParallelTaskHomogeneousProfile(*_builder, profile->_computation_amount, profile->_communication_amount, profile->_generation_strategy);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask_hg.Union());
    } break;
    case fb::Profile_SequentialCompositionProfile: {
        BAT_ASSERT(profile->_sub_profiles.get() != nullptr, "Internal inconsistency: profile->_sub_profiles is null");
        auto sub_profiles = serialize_string_vector(*profile->_sub_profiles);
        auto seq = fb::CreateSequentialCompositionProfileDirect(*_builder, profile->_repetition_count, &sub_profiles);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, seq.Union());
    } break;
    case fb::Profile_ForkJoinCompositionProfile: {
        BAT_ASSERT(profile->_sub_profiles.get() != nullptr, "Internal inconsistency: profile->_sub_profiles is null");
        auto sub_profiles = serialize_string_vector(*profile->_sub_profiles);
        auto fork_join = fb::CreateForkJoinCompositionProfileDirect(*_builder, &sub_profiles);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, fork_join.Union());
    } break;
    case fb::Profile_ParallelTaskMergeCompositionProfile: {
        BAT_ASSERT(profile->_sub_profiles.get() != nullptr, "Internal inconsistency: profile->_sub_profiles is null");
        auto sub_profiles = serialize_string_vector(*profile->_sub_profiles);
        auto ptask_merge = fb::CreateParallelTaskMergeCompositionProfileDirect(*_builder, &sub_profiles);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask_merge.Union());
    } break;
    case fb::Profile_ParallelTaskOnStorageHomogeneousProfile: {
        auto ptask_storage_hg = fb::CreateParallelTaskOnStorageHomogeneousProfileDirect(*_builder, profile->_storage_name.c_str(), profile->_bytes_to_read, profile->_bytes_to_write, profile->_generation_strategy);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask_storage_hg.Union());
    } break;
    case fb::Profile_ParallelTaskDataStagingBetweenStoragesProfile: {
        auto ptask_data_staging = fb::CreateParallelTaskDataStagingBetweenStoragesProfileDirect(*_builder, profile->_bytes_to_transfer, profile->_emitter_storage_name.c_str(), profile->_receiver_storage_name.c_str());
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask_data_staging.Union());
    } break;
    case fb::Profile_TraceReplayProfile: {
        auto trace_replay = fb::CreateTraceReplayProfileDirect(*_builder, profile->_trace_type, profile->_filename.c_str());
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, trace_replay.Union());
    } break;
    }

    BAT_ENFORCE(false, "Unhandled Profile value: %d", profile->_profile_type);
}

flatbuffers::Offset<void> MessageBuilder::serialize_resources(
    fb::Resources resources_type,
    const std::string & hosts_intervalset,
    const std::shared_ptr<std::vector<std::string> > & links)
{
    switch(resources_type) {
        case fb::Resources_NONE: {
            BAT_ENFORCE(false, "Invalid call to serialize_resources: resources have not been set");
        } break;
        case fb::Resources_HostResources: {
            return fb::CreateHostResourcesDirect(*_builder, hosts_intervalset.c_str()).Union();
        } break;
        case fb::Resources_LinkResources: {
            BAT_ENFORCE(links.get() != nullptr, "Invalid (null) links received");
            auto links_s = serialize_string_vector(*links.get());
            return fb::CreateLinkResourcesDirect(*_builder, &links_s).Union();
        } break;
    }

    BAT_ENFORCE(false, "Unhandled resources type value: %d", resources_type);
}


flatbuffers::Offset<void> MessageBuilder::serialize_temporal_trigger(const std::shared_ptr<TemporalTrigger> & temporal_trigger)
{
    BAT_ENFORCE(temporal_trigger.get() != nullptr, "Invalid (null) TemporalTrigger received");

    switch(temporal_trigger->_type)
    {
    case fb::TemporalTrigger_NONE: {
        BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped time specifiers");
    } break;
    case fb::TemporalTrigger_OneShot: {
        return fb::CreateOneShot(*_builder, temporal_trigger->_time_point).Union();
    } break;
    case fb::TemporalTrigger_Periodic: {
        flatbuffers::Offset<void> periodic_mode_s;
        fb::PeriodicMode periodic_mode = fb::PeriodicMode_NONE;
        if (temporal_trigger->_is_finite)
        {
            periodic_mode = fb::PeriodicMode_FinitePeriodNumber;
            periodic_mode_s = fb::CreateFinitePeriodNumber(*_builder, temporal_trigger->_nb_periods).Union();
        }
        else
        {
            periodic_mode = fb::PeriodicMode_Infinite;
            periodic_mode_s = fb::CreateInfinite(*_builder).Union();
        }

        return fb::CreatePeriodic(*_builder, temporal_trigger->_time_point, temporal_trigger->_period, periodic_mode, periodic_mode_s).Union();
    } break;
    }

    BAT_ENFORCE(false, "Unhandled TemporalTrigger value: %d", temporal_trigger->_type);
}

flatbuffers::Offset<void> MessageBuilder::serialize_placement(ExecuteJobOptions::Placement * placement)
{
    // Default placement if unset (nullptr)
    bool should_delete = false;
    if (placement == nullptr)
    {
        placement = ExecuteJobOptions::Placement::make_predefined(fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst);
        should_delete = true;
    }

    switch(placement->_type)
    {
    case fb::ExecutorPlacement_NONE: {
        BAT_ASSERT(false, "Internal inconsistency: should not be able to create untyped placements");
    } break;
    case fb::ExecutorPlacement_PredefinedExecutorPlacementStrategyWrapper: {
        auto strategy = fb::CreatePredefinedExecutorPlacementStrategyWrapper(*_builder, placement->_predefined_strategy);
        if (should_delete)
            delete placement;
        return strategy.Union();
    } break;
    case fb::ExecutorPlacement_CustomExecutorToHostMapping: {
        auto mapping = fb::CreateCustomExecutorToHostMappingDirect(*_builder, placement->_custom_mapping.get());
        return mapping.Union();
    } break;
    }

    BAT_ENFORCE(false, "Unhandled ExecutorPlacement value: %d", placement->_type);
}

flatbuffers::Offset<batprotocol::fb::KillProgressWrapper> MessageBuilder::serialize_kill_progress(const std::string & task_id, const std::string & job_id, const KillProgress::KillProgressVariant * variant, std::unordered_map<std::string, flatbuffers::Offset<batprotocol::fb::KillProgressWrapper> > & serialized_tasks)
{
    BAT_ASSERT(variant != nullptr, "inconsistent (null) kill progress variant received for task_id='%s' of job_id='%s'", task_id.c_str(), job_id.c_str());
    BAT_ENFORCE(serialized_tasks.find(task_id) == serialized_tasks.end(), "invalid KillProgress of job_id='%s': trying to serialize task_id='%s' while it has already been serialized: cycles are not allowed in your progress graph.", job_id.c_str(), task_id.c_str());

    switch (variant->type)
    {
    case fb::KillProgress_NONE: {
        BAT_ASSERT(false, "inconsistency: kill progress type should not be %s", fb::EnumNamesKillProgress()[variant->type]);
    } break;
    case fb::KillProgress_KillProgressAtomicProfile: {
        const auto * kp = static_cast<KillProgress::Atomic*>(variant->data);
        auto kp_s = fb::CreateKillProgressAtomicProfileDirect(*_builder, kp->profile_id.c_str(), kp->progress);
        auto wrapper_s = fb::CreateKillProgressWrapper(*_builder, variant->type, kp_s.Union());
        serialized_tasks[task_id] = wrapper_s;
        return wrapper_s;
    } break;
    case fb::KillProgress_KillProgressSequentialProfile: {
        const auto * kp = static_cast<KillProgress::Sequential*>(variant->data);
        auto child_it = serialized_tasks.find(kp->current_subtask_id);
        BAT_ENFORCE(child_it != serialized_tasks.end(), "invalid kill progress of job_id='%s': trying to serialize task_id='%s' while its child task_id='%s' has not been serialized yet. cycles are not allowed in your progress graph.", job_id.c_str(), task_id.c_str(), kp->current_subtask_id.c_str());
        auto kp_s = fb::CreateKillProgressSequentialProfileDirect(*_builder, kp->profile_id.c_str(), kp->current_repetition, kp->current_task_index, child_it->second);
        auto wrapper_s = fb::CreateKillProgressWrapper(*_builder, variant->type, kp_s.Union());
        serialized_tasks[task_id] = wrapper_s;
        return wrapper_s;
    } break;
    case fb::KillProgress_KillProgressForkJoinProfile: {
        const auto * kp = static_cast<KillProgress::ForkJoin*>(variant->data);
        auto children_s = serialize_kill_progress_vector(kp->subtasks_id, task_id, job_id, serialized_tasks);
        auto kp_s = fb::CreateKillProgressForkJoinProfileDirect(*_builder, kp->profile_id.c_str(), &children_s);
        auto wrapper_s = fb::CreateKillProgressWrapper(*_builder, variant->type, kp_s.Union());
        serialized_tasks[task_id] = wrapper_s;
        return wrapper_s;
    } break;
    }

    BAT_ENFORCE(false, "Unhandled KillProgress variant type value: %d", variant->type);
}


} // end of namespace batprotocol
