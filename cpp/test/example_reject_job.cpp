// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// Reject a job instead of executing it.
void example_reject_job_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_reject_job("w0!0");
}
