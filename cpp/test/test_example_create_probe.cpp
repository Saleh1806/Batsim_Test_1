#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_create_probe_one_shot(MessageBuilder &);
void example_create_probe_periodic(MessageBuilder &);
void example_create_probe_resource_aggregation_sum(MessageBuilder &);
void example_create_probe_resource_aggregation_arithmetic_mean(MessageBuilder &);
void example_create_probe_resource_aggregation_median(MessageBuilder &);
void example_create_probe_resource_aggregation_min(MessageBuilder &);
void example_create_probe_resource_aggregation_max(MessageBuilder &);
void example_create_probe_resource_aggregation_quantile_func(MessageBuilder &);
void example_create_probe_emission_filtering_threshold(MessageBuilder &);
void example_create_probe_accumulate_sum_reset(MessageBuilder &);
void example_create_probe_accumulate_sum_no_reset(MessageBuilder &);

TEST(example_create_probe, one_shot)
{
    MessageBuilder mb(true);
    example_create_probe_one_shot(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-now");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_NoResourcesAggregation);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_OneShot);
    auto one_shot = trigger->temporal_trigger_as_OneShot();
    EXPECT_NE(one_shot, nullptr);
    EXPECT_EQ(one_shot->time(), 30.0);

    write_test_mb(mb);
}

TEST(example_create_probe, periodic)
{
    MessageBuilder mb(true);
    example_create_probe_periodic(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "network-load-pfs");
    EXPECT_EQ(event0->metrics(), fb::Metrics_NetworkLoad);
    EXPECT_EQ(event0->resources_type(), fb::Resources_LinkResources);
    auto resources = event0->resources_as_LinkResources();
    EXPECT_NE(resources, nullptr);
    auto links = std::vector<std::string>({
        "pfs",
        "pfs_UP",
        "pfs_DOWN"
    });
    check_identical_str_vectors(resources->link_ids(), links);
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_NoResourcesAggregation);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_Periodic);
    auto periodic = trigger->temporal_trigger_as_Periodic();
    EXPECT_NE(periodic, nullptr);
    EXPECT_EQ(periodic->start_time(), 30.0);
    EXPECT_EQ(periodic->period(), 5.0);
    EXPECT_EQ(periodic->mode_type(), fb::PeriodicMode_Infinite);

    write_test_mb(mb);
}

TEST(example_create_probe, resource_aggregation_sum)
{
    MessageBuilder mb(true);
    example_create_probe_resource_aggregation_sum(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-now");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_Sum);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_OneShot);
    auto one_shot = trigger->temporal_trigger_as_OneShot();
    EXPECT_NE(one_shot, nullptr);
    EXPECT_EQ(one_shot->time(), 30.0);

    write_test_mb(mb);
}

TEST(example_create_probe, resource_aggregation_arithmetic_mean)
{
    MessageBuilder mb(true);
    example_create_probe_resource_aggregation_arithmetic_mean(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-now");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_ArithmeticMean);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_OneShot);
    auto one_shot = trigger->temporal_trigger_as_OneShot();
    EXPECT_NE(one_shot, nullptr);
    EXPECT_EQ(one_shot->time(), 30.0);

    write_test_mb(mb);
}

TEST(example_create_probe, resource_aggregation_median)
{
    MessageBuilder mb(true);
    example_create_probe_resource_aggregation_median(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-now");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_QuantileFunction);
    auto quantile_func = event0->resources_aggregation_function_as_QuantileFunction();
    EXPECT_NE(quantile_func, nullptr);
    EXPECT_EQ(quantile_func->threshold(), 0.5);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_OneShot);
    auto one_shot = trigger->temporal_trigger_as_OneShot();
    EXPECT_NE(one_shot, nullptr);
    EXPECT_EQ(one_shot->time(), 30.0);

    write_test_mb(mb);
}

TEST(example_create_probe, resource_aggregation_min)
{
    MessageBuilder mb(true);
    example_create_probe_resource_aggregation_min(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-now");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_QuantileFunction);
    auto quantile_func = event0->resources_aggregation_function_as_QuantileFunction();
    EXPECT_NE(quantile_func, nullptr);
    EXPECT_EQ(quantile_func->threshold(), 0.0);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_OneShot);
    auto one_shot = trigger->temporal_trigger_as_OneShot();
    EXPECT_NE(one_shot, nullptr);
    EXPECT_EQ(one_shot->time(), 30.0);

    write_test_mb(mb);
}

TEST(example_create_probe, resource_aggregation_max)
{
    MessageBuilder mb(true);
    example_create_probe_resource_aggregation_max(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-now");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_QuantileFunction);
    auto quantile_func = event0->resources_aggregation_function_as_QuantileFunction();
    EXPECT_NE(quantile_func, nullptr);
    EXPECT_EQ(quantile_func->threshold(), 1.0);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_OneShot);
    auto one_shot = trigger->temporal_trigger_as_OneShot();
    EXPECT_NE(one_shot, nullptr);
    EXPECT_EQ(one_shot->time(), 30.0);

    write_test_mb(mb);
}

TEST(example_create_probe, resource_aggregation_quantile_func)
{
    MessageBuilder mb(true);
    example_create_probe_resource_aggregation_quantile_func(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-now");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_QuantileFunction);
    auto quantile_func = event0->resources_aggregation_function_as_QuantileFunction();
    EXPECT_NE(quantile_func, nullptr);
    EXPECT_EQ(quantile_func->threshold(), 0.25);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_OneShot);
    auto one_shot = trigger->temporal_trigger_as_OneShot();
    EXPECT_NE(one_shot, nullptr);
    EXPECT_EQ(one_shot->time(), 30.0);

    write_test_mb(mb);
}

TEST(example_create_probe, emission_filtering_threshold)
{
    MessageBuilder mb(true);
    example_create_probe_emission_filtering_threshold(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_NoProbeDataAccumulation);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_NoResourcesAggregation);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_ThresholdFilteringFunction);
    auto threshold = event0->emission_filtering_policy_as_ThresholdFilteringFunction();
    EXPECT_NE(threshold, nullptr);
    EXPECT_EQ(threshold->threshold(), 1e6);
    EXPECT_EQ(threshold->operator_(), fb::BooleanComparisonOperator_GreaterThanOrEqual);
    EXPECT_EQ(event0->measurement_triggering_policy_type(), fb::ProbeMeasurementTriggeringPolicy_TemporalTriggerWrapper);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_Periodic);
    auto periodic = trigger->temporal_trigger_as_Periodic();
    EXPECT_NE(periodic, nullptr);
    EXPECT_EQ(periodic->start_time(), 30.0);
    EXPECT_EQ(periodic->period(), 5.0);
    EXPECT_EQ(periodic->mode_type(), fb::PeriodicMode_Infinite);

    write_test_mb(mb);
}

TEST(example_create_probe, accumulate_sum_reset)
{
    MessageBuilder mb(true);
    example_create_probe_accumulate_sum_reset(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_ProbeDataAccumulation);
    auto accumulation = event0->data_accumulation_strategy_as_ProbeDataAccumulation();
    EXPECT_NE(accumulation, nullptr);
    EXPECT_EQ(accumulation->temporal_normalization(), false);
    EXPECT_EQ(accumulation->cumulative_function(), fb::CumulativeFunction_Sum);
    EXPECT_EQ(accumulation->reset_mode_type(), fb::ResetMode_ProbeAccumulationReset);
    auto reset = accumulation->reset_mode_as_ProbeAccumulationReset();
    EXPECT_NE(reset, nullptr);
    EXPECT_EQ(reset->new_value(), 0.0);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_NoResourcesAggregation);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_Periodic);
    auto periodic = trigger->temporal_trigger_as_Periodic();
    EXPECT_NE(periodic, nullptr);
    EXPECT_EQ(periodic->start_time(), 30.0);
    EXPECT_EQ(periodic->period(), 5.0);
    EXPECT_EQ(periodic->mode_type(), fb::PeriodicMode_Infinite);

    write_test_mb(mb);
}

TEST(example_create_probe, accumulate_sum_no_reset)
{
    MessageBuilder mb(true);
    example_create_probe_accumulate_sum_no_reset(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_CreateProbeEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_accumulation_strategy_type(), fb::ProbeDataAccumulationStrategy_ProbeDataAccumulation);
    auto accumulation = event0->data_accumulation_strategy_as_ProbeDataAccumulation();
    EXPECT_NE(accumulation, nullptr);
    EXPECT_EQ(accumulation->temporal_normalization(), true);
    EXPECT_EQ(accumulation->cumulative_function(), fb::CumulativeFunction_Sum);
    EXPECT_EQ(accumulation->reset_mode_type(), fb::ResetMode_NoReset);
    EXPECT_EQ(event0->resources_aggregation_function_type(), fb::ResourcesAggregationFunction_NoResourcesAggregation);
    EXPECT_EQ(event0->temporal_aggregation_function_type(), fb::TemporalAggregationFunction_NoTemporalAggregation);
    EXPECT_EQ(event0->emission_filtering_policy_type(), fb::ProbeEmissionFilteringPolicy_NoFiltering);
    auto trigger = event0->measurement_triggering_policy_as_TemporalTriggerWrapper();
    EXPECT_NE(trigger, nullptr);
    EXPECT_EQ(trigger->temporal_trigger_type(), fb::TemporalTrigger_Periodic);
    auto periodic = trigger->temporal_trigger_as_Periodic();
    EXPECT_NE(periodic, nullptr);
    EXPECT_EQ(periodic->start_time(), 30.0);
    EXPECT_EQ(periodic->period(), 5.0);
    EXPECT_EQ(periodic->mode_type(), fb::PeriodicMode_Infinite);

    write_test_mb(mb);
}
