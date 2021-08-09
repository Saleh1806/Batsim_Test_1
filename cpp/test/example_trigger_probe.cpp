// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// Trigger a persistent probe, forcing its data emission.
void example_trigger_probe_force(batprotocol::MessageBuilder & builder)
{
    builder.add_trigger_probe("power-all-hosts", true);
}

// Trigger a persistent probe, not forcing its data emission.
// This means the data will only be given back if data passes the probe's filtering policy.
void example_trigger_probe_no_force(batprotocol::MessageBuilder & builder)
{
    builder.add_trigger_probe("power-all-hosts", false);
}
