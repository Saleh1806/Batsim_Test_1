// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// Stop a persistent probe.
void example_stop_probe_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_stop_probe("power-all-hosts");
}
