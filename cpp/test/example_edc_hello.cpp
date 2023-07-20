// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// An external decision component says hello!
void example_edc_hello_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown");
}

// External decision components that request various simulation features
void example_edc_hello_feature1(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().request_dynamic_registration()
    );
}

void example_edc_hello_feature2(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().request_dynamic_registration()
                         .request_profile_reuse()
    );
}

void example_edc_hello_feature3(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().request_dynamic_registration()
                         .request_profile_reuse()
                         .request_acknowledge_dynamic_jobs()
    );
}

void example_edc_hello_feature4(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().request_forward_profiles_on_job_submission()
    );
}

void example_edc_hello_feature5(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().request_forward_profiles_on_jobs_killed()
    );
}

void example_edc_hello_feature6(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().request_forward_profiles_on_simulation_begins()
    );
}

void example_edc_hello_feature7(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().request_forward_unknown_external_events()
    );
}

void example_edc_hello_compute_sharing(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().set_compute_sharing(true)
                         .set_storage_sharing(false)
    );
}

void example_edc_hello_no_storage_sharing(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().set_compute_sharing(false)
                         .set_storage_sharing(false)
    );
}

void example_edc_hello_job_alloc_validation_strategy(batprotocol::MessageBuilder & builder)
{
    builder.add_edc_hello("cpp-unit-test", "0.1.0", "unknown",
        EDCHelloOptions().set_job_allocation_validation_strategy(fb::JobAllocationValidationStrategy_MatchJobRequestBigEnough)
    );
}
