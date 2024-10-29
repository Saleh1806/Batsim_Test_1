// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// Create a one shot probe.
void example_create_probe_one_shot(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_one_shot(30);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3");

    builder.add_create_probe("power-now", fb::Metrics_Power, create_probe);
}

// Create a periodic probe.
void example_create_probe_periodic(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_periodic(30);
    trigger->set_offset(5);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    auto links = std::vector<std::string>({
        "pfs",
        "pfs_UP",
        "pfs_DOWN"
    });
    create_probe->set_resources_as_links(
        std::make_shared<std::vector<std::string> >(std::move(links))
    );

    builder.add_create_probe("network-load-pfs", fb::Metrics_NetworkLoad, create_probe);
}

// Aggregate values over resources as a sum.
void example_create_probe_resource_aggregation_sum(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_one_shot(30);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .set_resource_aggregation_as_sum();

    builder.add_create_probe("power-now", fb::Metrics_Power, create_probe);
}

// Aggregate values over resources as an arithmetic mean.
void example_create_probe_resource_aggregation_arithmetic_mean(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_one_shot(30);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .set_resource_aggregation_as_arithmetic_mean();

    builder.add_create_probe("power-now", fb::Metrics_Power, create_probe);
}

// Aggregate values over resources as a median.
void example_create_probe_resource_aggregation_median(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_one_shot(30);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .set_resource_aggregation_as_median();

    builder.add_create_probe("power-now", fb::Metrics_Power, create_probe);
}

// Aggregate values over resources as a minimum.
void example_create_probe_resource_aggregation_min(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_one_shot(30);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .set_resource_aggregation_as_min();

    builder.add_create_probe("power-now", fb::Metrics_Power, create_probe);
}

// Aggregate values over resources as a maximum.
void example_create_probe_resource_aggregation_max(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_one_shot(30);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .set_resource_aggregation_as_max();

    builder.add_create_probe("power-now", fb::Metrics_Power, create_probe);
}

// Aggregate values over resources as a quantile function.
void example_create_probe_resource_aggregation_quantile_func(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_one_shot(30);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .set_resource_aggregation_as_quantile_function(0.25);

    builder.add_create_probe("power-now", fb::Metrics_Power, create_probe);
}

// Set an emission filtering threshold.
void example_create_probe_emission_filtering_threshold(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_periodic(30);
    trigger->set_offset(5);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .set_emission_filtering_as_threshold(1e6, fb::BooleanComparisonOperator_GreaterThanOrEqual);

    builder.add_create_probe("power", fb::Metrics_Power, create_probe);
}

// Accumulate values over time as a sum, resetting the value at every period.
void example_create_probe_accumulate_sum_reset(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_periodic(30);
    trigger->set_offset(5);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .enable_accumulation_with_reset(0.0, false, fb::CumulativeFunction_Sum);

    builder.add_create_probe("power", fb::Metrics_Power, create_probe);
}

// Accumulate values over time as a sum, without reset at every period.
void example_create_probe_accumulate_sum_no_reset(batprotocol::MessageBuilder & builder)
{
    auto trigger = TemporalTrigger::make_periodic(30);
    trigger->set_offset(5);
    auto create_probe = CreateProbe::make_temporal_triggerred(trigger);
    create_probe->set_resources_as_hosts("0-3")
                 .enable_accumulation_no_reset(true, fb::CumulativeFunction_Sum);

    builder.add_create_probe("power", fb::Metrics_Power, create_probe);
}
