#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_probe_data_emitted_aggregated(MessageBuilder &);
void example_probe_data_emitted_vectorial(MessageBuilder &);

TEST(example_probe_data_emitted, aggregated)
{
    MessageBuilder mb(true);
    example_probe_data_emitted_aggregated(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ProbeDataEmittedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-all-hosts");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->manually_triggered(), false);
    EXPECT_EQ(event0->nb_emitted(), 1);
    EXPECT_EQ(event0->nb_triggered(), 1);
    EXPECT_EQ(event0->resources_type(), fb::Resources_HostResources);
    auto resources = event0->resources_as_HostResources();
    EXPECT_NE(resources, nullptr);
    EXPECT_EQ(resources->host_ids()->str(), "0-3");
    EXPECT_EQ(event0->data_type(), fb::ProbeData_AggregatedProbeData);
    auto data = event0->data_as_AggregatedProbeData();
    EXPECT_NE(data, nullptr);
    EXPECT_EQ(data->data(), 1e6);

    write_test_mb(mb);
}

TEST(example_probe_data_emitted, vectorial)
{
    MessageBuilder mb(true);
    example_probe_data_emitted_vectorial(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ProbeDataEmittedEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->probe_id()->str(), "power-switches");
    EXPECT_EQ(event0->metrics(), fb::Metrics_Power);
    EXPECT_EQ(event0->manually_triggered(), true);
    EXPECT_EQ(event0->nb_emitted(), 5);
    EXPECT_EQ(event0->nb_triggered(), 7);
    EXPECT_EQ(event0->resources_type(), fb::Resources_LinkResources);
    auto resources = event0->resources_as_LinkResources();
    EXPECT_NE(resources, nullptr);
    auto links = std::vector<std::string>({
        "switch0",
        "switch1"
    });
    check_identical_str_vectors(resources->link_ids(), links);
    EXPECT_EQ(event0->data_type(), fb::ProbeData_VectorialProbeData);
    auto data = event0->data_as_VectorialProbeData();
    EXPECT_NE(data, nullptr);
    auto expected_data = std::vector<double>({
        50, 30
    });
    check_identical_vectors(data->data(), expected_data);

    write_test_mb(mb);
}
