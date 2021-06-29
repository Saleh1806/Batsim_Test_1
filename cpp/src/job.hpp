#pragma once

namespace batprotocol
{
    class Job
    {
    public:
        Job() = default;

        Job & set_host_number(uint32_t host_number);
        Job & set_core_number(uint32_t core_number);

        Job & set_walltime(double walltime);
        Job & set_extra_data(const std::string extra_data);

        Job & set_profile(const std::string & profile_id);
        Job & set_non_rigid();
    private:
        TODO
    };
} // end of namespace batprotocol
