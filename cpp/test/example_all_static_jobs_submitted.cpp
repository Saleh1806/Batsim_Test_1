// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// All static jobs have been submitted.
void example_all_static_jobs_submitted_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_all_static_jobs_have_been_submitted();
}
