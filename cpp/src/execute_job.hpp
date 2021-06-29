#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class ExecuteJobOptions
    {
    public:
        ExecuteJobOptions() = default;
        ~ExecuteJobOptions();

        ExecuteJobOptions & set_predefined_placement_strategy(fb::ExecutorPlacementStrategy strategy);
        ExecuteJobOptions & set_custom_placement(const std::shared_ptr<std::vector<uint32_t> > & mapping);

        ExecuteJobOptions & override_profile_alloc_predefined_placement(const std::string & profile_id, const std::string & host_allocation, fb::ExecutorPlacementStrategy strategy);
        ExecuteJobOptions & override_profile_alloc_custom_placement(const std::string & profile_id, const std::string & host_allocation, const std::shared_ptr<std::vector<uint32_t> > & mapping);

        ExecuteJobOptions & override_storage_placement(const std::string & storage_name, uint32_t host_id);

    private:
        friend class EventQueue;
        enum class PlacementType
        {
            UNSET
            ,PREDEFINED_STRATEGY
            ,CUSTOM
        };

        class Placement
        {
        public:
            Placement * make_predefined(fb::ExecutorPlacementStrategy predefined_strategy);
            Placement * make_custom(const std::shared_ptr<std::vector<uint32_t> > & mapping);
            ~Placement = default();

            PlacementType _placement_type = PlacementType::UNSET;
            fb::ExecutorPlacementStrategy _predefined_strategy;
            std::shared_ptr<std::vector<uint32_t> > _custom_mapping;

        private:
            Placement() = default;
        }

        struct ProfilePlacement
        {
            std::string host_allocation = "";
            Placement * placement = nullptr;
        }

        Placement * _placement = nullptr;
        std::unordered_map<std::string, ProfilePlacement*> _profile_overrides;
        std::unordered_map<std::string, uint32_t> _storage_overrides;
    };

} // end of namespace batprotocol
