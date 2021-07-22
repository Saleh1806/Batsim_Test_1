// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// Kill a single job.
void example_kill_jobs_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_kill_jobs({"w0!0"});
}

// Kill several jobs in one decision.
// Kill acknowledgment will only be sent once all the involved jobs are killed.
void example_kill_jobs_multiple(batprotocol::MessageBuilder & builder)
{
    builder.add_kill_jobs({"w0!3", "w0!7", "dyn!0"});
}
