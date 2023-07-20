// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// The decision component forces to stop the simulation.
void example_force_simulation_stop_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_force_simulation_stop();
}
