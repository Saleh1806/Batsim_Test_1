// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// A fully completed simulation begins.
void example_simulation_begins_full(batprotocol::MessageBuilder & builder)
{
    SimulationBegins begins;
    begins.add_host(0, "host0", 0, 1, fb::HostState_IDLE, 1,
        std::shared_ptr<std::vector<double> >(new std::vector<double>({1e9})));
    begins.add_host(1, "host1", 0, 2, fb::HostState_SLEEPING, 8,
        std::shared_ptr<std::vector<double> >(new std::vector<double>({1e9, 1e8})));
    begins.add_host(2, "pfs", 0, 1, fb::HostState_IDLE, 1,
        std::shared_ptr<std::vector<double> >(new std::vector<double>({1e9})));
    begins.set_host_number(3);

    begins.set_host_property(0, "a", "0");
    begins.set_host_property(0, "b", "1");
    begins.set_host_property(1, "a", "1");
    begins.set_host_property(1, "b", "3");

    begins.set_host_zone_property(0, "zone_key", "g5k");
    begins.set_host_zone_property(1, "zone_key", "g5k");

    begins.set_host_as_storage(2);

    begins.add_workload("w0", "/tmp/workload0.json");
    begins.add_workload("w1", "/tmp/workload1.json");

    auto profile = Profile::make_delay(60.0);
    begins.add_profile("w0!delay0", profile);
    begins.add_profile("w1!delay0", profile);

    begins.set_batsim_execution_context("another JSON string");

    builder.add_simulation_begins(begins);
}
