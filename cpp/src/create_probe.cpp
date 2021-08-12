#include "create_probe.hpp"

#include "assert.hpp"

namespace batprotocol
{

std::shared_ptr<CreateProbe> CreateProbe::make_temporal_triggerred(
    std::shared_ptr<TemporalTrigger> & temporal_trigger)
{
    std::shared_ptr<CreateProbe> create_probe(new CreateProbe());
    create_probe->_measurement_triggering_policy = fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper;
    create_probe->_temporal_trigger = temporal_trigger;

    return create_probe;
}

CreateProbe & CreateProbe::set_resources_as_hosts(const std::string & intervalset)
{
    BAT_ENFORCE(_resources_type == fb::Resources_NONE, "Cannot call set_resources_as_hosts since resources have already been set");
    _resources_type = fb::Resources_HostResources;
    _hosts_resources = intervalset;

    return *this;
}

CreateProbe & CreateProbe::set_resources_as_links(
    const std::shared_ptr<std::vector<std::string> > & links)
{
    BAT_ENFORCE(_resources_type == fb::Resources_NONE, "Cannot call set_resources_as_hosts since resources have already been set");
    BAT_ENFORCE(links.get() != nullptr, "invalid (null) links received");
    _resources_type = fb::Resources_LinkResources;
    _links_resources = links;

    return *this;
}


CreateProbe & CreateProbe::set_resource_aggregation_as_sum()
{
    BAT_ENFORCE(_resource_aggregation_function == fb::ResourcesAggregationFunction_NoResourcesAggregation, "Cannot call set_resource_aggregation_as_sum as a resource aggregation function (%s) has already been set", fb::EnumNamesResourcesAggregationFunction()[_resource_aggregation_function]);
    _resource_aggregation_function = fb::ResourcesAggregationFunction_Sum;

    return *this;
}

CreateProbe & CreateProbe::set_resource_aggregation_as_arithmetic_mean()
{
    BAT_ENFORCE(_resource_aggregation_function == fb::ResourcesAggregationFunction_NoResourcesAggregation, "Cannot call set_resource_aggregation_as_arithmetic_mean as a resource aggregation function (%s) has already been set", fb::EnumNamesResourcesAggregationFunction()[_resource_aggregation_function]);
    _resource_aggregation_function = fb::ResourcesAggregationFunction_ArithmeticMean;

    return *this;
}

CreateProbe & CreateProbe::set_resource_aggregation_as_median()
{
    BAT_ENFORCE(_resource_aggregation_function == fb::ResourcesAggregationFunction_NoResourcesAggregation, "Cannot call set_resource_aggregation_as_median as a resource aggregation function (%s) has already been set", fb::EnumNamesResourcesAggregationFunction()[_resource_aggregation_function]);
    return set_resource_aggregation_as_quantile_function(0.5);
}

CreateProbe & CreateProbe::set_resource_aggregation_as_min()
{
    BAT_ENFORCE(_resource_aggregation_function == fb::ResourcesAggregationFunction_NoResourcesAggregation, "Cannot call set_resource_aggregation_as_min as a resource aggregation function (%s) has already been set", fb::EnumNamesResourcesAggregationFunction()[_resource_aggregation_function]);
    return set_resource_aggregation_as_quantile_function(0.0);
}

CreateProbe & CreateProbe::set_resource_aggregation_as_max()
{
    BAT_ENFORCE(_resource_aggregation_function == fb::ResourcesAggregationFunction_NoResourcesAggregation, "Cannot call set_resource_aggregation_as_max as a resource aggregation function (%s) has already been set", fb::EnumNamesResourcesAggregationFunction()[_resource_aggregation_function]);
    return set_resource_aggregation_as_quantile_function(1.0);
}

CreateProbe & CreateProbe::set_resource_aggregation_as_quantile_function(
    double threshold)
{
    BAT_ENFORCE(_resource_aggregation_function == fb::ResourcesAggregationFunction_NoResourcesAggregation, "Cannot call set_resource_aggregation_as_quantile_function as a resource aggregation function (%s) has already been set", fb::EnumNamesResourcesAggregationFunction()[_resource_aggregation_function]);
    BAT_ENFORCE(threshold >= 0.0 && threshold <= 1.0, "invalid threshold (%g): should be in [0,1]", threshold);

    _resource_aggregation_function = fb::ResourcesAggregationFunction_QuantileFunction;
    _resource_aggregation_quantile_threshold = threshold;

    return *this;
}

CreateProbe & CreateProbe::set_emission_filtering_as_threshold(
    double threshold,
    fb::BooleanComparisonOperator comp_operator)
{
    BAT_ENFORCE(_emission_filtering_policy == fb::ProbeEmissionFilteringPolicy_NoFiltering, "Cannot call set_emission_filtering_as_threshold, as a probe emission filtering policy has already been set");
    _emission_filtering_policy = fb::ProbeEmissionFilteringPolicy_ThresholdFilteringFunction;
    _emission_filtering_threshold = threshold;
    _emission_filtering_threshold_operator = comp_operator;

    return *this;
}

CreateProbe & CreateProbe::enable_accumulation_with_reset(
    double reset_value,
    bool temporal_normalization,
    fb::CumulativeFunction cumulative_function)
{
    BAT_ENFORCE(_data_accumulation_strategy == fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation, "Cannot call enable_accumulation_with_reset as a probe data accumulation strategy has already been set");
    _data_accumulation_strategy = fb::ProbeDataAccumulationStrategy_ProbeDataAccumulation;
    _data_accumulation_reset_mode = fb::ResetMode_ProbeAccumulationReset;
    _data_accumulation_reset_value = reset_value;
    _data_accumulation_temporal_normalization = temporal_normalization;
    _data_accumulation_cumulative_function = cumulative_function;

    return *this;
}

CreateProbe & CreateProbe::enable_accumulation_no_reset(
    bool temporal_normalization,
    fb::CumulativeFunction cumulative_function)
{
    BAT_ENFORCE(_data_accumulation_strategy == fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation, "Cannot call enable_accumulation_no_reset as a probe data accumulation strategy has already been set");
    _data_accumulation_strategy = fb::ProbeDataAccumulationStrategy_ProbeDataAccumulation;
    _data_accumulation_reset_mode = fb::ResetMode_NoReset;
    _data_accumulation_temporal_normalization = temporal_normalization;
    _data_accumulation_cumulative_function = cumulative_function;

    return *this;
}

} // end of namespace batprotocol
