// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// Batsim says hello!
void example_batsim_hello_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_batsim_hello("5.0.0", "unknown");
}
