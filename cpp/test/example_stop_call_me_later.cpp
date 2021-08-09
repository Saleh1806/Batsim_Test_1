// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// Stop a (non one shot) call me later.
void example_stop_call_me_later(batprotocol::MessageBuilder & builder)
{
    builder.add_stop_call_me_later("example_infinite_period");
}
