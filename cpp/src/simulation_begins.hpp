#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;
    class Profile;

    class SimulationBegins
    {
        SimulationBegins() = default;

        SimulationBegins & add_host(uint32_t id, const std::string & name, uint32_t pstate, uint32_t pstate_count, fb::HostState state, uint32_t core_count, const std::vector<double> computation_speed);
        SimulationBegins & set_host_property(uint32_t id, const std::string & key, const std::string & value);
        SimulationBegins & set_host_as_storage(uint32_t id);

        SimulationBegins & add_workload(const std::string & workload_id, const std::string & filename);

        SimulationBegins & add_profile(const std::string & profile_id, const std::shared_ptr<Profile> & profile);

        SimulationBegins & set_batsim_execution_context(const std::string & json);
    private:
        friend class MessageBuilder;
        // TODO
    };
} // end of namespace batprotocol
