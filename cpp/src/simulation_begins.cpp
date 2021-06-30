#include "simulation_begins.hpp"

namespace batprotocol
{

SimulationBegins & SimulationBegins::add_host(uint32_t id, const std::string & name, uint32_t pstate, uint32_t pstate_count, fb::HostState state, uint32_t core_count, const std::vector<double> computation_speed)
{
    // TODO
    return *this;
}

SimulationBegins & SimulationBegins::set_host_property(uint32_t id, const std::string & key, const std::string & value)
{
    // TODO
    return *this;
}

SimulationBegins & SimulationBegins::set_host_as_storage(uint32_t id)
{
    // TODO
    return *this;
}

SimulationBegins & SimulationBegins::add_workload(const std::string & workload_id, const std::string & filename)
{
    // TODO
    return *this;
}

SimulationBegins & SimulationBegins::add_profile(const std::string & profile_id, const std::shared_ptr<Profile> & profile)
{
    // TODO
    return *this;
}

SimulationBegins & SimulationBegins::set_batsim_execution_context(const std::string & json)
{
    // TODO
    return *this;
}

} // end of namespace batprotocol
