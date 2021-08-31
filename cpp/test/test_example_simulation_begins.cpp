#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_simulation_begins_full(MessageBuilder &);

TEST(example_simulation_begins, full)
{
    MessageBuilder mb(true);
    example_simulation_begins_full(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_SimulationBeginsEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->host_number(), 3);
    EXPECT_EQ(event0->computation_host_number(), 2);
    EXPECT_EQ(event0->storage_host_number(), 1);
    EXPECT_EQ(event0->batsim_execution_context_json()->str(), "another JSON string");

    auto batsim_arguments = std::vector<std::string>({
        "batsim",
        "-p", "cluster_energy_128.xml",
        "-w", "test_energy_minimal_load0.json",
        "-e" "batres",
        "--energy"
    });
    check_identical_str_vectors(event0->batsim_arguments(), batsim_arguments);

    // computation hosts
    auto hosts = event0->computation_hosts();
    EXPECT_EQ(hosts->size(), 2);

    auto host = hosts->Get(0);
    EXPECT_EQ(host->id(), 0);
    EXPECT_EQ(host->name()->str(), "host0");
    EXPECT_EQ(host->pstate(), 0);
    EXPECT_EQ(host->pstate_count(), 1);
    EXPECT_EQ(host->state(), fb::HostState_IDLE);
    EXPECT_EQ(host->core_count(), 1);
    EXPECT_EQ(host->computation_speed()->size(), 1);
    EXPECT_EQ(host->computation_speed()->Get(0), 1e9);
    EXPECT_EQ(host->properties()->size(), 2);
    EXPECT_EQ(host->properties()->Get(0)->key()->str(), "a");
    EXPECT_EQ(host->properties()->Get(0)->value()->str(), "0");
    EXPECT_EQ(host->properties()->Get(1)->key()->str(), "b");
    EXPECT_EQ(host->properties()->Get(1)->value()->str(), "1");
    EXPECT_EQ(host->zone_properties()->size(), 1);
    EXPECT_EQ(host->zone_properties()->Get(0)->key()->str(), "zone_key");
    EXPECT_EQ(host->zone_properties()->Get(0)->value()->str(), "g5k");

    host = hosts->Get(1);
    EXPECT_EQ(host->id(), 1);
    EXPECT_EQ(host->name()->str(), "host1");
    EXPECT_EQ(host->pstate(), 0);
    EXPECT_EQ(host->pstate_count(), 2);
    EXPECT_EQ(host->state(), fb::HostState_SLEEPING);
    EXPECT_EQ(host->core_count(), 8);
    EXPECT_EQ(host->computation_speed()->size(), 2);
    EXPECT_EQ(host->computation_speed()->Get(0), 1e9);
    EXPECT_EQ(host->computation_speed()->Get(1), 1e8);
    EXPECT_EQ(host->properties()->size(), 2);
    EXPECT_EQ(host->properties()->Get(0)->key()->str(), "a");
    EXPECT_EQ(host->properties()->Get(0)->value()->str(), "1");
    EXPECT_EQ(host->properties()->Get(1)->key()->str(), "b");
    EXPECT_EQ(host->properties()->Get(1)->value()->str(), "3");
    EXPECT_EQ(host->zone_properties()->size(), 1);
    EXPECT_EQ(host->zone_properties()->Get(0)->key()->str(), "zone_key");
    EXPECT_EQ(host->zone_properties()->Get(0)->value()->str(), "g5k");

    // storage hosts
    hosts = event0->storage_hosts();
    EXPECT_EQ(hosts->size(), 1);
    host = hosts->Get(0);
    EXPECT_EQ(host->id(), 2);
    EXPECT_EQ(host->name()->str(), "pfs");
    EXPECT_EQ(host->pstate(), 0);
    EXPECT_EQ(host->pstate_count(), 1);
    EXPECT_EQ(host->state(), fb::HostState_IDLE);
    EXPECT_EQ(host->core_count(), 1);
    EXPECT_EQ(host->computation_speed()->size(), 1);
    EXPECT_EQ(host->computation_speed()->Get(0), 1e9);
    EXPECT_EQ(host->properties()->size(), 0);
    EXPECT_EQ(host->zone_properties()->size(), 0);

    // workloads
    EXPECT_EQ(event0->workloads()->size(), 2);
    EXPECT_EQ(event0->workloads()->Get(0)->name()->str(), "w0");
    EXPECT_EQ(event0->workloads()->Get(0)->filename()->str(), "/tmp/workload0.json");
    EXPECT_EQ(event0->workloads()->Get(1)->name()->str(), "w1");
    EXPECT_EQ(event0->workloads()->Get(1)->filename()->str(), "/tmp/workload1.json");

    // profiles
    EXPECT_EQ(event0->profiles()->size(), 2);
    EXPECT_EQ(event0->profiles()->Get(0)->id()->str(), "w0!delay0");
    auto profile = event0->profiles()->Get(0)->profile_as_DelayProfile();
    EXPECT_NE(profile, nullptr);
    EXPECT_EQ(profile->delay(), 60.0);

    EXPECT_EQ(event0->profiles()->Get(1)->id()->str(), "w1!delay0");
    profile = event0->profiles()->Get(1)->profile_as_DelayProfile();
    EXPECT_NE(profile, nullptr);
    EXPECT_EQ(profile->delay(), 60.0);

    write_test_mb(mb);
}
