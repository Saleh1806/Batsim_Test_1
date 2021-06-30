#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;

    class Job
    {
    public:
        static std::shared_ptr<Job> make();

        Job & set_host_number(uint32_t host_number);
        Job & set_core_number(uint32_t core_number);

        Job & set_walltime(double walltime);
        Job & set_extra_data(const std::string extra_data);

        Job & set_profile(const std::string & profile_id);
        Job & set_non_rigid();
    private:
        friend class MessageBuilder;
        Job() = default;
        fb::ComputationResourceRequest _request_type = fb::ComputationResourceRequest_NONE;
        uint32_t _resource_number;
        double _walltime = -1;
        bool _rigid = true;
        std::string _extra_data;
        std::string _profile_id;
    };
} // end of namespace batprotocol
