#pragma once

#include <memory>

#include "batprotocol_generated.h"

#include "temporal_trigger.hpp"

namespace batprotocol
{
    class MessageBuilder;

    class CreateProbe
    {
    public:
        ~CreateProbe();
        static std::shared_ptr<CreateProbe> make_temporal_triggerred(
            std::shared_ptr<TemporalTrigger> & temporal_trigger
        );
        // other make_ functions will be done for future measurement triggering policies

        CreateProbe & set_resources_as_hosts(const std::string & intervalset);
        CreateProbe & set_resources_as_links(const std::shared_ptr<std::vector<std::string> > & links);

        CreateProbe & set_resource_aggregation_as_sum();
        CreateProbe & set_resource_aggregation_as_arithmetic_mean();
        CreateProbe & set_resource_aggregation_as_median();
        CreateProbe & set_resource_aggregation_as_min();
        CreateProbe & set_resource_aggregation_as_max();
        CreateProbe & set_resource_aggregation_as_quantile_function(
            double threshold
        );

        // CreateProbe & set_temporal_aggregation_as_... (not implemented yet)

        CreateProbe & set_emission_filtering_as_threshold(
            double threshold,
            fb::BooleanComparisonOperator comp_operator
        );

        CreateProbe & enable_accumulation_with_reset(
            double reset_value = 0,
            bool temporal_normalization = false,
            fb::CumulativeFunction cumulative_function = fb::CumulativeFunction_Sum
        );
        CreateProbe & enable_accumulation_no_reset(
            bool temporal_normalization = false,
            fb::CumulativeFunction cumulative_function = fb::CumulativeFunction_Sum
        );

    private:
        CreateProbe() = default;
        friend class MessageBuilder;

        fb::ProbeMeasurementTriggeringPolicy _measurement_triggering_policy = fb::ProbeMeasurementTriggeringPolicy_NONE;
        std::shared_ptr<TemporalTrigger> _temporal_trigger;

        fb::Resources _resources_type = fb::Resources_NONE;
        std::string _hosts_resources;
        std::shared_ptr<std::vector<std::string> > _links_resources = nullptr;

        fb::ResourcesAggregationFunction _resource_aggregation_function = fb::ResourcesAggregationFunction_NoResourcesAggregation;
        double _resource_aggregation_quantile_threshold;

        fb::TemporalAggregationFunction _temporal_aggregation_function = fb::TemporalAggregationFunction_NoTemporalAggregation;

        fb::ProbeEmissionFilteringPolicy _emission_filtering_policy = fb::ProbeEmissionFilteringPolicy_NoFiltering;
        double _emission_filtering_threshold;
        fb::BooleanComparisonOperator _emission_filtering_threshold_operator;

        fb::ProbeDataAccumulationStrategy _data_accumulation_strategy = fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation;
        fb::ResetMode _data_accumulation_reset_mode = fb::ResetMode_NONE;
        double _data_accumulation_reset_value = 0;
        bool _data_accumulation_temporal_normalization = false;
        fb::CumulativeFunction _data_accumulation_cumulative_function = fb::CumulativeFunction_Sum;
    };
} // end of namespace batprotocol
