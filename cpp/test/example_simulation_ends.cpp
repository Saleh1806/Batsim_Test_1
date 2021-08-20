// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// Simulation is about to end.
void example_simulation_ends_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_simulation_ends();
}
