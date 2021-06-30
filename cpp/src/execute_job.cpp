#include "execute_job.hpp"

#include "assert.hpp"

namespace batprotocol
{

ExecuteJobOptions::~ExecuteJobOptions()
{
    delete _placement;
    _placement = nullptr;

    for (auto & kv : _profile_overrides)
    {
        delete kv.second->placement;
        delete kv.second;
    }
    _profile_overrides.clear();

    _storage_overrides.clear();
}

ExecuteJobOptions & ExecuteJobOptions::set_predefined_placement_strategy(fb::ExecutorPlacementStrategy strategy)
{
    BAT_ENFORCE(_placement == nullptr, "placement has already been set");
    _placement = Placement::make_predefined(strategy);

    return *this;
}

ExecuteJobOptions & ExecuteJobOptions::set_custom_placement(const std::shared_ptr<std::vector<uint32_t> > & mapping)
{
    BAT_ENFORCE(_placement == nullptr, "placement has already been set");
    _placement = Placement::make_custom(mapping);

    return *this;
}

ExecuteJobOptions & ExecuteJobOptions::override_profile_alloc_predefined_placement(const std::string & profile_id, const std::string & host_allocation, fb::ExecutorPlacementStrategy strategy)
{
    BAT_ENFORCE(_profile_overrides.find(profile_id) == _profile_overrides.end(), "placement has already been set for profile '%s'", profile_id.c_str());
    auto profile_placement = new ProfilePlacement;
    profile_placement->host_allocation = host_allocation;
    profile_placement->placement = Placement::make_predefined(strategy);
    _profile_overrides[profile_id] = profile_placement;

    return *this;
}

ExecuteJobOptions & ExecuteJobOptions::override_profile_alloc_custom_placement(const std::string & profile_id, const std::string & host_allocation, const std::shared_ptr<std::vector<uint32_t> > & mapping)
{
    BAT_ENFORCE(_profile_overrides.find(profile_id) == _profile_overrides.end(), "placement has already been set for profile '%s'", profile_id.c_str());
    auto profile_placement = new ProfilePlacement;
    profile_placement->host_allocation = host_allocation;
    profile_placement->placement = Placement::make_custom(mapping);
    _profile_overrides[profile_id] = profile_placement;

    return *this;
}

ExecuteJobOptions & ExecuteJobOptions::override_storage_placement(const std::string & storage_name, uint32_t host_id)
{
    BAT_ENFORCE(_storage_overrides.find(storage_name) == _storage_overrides.end(), "placement has already been set for storage '%s'", storage_name.c_str());
    _storage_overrides[storage_name] = host_id;

    return *this;
}

ExecuteJobOptions::Placement * ExecuteJobOptions::Placement::make_predefined(fb::ExecutorPlacementStrategy predefined_strategy)
{
    auto placement = new Placement;
    placement->_placement_type = fb::ExecutorPlacement_predefined_strategy;
    placement->_predefined_strategy = predefined_strategy;
    return placement;
}

ExecuteJobOptions::Placement * ExecuteJobOptions::Placement::make_custom(const std::shared_ptr<std::vector<uint32_t> > & mapping)
{
    auto placement = new Placement;
    placement->_placement_type = fb::ExecutorPlacement_custom_executor_to_host_mapping;
    placement->_custom_mapping = mapping;
    return placement;
}

} // end of namespace batprotocol
