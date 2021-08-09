// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// All static external events have been injected.
void example_all_static_external_events_injected_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_all_static_external_events_have_been_injected();
}
