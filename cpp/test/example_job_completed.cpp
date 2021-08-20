// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// A job finished successfully (not killed, returned 0)
void example_job_completed_success(batprotocol::MessageBuilder & builder)
{
    builder.add_job_completed("w0!0",
        fb::FinalJobState_COMPLETED_SUCCESSFULLY,
        0
    );
}

// A job finished with failure (not killed, returned non-0)
void example_job_completed_failure(batprotocol::MessageBuilder & builder)
{
    builder.add_job_completed("w0!1",
        fb::FinalJobState_COMPLETED_FAILED,
        1
    );
}

// A job finished by getting killed
void example_job_completed_killed(batprotocol::MessageBuilder & builder)
{
    builder.add_job_completed("w0!2",
        fb::FinalJobState_COMPLETED_KILLED,
        1
    );
}
