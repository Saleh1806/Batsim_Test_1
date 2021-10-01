#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_external_decision_component_hello_simple(MessageBuilder &);
void example_external_decision_component_hello_feature1(MessageBuilder &);
void example_external_decision_component_hello_feature2(MessageBuilder &);
void example_external_decision_component_hello_feature3(MessageBuilder &);
void example_external_decision_component_hello_feature4(MessageBuilder &);
void example_external_decision_component_hello_feature5(MessageBuilder &);
void example_external_decision_component_hello_feature6(MessageBuilder &);
void example_external_decision_component_hello_compute_sharing(MessageBuilder &);
void example_external_decision_component_hello_no_storage_sharing(MessageBuilder &);
void example_external_decision_component_hello_job_alloc_validation_strategy(MessageBuilder &);

TEST(example_external_decision_component_hello, simple)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), false);
    EXPECT_EQ(requested_features->profile_reuse(), false);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), true);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, feature1)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_feature1(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), true);
    EXPECT_EQ(requested_features->profile_reuse(), false);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), true);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, feature2)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_feature2(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), true);
    EXPECT_EQ(requested_features->profile_reuse(), true);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), true);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, feature3)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_feature3(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), true);
    EXPECT_EQ(requested_features->profile_reuse(), true);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), true);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), true);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, feature4)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_feature4(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), false);
    EXPECT_EQ(requested_features->profile_reuse(), false);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), true);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), true);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, feature5)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_feature5(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), false);
    EXPECT_EQ(requested_features->profile_reuse(), false);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), true);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), true);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, feature6)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_feature6(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), false);
    EXPECT_EQ(requested_features->profile_reuse(), false);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), true);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), true);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, compute_sharing)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_compute_sharing(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), false);
    EXPECT_EQ(requested_features->profile_reuse(), false);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), true);
    EXPECT_EQ(sched_constraints->storage_sharing(), false);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, no_storage_sharing)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_no_storage_sharing(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), false);
    EXPECT_EQ(requested_features->profile_reuse(), false);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), false);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestExactly);

    write_test_mb(mb);
}

TEST(example_external_decision_component_hello, job_alloc_validation_strategy)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_job_alloc_validation_strategy(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    auto requested_features = event0->requested_simulation_features();
    EXPECT_NE(requested_features, nullptr);
    EXPECT_EQ(requested_features->dynamic_registration(), false);
    EXPECT_EQ(requested_features->profile_reuse(), false);
    EXPECT_EQ(requested_features->acknowledge_dynamic_jobs(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_job_submission(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_jobs_killed(), false);
    EXPECT_EQ(requested_features->forward_profiles_on_simulation_begins(), false);

    auto sched_constraints = event0->scheduling_constraints();
    EXPECT_NE(sched_constraints, nullptr);
    EXPECT_EQ(sched_constraints->compute_sharing(), false);
    EXPECT_EQ(sched_constraints->storage_sharing(), true);
    EXPECT_EQ(sched_constraints->job_allocation_validation_strategy(), fb::JobAllocationValidationStrategy_MatchJobRequestBigEnough);

    write_test_mb(mb);
}
