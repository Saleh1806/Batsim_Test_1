#include "message_builder.hpp"

namespace batprotocol
{

MessageBuilder::MessageBuilder()
{
    // TODO
}

MessageBuilder::~MessageBuilder()
{
    // TODO
}


void MessageBuilder::clear()
{
    // TODO
}

void MessageBuilder::generate_message()
{
    // TODO
}

const uint8_t * const MessageBuilder::buffer_pointer() const
{
    // TODO
}

const uint32_t MessageBuilder::buffer_size() const
{
    // TODO
}

void MessageBuilder::add_job_submitted(
    const std::string & job_id,
    const std::shared_ptr<Job> & job)
{
    // TODO
}

void MessageBuilder::add_job_completed(
    const std::string & job_id,
    fb::FinalJobState state,
    uint32_t return_code)
{
    // TODO
}

void MessageBuilder::add_reject_job(const std::string & job_id)
{
    // TODO
}

void MessageBuilder::add_execute_job(
    const std::string & job_id,
    const std::string & host_allocation,
    const ExecuteJobOptions & options)
{
    // TODO
}

void MessageBuilder::add_kill_jobs(const std::vector<std::string> & job_ids)
{
    // TODO
}

void MessageBuilder::add_jobs_killed(
    const std::vector<std::string> & job_ids,
    const KillProgress & progress)
{
    // TODO
}

void MessageBuilder::add_register_profile(
    const std::string & profile_id,
    const std::shared_ptr<Profile> & profile)
{
    // TODO
}

void MessageBuilder::add_register_job(
    const std::string & job_id,
    const std::shared_ptr<Job> & job)
{
    // TODO
}

void MessageBuilder::add_call_me_later(
    const std::string & call_me_later_id,
    const std::shared_ptr<TimeSpecifier> & when)
{
    // TODO
}

void MessageBuilder::add_requested_call(const std::string & call_me_later_id)
{
    // TODO
}

void MessageBuilder::add_stop_call_me_later(const std::string & call_me_later_id)
{
    // TODO
}

void MessageBuilder::add_batsim_hello(
    const std::string & batsim_version,
    const std::string & batsim_commit)
{
    // TODO
}

void MessageBuilder::add_external_decision_component_hello(
    const std::string & decision_component_name,
    const std::string & decision_component_version,
    const std::string & decision_component_commit)
{
    // TODO
}

void MessageBuilder::add_simulation_begins(
    SimulationBegins & simulation_begins
)
{
    // TODO
}

void MessageBuilder::add_simulation_ends()
{
    // TODO
}

void MessageBuilder::add_all_static_jobs_have_been_submitted()
{
    // TODO
}

void MessageBuilder::add_all_static_external_events_have_been_injected()
{
    // TODO
}

void MessageBuilder::add_finish_registration()
{
    // TODO
}

} // end of namespace batprotocol
