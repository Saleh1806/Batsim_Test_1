// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// Reset a cumulative (thus persistent) probe.
void example_reset_probe_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_reset_probe("power-all-hosts", 1.0);
}
