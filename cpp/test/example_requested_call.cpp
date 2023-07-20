// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// A requested call has been triggerred.
void example_requested_call_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_requested_call("example_one_shot", false);
}

// The last requested call from a non-infinite sequence has been triggerred.
void example_requested_call_finite_periodic_last(batprotocol::MessageBuilder & builder)
{
    builder.add_requested_call("example_finite_periodic_last", true);
}
