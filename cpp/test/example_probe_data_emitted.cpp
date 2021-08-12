// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// Data from an aggregated probe is emitted.
void example_probe_data_emitted_aggregated(batprotocol::MessageBuilder & builder)
{
    auto probe_data = ProbeData::make_aggregated(1e6);
    probe_data->set_resources_as_hosts("0-3");

    builder.add_probe_data_emitted(
        "power-all-hosts",
        fb::Metrics_Power,
        probe_data,
        false,
        1,
        1
    );
}

// Data from a vectorial probe is emitted.
void example_probe_data_emitted_vectorial(batprotocol::MessageBuilder & builder)
{
    auto data = std::vector<double>({50, 30});
    auto probe_data = ProbeData::make_vectorial(
        std::make_shared<std::vector<double> >(std::move(data))
    );

    auto links = std::vector<std::string>({
        "switch0",
        "switch1"
    });
    probe_data->set_resources_as_links(
        std::make_shared<std::vector<std::string> >(std::move(links))
    );

    builder.add_probe_data_emitted(
        "power-switches",
        fb::Metrics_Power,
        probe_data,
        true,
        5,
        7
    );
}
