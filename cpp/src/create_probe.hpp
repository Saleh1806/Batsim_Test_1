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

        fb::ResourcesAggregationFunction _resource_aggregation_function = fb::ResourcesAggregationFunction_NoResourcesAggregation;
        double resource_aggregation_quantile_threshold;

        fb::TemporalAggregationFunction _temporal_aggregation_function = fb::TemporalAggregationFunction_NoTemporalAggregation;

        fb::ProbeEmissionFilteringPolicy _emission_filtering_policy = fb::ProbeEmissionFilteringPolicy_NoFiltering;
        double _emission_filtering_threshold;
        fb::BooleanComparisonOperator _emission_filtering_threshold_operator;

        fb::ProbeDataAccumulationStrategy _data_accumulation_strategy = fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation;
        double reset_value = 0;
        bool temporal_normalization = false;
        fb::CumulativeFunction cumulative_function = fb::CumulativeFunction_Sum;
    };
} // end of namespace batprotocol
