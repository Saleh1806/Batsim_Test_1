#include "edc_hello.hpp"

#include "assert.hpp"

namespace batprotocol
{

EDCHelloOptions & EDCHelloOptions::request_dynamic_registration()
{
    _dynamic_registration = true;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::request_profile_reuse()
{
    _profile_reuse = true;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::request_acknowledge_dynamic_jobs()
{
    _acknowledge_dynamic_jobs = true;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::request_forward_profiles_on_job_submission()
{
    _forward_profiles_on_job_submission = true;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::request_forward_profiles_on_jobs_killed()
{
    _forward_profiles_on_jobs_killed = true;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::request_forward_profiles_on_simulation_begins()
{
    _forward_profiles_on_simulation_begins = true;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::request_forward_unknown_external_events()
{
    _forward_unknown_external_events = true;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::set_compute_sharing(bool value)
{
    _compute_sharing = value;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::set_storage_sharing(bool value)
{
    _storage_sharing = value;
    return *this;
}

EDCHelloOptions & EDCHelloOptions::set_job_allocation_validation_strategy(fb::JobAllocationValidationStrategy strategy)
{
    _job_allocation_validation_strategy = strategy;
    return *this;
}


} // end of namespace batprotocol
