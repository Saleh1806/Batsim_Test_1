#pragma once

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;

    class EDCHelloOptions
    {
    public:
        EDCHelloOptions() = default;
        ~EDCHelloOptions() = default;

        EDCHelloOptions & request_dynamic_registration();
        EDCHelloOptions & request_profile_reuse();
        EDCHelloOptions & request_acknowledge_dynamic_jobs();
        EDCHelloOptions & request_forward_profiles_on_job_submission();
        EDCHelloOptions & request_forward_profiles_on_jobs_killed();
        EDCHelloOptions & request_forward_profiles_on_simulation_begins();

        EDCHelloOptions & set_compute_sharing(bool value);
        EDCHelloOptions & set_storage_sharing(bool value);
        EDCHelloOptions & set_job_allocation_validation_strategy(fb::JobAllocationValidationStrategy strategy);
    private:
        friend class MessageBuilder;

        bool _dynamic_registration = false;
        bool _profile_reuse = false;
        bool _acknowledge_dynamic_jobs = false;
        bool _forward_profiles_on_job_submission = false;
        bool _forward_profiles_on_jobs_killed = false;
        bool _forward_profiles_on_simulation_begins = false;

        bool _compute_sharing = false;
        bool _storage_sharing = true;
        fb::JobAllocationValidationStrategy _job_allocation_validation_strategy = fb::JobAllocationValidationStrategy_MatchJobRequestExactly;
    };
} // end of namespace batprotocol
