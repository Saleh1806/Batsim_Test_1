#include "message_builder.hpp"

#include "assert.hpp"

namespace batprotocol
{

MessageBuilder::MessageBuilder()
{
    _builder = new flatbuffers::FlatBufferBuilder();
}

MessageBuilder::~MessageBuilder()
{
    delete _builder;
    _builder = nullptr;
}


void MessageBuilder::clear(double now)
{
    _current_time = now;
    _events.clear();
    _builder->Clear();
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

const uint8_t * const MessageBuilder::buffer_pointer() const
{
    BAT_ENFORCE(_is_buffer_finished, "Cannot call buffer_pointer() while buffer is not finished. Please call finish_message() first.");
    return _builder->GetBufferPointer();
}

const uint32_t MessageBuilder::buffer_size() const
{
    BAT_ENFORCE(_is_buffer_finished, "Cannot call buffer_size() while buffer is not finished. Please call finish_message() first.");
    return static_cast<uint32_t>(_builder->GetSize());
}

void MessageBuilder::add_job_submitted(
    const std::string & job_id,
    const std::shared_ptr<Job> & job)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_job_submitted() while buffer is finished. Please call clear() first.");

    auto job_id_s = _builder->CreateString(job_id);
    auto job_s = serialize_job(job);

    auto job_submitted = fb::CreateJobSubmittedEvent(*_builder, job_id_s, job_s);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_JobSubmittedEvent, job_submitted.Union());
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
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_JobCompletedEvent, job_completed.Union());
    _events.push_back(event);
}

void MessageBuilder::add_reject_job(const std::string & job_id)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_reject_job() while buffer is finished. Please call clear() first.");

    auto job_id_s = _builder->CreateString(job_id);

    auto reject_job = fb::CreateRejectJobEvent(*_builder, job_id_s);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_RejectJobEvent, reject_job.Union());
    _events.push_back(event);
}

void MessageBuilder::add_execute_job(
    const std::string & job_id,
    const std::string & host_allocation,
    const ExecuteJobOptions & options)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_execute_job() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(false, "UNIMPLEMENTED");
}

void MessageBuilder::add_kill_jobs(const std::vector<std::string> & job_ids)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_kill_jobs() while buffer is finished. Please call clear() first.");

    auto job_ids_s = serialize_string_vector(job_ids);

    auto kill_jobs = fb::CreateKillJobsEventDirect(*_builder, &job_ids_s);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_KillJobsEvent, kill_jobs.Union());
    _events.push_back(event);
}

void MessageBuilder::add_jobs_killed(
    const std::vector<std::string> & job_ids,
    const KillProgress & progress)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_jobs_killed() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(false, "UNIMPLEMENTED");
}

void MessageBuilder::add_register_profile(
    const std::string & profile_id,
    const std::shared_ptr<Profile> & profile)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_register_profile() while buffer is finished. Please call clear() first.");

    auto profile_and_id = serialize_profile_and_id(profile_id, profile);
    auto register_profile = fb::CreateRegisterProfileEvent(*_builder, profile_and_id);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_RegisterProfileEvent, register_profile.Union());
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
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_RegisterJobEvent, register_job.Union());
    _events.push_back(event);
}

void MessageBuilder::add_call_me_later(
    const std::string & call_me_later_id,
    const std::shared_ptr<TimeSpecifier> & when)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_call_me_later() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!call_me_later_id.empty(), "Invalid (empty) call_me_later_id received");
    BAT_ENFORCE(when.get() != nullptr, "Invalid (null) TimeSpecifier received");

    auto ts_s = serialize_time_specifier(when);

    auto call_me_later = fb::CreateCallMeLaterEventDirect(*_builder, call_me_later_id.c_str(), when->_type, ts_s);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_CallMeLaterEvent, call_me_later.Union());
    _events.push_back(event);
}

void MessageBuilder::add_requested_call(const std::string & call_me_later_id)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_requested_call() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!call_me_later_id.empty(), "Invalid (empty) call_me_later_id received");

    auto call_me_later_id_s = _builder->CreateString(call_me_later_id);

    auto requested_call = fb::CreateRequestedCallEvent(*_builder, call_me_later_id_s);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_RequestedCallEvent, requested_call.Union());
    _events.push_back(event);
}

void MessageBuilder::add_stop_call_me_later(const std::string & call_me_later_id)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_stop_call_me_later() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!call_me_later_id.empty(), "Invalid (empty) call_me_later_id received");

    auto call_me_later_id_s = _builder->CreateString(call_me_later_id);

    auto stop_call_me_later = fb::CreateStopCallMeLaterEvent(*_builder, call_me_later_id_s);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_StopCallMeLaterEvent, stop_call_me_later.Union());
    _events.push_back(event);
}

void MessageBuilder::add_batsim_hello(
    const std::string & batsim_version,
    const std::string & batsim_commit)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_batsim_hello() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!batsim_version.empty(), "Invalid (empty) batsim_version received");

    auto batsim_version_s = _builder->CreateString(batsim_version);
    auto batsim_commit_s = _builder->CreateString(batsim_commit);

    auto batsim_hello = fb::CreateBatsimHelloEvent(*_builder, batsim_version_s, batsim_commit_s);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_BatsimHelloEvent, batsim_hello.Union());
    _events.push_back(event);
}

void MessageBuilder::add_external_decision_component_hello(
    const std::string & decision_component_name,
    const std::string & decision_component_version,
    const std::string & decision_component_commit)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_external_decision_component_hello() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(!decision_component_name.empty(), "Invalid (empty) decision_component_name received");
    BAT_ENFORCE(!decision_component_version.empty(), "Invalid (empty) decision_component_version received");

    auto dc_name_s = _builder->CreateString(decision_component_name);
    auto dc_version_s = _builder->CreateString(decision_component_version);
    auto dc_commit_s = _builder->CreateString(decision_component_commit);

    auto dc_hello = fb::CreateExternalDecisionComponentHelloEvent(*_builder, dc_name_s, dc_version_s, dc_commit_s);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_ExternalDecisionComponentHelloEvent, dc_hello.Union());
    _events.push_back(event);
}

void MessageBuilder::add_simulation_begins(SimulationBegins & simulation_begins)
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_simulation_begins() while buffer is finished. Please call clear() first.");
    BAT_ENFORCE(false, "UNIMPLEMENTED");
}

void MessageBuilder::add_simulation_ends()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_simulation_ends() while buffer is finished. Please call clear() first.");

    auto simulation_ends = fb::CreateSimulationEndsEvent(*_builder);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_SimulationEndsEvent, simulation_ends.Union());
    _events.push_back(event);
}

void MessageBuilder::add_all_static_jobs_have_been_submitted()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_all_static_jobs_have_been_submitted() while buffer is finished. Please call clear() first.");

    auto all_submit = fb::CreateAllStaticJobsHaveBeenSubmittedEvent(*_builder);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_AllStaticJobsHaveBeenSubmittedEvent, all_submit.Union());
    _events.push_back(event);
}

void MessageBuilder::add_all_static_external_events_have_been_injected()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_all_static_external_events_have_been_injected() while buffer is finished. Please call clear() first.");

    auto all_injected = fb::CreateAllStaticExternalEventsHaveBeenInjectedEvent(*_builder);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_AllStaticExternalEventsHaveBeenInjectedEvent, all_injected.Union());
    _events.push_back(event);
}

void MessageBuilder::add_finish_registration()
{
    BAT_ENFORCE(!_is_buffer_finished, "Cannot call add_finish_registration() while buffer is finished. Please call clear() first.");

    auto finish_registration = fb::CreateFinishRegistrationEvent(*_builder);
    auto event = fb::CreateEvent(*_builder, _current_time, fb::EventUnion_FinishRegistrationEvent, finish_registration.Union());
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

flatbuffers::Offset<fb::Job> MessageBuilder::serialize_job(const std::shared_ptr<Job> & job)
{
    auto job_s = fb::CreateJobDirect(*_builder, job->_request_type, job->_resource_number, job->_walltime, job->_extra_data.c_str(), job->_rigid, job->_profile_id.c_str());
    return job_s;
}


flatbuffers::Offset<fb::ProfileAndId> MessageBuilder::serialize_profile_and_id(const std::string & profile_id, const std::shared_ptr<Profile> & profile)
{
    auto profile_id_s = _builder->CreateString(profile_id);

    switch(profile->_profile_type)
    {
    case fb::ProfileUnion_NONE: {
        BAT_ENFORCE(false, "Internal inconsistency: should not be possible to create untyped profiles");
    } break;
    case fb::ProfileUnion_DelayProfile: {
        auto delay = fb::CreateDelayProfile(*_builder, profile->_delay);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, delay.Union());
    } break;
    case fb::ProfileUnion_ParallelTaskProfile: {
        auto ptask = fb::CreateParallelTaskProfileDirect(*_builder, profile->_computation_vector.get(), profile->_communication_matrix.get());
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask.Union());
    } break;
    case fb::ProfileUnion_ParallelTaskHomogeneousProfile: {
        auto ptask_hg = fb::CreateParallelTaskHomogeneousProfile(*_builder, profile->_computation_amount, profile->_communication_amount, profile->_generation_strategy);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask_hg.Union());
    } break;
    case fb::ProfileUnion_SequentialCompositionProfile: {
        BAT_ASSERT(profile->_sub_profiles.get() != nullptr, "Internal inconsistency: profile->_sub_profiles is null");
        auto sub_profiles = serialize_string_vector(*profile->_sub_profiles);
        auto seq = fb::CreateSequentialCompositionProfileDirect(*_builder, profile->_repetition_count, &sub_profiles);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, seq.Union());
    } break;
    case fb::ProfileUnion_ForkJoinCompositionProfile: {
        BAT_ASSERT(profile->_sub_profiles.get() != nullptr, "Internal inconsistency: profile->_sub_profiles is null");
        auto sub_profiles = serialize_string_vector(*profile->_sub_profiles);
        auto fork_join = fb::CreateForkJoinCompositionProfileDirect(*_builder, &sub_profiles);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, fork_join.Union());
    } break;
    case fb::ProfileUnion_ParallelTaskMergeCompositionProfile: {
        BAT_ASSERT(profile->_sub_profiles.get() != nullptr, "Internal inconsistency: profile->_sub_profiles is null");
        auto sub_profiles = serialize_string_vector(*profile->_sub_profiles);
        auto ptask_merge = fb::CreateParallelTaskMergeCompositionProfileDirect(*_builder, &sub_profiles);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask_merge.Union());
    } break;
    case fb::ProfileUnion_ParallelTaskOnStorageHomogeneousProfile: {
        auto ptask_storage_hg = fb::CreateParallelTaskOnStorageHomogeneousProfileDirect(*_builder, profile->_storage_name.c_str(), profile->_bytes_to_read, profile->_bytes_to_write, profile->_generation_strategy);
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask_storage_hg.Union());
    } break;
    case fb::ProfileUnion_ParallelTaskDataStagingBetweenStoragesProfile: {
        auto ptask_data_staging = fb::CreateParallelTaskDataStagingBetweenStoragesProfileDirect(*_builder, profile->_bytes_to_transfer, profile->_emitter_storage_name.c_str(), profile->_receiver_storage_name.c_str());
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, ptask_data_staging.Union());
    } break;
    case fb::ProfileUnion_TraceReplayProfile: {
        auto trace_replay = fb::CreateTraceReplayProfileDirect(*_builder, profile->_trace_type, profile->_filename.c_str());
        return fb::CreateProfileAndId(*_builder, profile_id_s, profile->_profile_type, trace_replay.Union());
    } break;
    }

    BAT_ENFORCE(false, "Unhandled ProfileUnion value: %d", profile->_profile_type);
}

flatbuffers::Offset<void> MessageBuilder::serialize_time_specifier(const std::shared_ptr<TimeSpecifier> & time_specifier)
{
    BAT_ENFORCE(time_specifier.get() != nullptr, "Invalid (null) TimeSpecifier received");

    switch(time_specifier->_type)
    {
    case fb::TimeSpecifierUnion_NONE: {
        BAT_ASSERT(false, "Internal inconsistency: should not be able to created untyped time specifiers");
    } break;
    case fb::TimeSpecifierUnion_OneTime: {
        return fb::CreateOneTime(*_builder, time_specifier->_time_point).Union();
    } break;
    case fb::TimeSpecifierUnion_Periodic: {
        return fb::CreatePeriodic(*_builder, time_specifier->_time_point, time_specifier->_period).Union();
    } break;
    }

    BAT_ENFORCE(false, "Unhandled TimeSpecifierUnion value: %d", time_specifier->_type);
}


} // end of namespace batprotocol
