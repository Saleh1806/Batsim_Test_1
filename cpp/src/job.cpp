#include "job.hpp"

#include "assert.hpp"

namespace batprotocol
{

std::shared_ptr<Job> Job::make()
{
    std::shared_ptr<Job> job(new Job());
    return job;
}

Job & Job::set_host_number(uint32_t host_number)
{
    BAT_ENFORCE(_request_type == fb::ComputationResourceRequest_NONE, "another computation resource request has already been set");
    _request_type = fb::ComputationResourceRequest_HostNumber;
    _resource_number = host_number;
    return *this;
}

Job & Job::set_core_number(uint32_t core_number)
{
    BAT_ENFORCE(_request_type == fb::ComputationResourceRequest_NONE, "another computation resource request has already been set");
    _request_type = fb::ComputationResourceRequest_CoreNumber;
    _resource_number = core_number;
    return *this;
}


Job & Job::set_walltime(double walltime)
{
    BAT_ENFORCE(walltime == -1 || walltime >= 0, "invalid walltime %g (not -1 nor positive)", walltime);
    _walltime = walltime;
    return *this;
}

Job & Job::set_extra_data(const std::string extra_data)
{
    _extra_data = extra_data;
    return *this;
}

Job & Job::set_profile(const std::string & profile_id)
{
    _profile_id = profile_id;
    return *this;
}

Job & Job::set_non_rigid()
{
    _rigid = false;
    return *this;
}

} // end of namespace batprotocol
