#include "simulation_begins.hpp"

#include "assert.hpp"

namespace batprotocol
{

SimulationBegins::~SimulationBegins()
{
    for (auto & kv : _hosts)
    {
        delete kv.second;
    }
    _hosts.clear();
}

SimulationBegins & SimulationBegins::add_host(uint32_t id, const std::string & name, uint32_t pstate, uint32_t pstate_count, fb::HostState state, uint32_t core_count, const std::shared_ptr<std::vector<double> > & computation_speed)
{
    BAT_ENFORCE(_hosts.find(id) == _hosts.end(), "Host with id=%u already exists", id);
    BAT_ENFORCE(!name.empty(), "Invalid (empty) host name received");
    BAT_ENFORCE(computation_speed.get() != nullptr, "Invalid (null) computation_speed received");
    BAT_ENFORCE(!computation_speed->empty(), "Invalid (empty) computation_speed received");
    BAT_ENFORCE(computation_speed->size() == pstate_count, "Invalid computation_speed received (size=%lu differs from pstate_count=%d)", computation_speed->size(), pstate_count);
    BAT_ENFORCE(pstate < pstate_count, "Invalid pstate=%d received: must be in [0,pstate_count[ and pstate_count=%d", pstate, pstate_count);

    auto host = new Host();
    host->name = name;
    host->pstate = pstate;
    host->pstate_count = pstate_count;
    host->state = state;
    host->core_count = core_count;
    host->computation_speed = computation_speed;
    host->is_storage = false;

    _hosts[id] = host;

    return *this;
}

SimulationBegins & SimulationBegins::set_host_property(uint32_t id, const std::string & key, const std::string & value)
{
    auto host_it = _hosts.find(id);
    BAT_ENFORCE(host_it != _hosts.end(), "Host with id=%u does not exist", id);

    host_it->second->properties[key] = value;

    return *this;
}

SimulationBegins & SimulationBegins::set_host_zone_property(uint32_t id, const std::string & key, const std::string & value)
{
    auto host_it = _hosts.find(id);
    BAT_ENFORCE(host_it != _hosts.end(), "Host with id=%u does not exist", id);

    host_it->second->zone_properties[key] = value;

    return *this;
}

SimulationBegins & SimulationBegins::set_host_as_storage(uint32_t id)
{
    auto host_it = _hosts.find(id);
    BAT_ENFORCE(host_it != _hosts.end(), "Host with id=%u does not exist", id);

    host_it->second->is_storage = true;

    return *this;
}

SimulationBegins & SimulationBegins::add_workload(const std::string & workload_id, const std::string & filename)
{
    BAT_ENFORCE(workload_id.find("!") == std::string::npos, "invalid workload_id received (contains '!'): '%s'", workload_id.c_str());
    BAT_ENFORCE(_workloads.find(workload_id) == _workloads.end(), "Workload with id='%s' already exists", workload_id.c_str());
    _workloads[workload_id] = filename;

    return *this;
}

SimulationBegins & SimulationBegins::add_profile(const std::string & profile_id, const std::shared_ptr<Profile> & profile)
{
    BAT_ENFORCE(_profiles.find(profile_id) == _profiles.end(), "Profile with id='%s' already exists", profile_id.c_str());
    _profiles[profile_id] = profile;

    return *this;
}

SimulationBegins & SimulationBegins::set_batsim_arguments(const std::shared_ptr<std::vector<std::string> > & arguments)
{
    BAT_ENFORCE(arguments != nullptr, "Invalid (null) arguments received");
    _batsim_arguments = arguments;

    return *this;
}

SimulationBegins & SimulationBegins::set_batsim_execution_context(const std::string & json)
{
    BAT_ENFORCE(!json.empty(), "Invalid (empty) json batsim execution context received");
    _batsim_execution_context = json;

    return *this;
}

SimulationBegins & SimulationBegins::set_host_number(uint32_t host_number)
{
    _host_number = host_number;

    return *this;
}


} // end of namespace batprotocol
