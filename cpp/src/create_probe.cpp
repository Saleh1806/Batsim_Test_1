#include "create_probe.hpp"

#include "assert.hpp"

namespace batprotocol
{

CreateProbe::~CreateProbe()
{

}

std::shared_ptr<CreateProbe> CreateProbe::make_temporal_triggerred(
    std::shared_ptr<TemporalTrigger> & temporal_trigger)
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
}

CreateProbe & CreateProbe::set_resource_aggregation_as_sum()
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

CreateProbe & CreateProbe::set_resource_aggregation_as_arithmetic_mean()
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

CreateProbe & CreateProbe::set_resource_aggregation_as_median()
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

CreateProbe & CreateProbe::set_resource_aggregation_as_min()
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

CreateProbe & CreateProbe::set_resource_aggregation_as_max()
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

CreateProbe & CreateProbe::set_resource_aggregation_as_quantile_function(
    double threshold)
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

CreateProbe & CreateProbe::set_emission_filtering_as_threshold(
    double threshold, fb::BooleanComparisonOperator comp_operator)
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

CreateProbe & CreateProbe::enable_accumulation_with_reset(
    double reset_value,
    bool temporal_normalization,
    fb::CumulativeFunction cumulative_function)
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

CreateProbe & CreateProbe::enable_accumulation_no_reset(
    bool temporal_normalization,
    fb::CumulativeFunction cumulative_function)
{
    BAT_ENFORCE(false, "NOT IMPLEMENTED");
    return *this;
}

} // end of namespace batprotocol
