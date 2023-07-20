#include "job.hpp"

#include "assert.hpp"

namespace batprotocol
{

std::shared_ptr<Job> Job::make()
{
    std::shared_ptr<Job> job(new Job());
    return job;
}

Job & Job::set_resource_number(uint32_t res_number)
{
    _resource_number = res_number;
    return *this;
}


Job & Job::set_walltime(double walltime)
{
    BAT_ENFORCE(walltime == -1 || walltime >= 0, "invalid walltime %g (not positive nor -1)", walltime);
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
