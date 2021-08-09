#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;
    class Profile;

    class SimulationBegins
    {
    public:
        SimulationBegins() = default;
        ~SimulationBegins();

        SimulationBegins & add_host(uint32_t id, const std::string & name, uint32_t pstate, uint32_t pstate_count, fb::HostState state, uint32_t core_count, const std::shared_ptr<std::vector<double> > & computation_speed);
        SimulationBegins & set_host_property(uint32_t id, const std::string & key, const std::string & value);
        SimulationBegins & set_host_zone_property(uint32_t id, const std::string & key, const std::string & value);
        SimulationBegins & set_host_as_storage(uint32_t id);

        SimulationBegins & add_workload(const std::string & workload_id, const std::string & filename);

        SimulationBegins & add_profile(const std::string & profile_id, const std::shared_ptr<Profile> & profile);

        SimulationBegins & set_batsim_execution_context(const std::string & json);
        SimulationBegins & set_host_number(uint32_t host_number);
    private:
        friend class MessageBuilder;

        struct Host
        {
            std::string name;
            uint32_t pstate = 0;
            uint32_t pstate_count = 1;
            fb::HostState state = fb::HostState_IDLE;
            uint32_t core_count = 1;
            std::shared_ptr<std::vector<double> > computation_speed;
            std::unordered_map<std::string, std::string> properties;
            std::unordered_map<std::string, std::string> zone_properties;
            bool is_storage = false;
        };

        uint32_t _host_number = 0;
        std::unordered_map<uint32_t, Host*> _hosts;
        std::unordered_map<std::string, std::string> _workloads;
        std::unordered_map<std::string, std::shared_ptr<Profile> > _profiles;
        std::string _batsim_execution_context;
    };
} // end of namespace batprotocol
