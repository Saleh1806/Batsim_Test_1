// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// The decision component stops registration of profiles/jobs.
void example_finish_registration_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_finish_registration();
}
