// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// A simple job has been submitted.
void example_job_submitted_simple(batprotocol::MessageBuilder & builder)
{
    auto job = Job::make();
    job->set_resource_number(4);
    job->set_profile("w0!prof");
    job->set_walltime(3600.0);

    builder.add_job_submitted("w0!0", job, 0.0);
}

// A job with extra data has been submitted.
void example_job_submitted_extra_data(batprotocol::MessageBuilder & builder)
{
    auto job = Job::make();
    job->set_resource_number(16);
    job->set_profile("w0!prof");
    job->set_non_rigid();
    job->set_extra_data(R"("application": "npb-lu")");

    builder.add_job_submitted("w0!1", job, 1.0);
}

// A ghost/virtual job (without final computation request) has been submitted.
// This kind of jobs can be convenient to register jobs/profiles dynamically.
void example_job_submitted_ghost(batprotocol::MessageBuilder & builder)
{
    auto job = Job::make();
    job->set_extra_data("... (some data to do dynamic registrations)");

    builder.add_job_submitted("w0!2", job, 2.0);
}
