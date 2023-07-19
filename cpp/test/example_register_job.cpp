// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// Register a job dynamically.
void example_register_job_simple(batprotocol::MessageBuilder & builder)
{
    auto job = Job::make();
    job->set_resource_number(8);
    job->set_walltime(600.0);
    job->set_profile("w0!prof");
    job->set_non_rigid();

    builder.add_register_job("dyn!0", job);
}
