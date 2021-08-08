#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_jobs_killed_atomic_simple(MessageBuilder &);
void example_jobs_killed_atomic_several_jobs(MessageBuilder &);
void example_jobs_killed_sequential_simple(MessageBuilder &);
void example_jobs_killed_sequential_recursive(MessageBuilder &);
void example_jobs_killed_forkjoin_simple(MessageBuilder &);
void example_jobs_killed_forkjoin_recursive(MessageBuilder &);

TEST(example_jobs_killed, atomic_simple)
{
    MessageBuilder mb(true);
    example_jobs_killed_atomic_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobsKilledEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_ids()->size(), 1u);
    EXPECT_EQ(event0->progresses()->size(), 1u);
    EXPECT_EQ(event0->job_ids()->Get(0)->str(), "w0!0");
    auto progress = event0->progresses()->Get(0);
    EXPECT_EQ(progress->job_id()->str(), "w0!0");
    auto atomic = progress->wrapper()->kill_progress_as_KillProgressAtomicProfile();
    EXPECT_NE(atomic, nullptr);
    EXPECT_EQ(atomic->profile_id()->str(), "w0!prof");
    EXPECT_EQ(atomic->progress(), 0.5);

    write_test_mb(mb);
}

TEST(example_jobs_killed, atomic_several_jobs)
{
    MessageBuilder mb(true);
    example_jobs_killed_atomic_several_jobs(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobsKilledEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_ids()->size(), 2u);
    EXPECT_EQ(event0->progresses()->size(), 2u);

    EXPECT_EQ(event0->job_ids()->Get(0)->str(), "w0!3");
    auto progress = event0->progresses()->Get(0);
    EXPECT_EQ(progress->job_id()->str(), "w0!3");
    auto atomic = progress->wrapper()->kill_progress_as_KillProgressAtomicProfile();
    EXPECT_NE(atomic, nullptr);
    EXPECT_EQ(atomic->profile_id()->str(), "w0!prof");
    EXPECT_EQ(atomic->progress(), 0.3);

    EXPECT_EQ(event0->job_ids()->Get(1)->str(), "w0!4");
    progress = event0->progresses()->Get(1);
    EXPECT_EQ(progress->job_id()->str(), "w0!4");
    atomic = progress->wrapper()->kill_progress_as_KillProgressAtomicProfile();
    EXPECT_NE(atomic, nullptr);
    EXPECT_EQ(atomic->profile_id()->str(), "w0!prof");
    EXPECT_EQ(atomic->progress(), 0.7);

    write_test_mb(mb);
}

TEST(example_jobs_killed, sequential_simple)
{
    MessageBuilder mb(true);
    example_jobs_killed_sequential_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobsKilledEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_ids()->size(), 1u);
    EXPECT_EQ(event0->progresses()->size(), 1u);
    EXPECT_EQ(event0->job_ids()->Get(0)->str(), "w0!1");
    auto progress = event0->progresses()->Get(0);
    EXPECT_EQ(progress->job_id()->str(), "w0!1");
    auto sequential = progress->wrapper()->kill_progress_as_KillProgressSequentialProfile();
    EXPECT_NE(sequential, nullptr);
    EXPECT_EQ(sequential->profile_id()->str(), "w0!prof_seq");
    EXPECT_EQ(sequential->current_repetition(), 1);
    EXPECT_EQ(sequential->current_task_index(), 2);
    auto atomic = sequential->current_task_progress()->kill_progress_as_KillProgressAtomicProfile();
    EXPECT_EQ(atomic->profile_id()->str(), "w0!prof");
    EXPECT_EQ(atomic->progress(), 0.1);

    write_test_mb(mb);
}

TEST(example_jobs_killed, sequential_recursive)
{
    MessageBuilder mb(true);
    example_jobs_killed_sequential_recursive(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobsKilledEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_ids()->size(), 1u);
    EXPECT_EQ(event0->progresses()->size(), 1u);
    EXPECT_EQ(event0->job_ids()->Get(0)->str(), "w0!2");
    auto progress = event0->progresses()->Get(0);
    EXPECT_EQ(progress->job_id()->str(), "w0!2");
    auto sequential = progress->wrapper()->kill_progress_as_KillProgressSequentialProfile();
    EXPECT_NE(sequential, nullptr);
    EXPECT_EQ(sequential->profile_id()->str(), "w0!prof_seq_seq");
    EXPECT_EQ(sequential->current_repetition(), 0);
    EXPECT_EQ(sequential->current_task_index(), 0);
    auto sequential2 = sequential->current_task_progress()->kill_progress_as_KillProgressSequentialProfile();
    EXPECT_NE(sequential2, nullptr);
    EXPECT_EQ(sequential2->profile_id()->str(), "w0!prof_seq");
    EXPECT_EQ(sequential2->current_repetition(), 1);
    EXPECT_EQ(sequential2->current_task_index(), 2);
    auto atomic = sequential2->current_task_progress()->kill_progress_as_KillProgressAtomicProfile();
    EXPECT_NE(atomic, nullptr);
    EXPECT_EQ(atomic->profile_id()->str(), "w0!prof");
    EXPECT_EQ(atomic->progress(), 0.2);

    write_test_mb(mb);
}

TEST(example_jobs_killed, forkjoin_simple)
{
    MessageBuilder mb(true);
    example_jobs_killed_forkjoin_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobsKilledEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_ids()->size(), 1u);
    EXPECT_EQ(event0->progresses()->size(), 1u);
    EXPECT_EQ(event0->job_ids()->Get(0)->str(), "w0!5");
    auto progress = event0->progresses()->Get(0);
    EXPECT_EQ(progress->job_id()->str(), "w0!5");
    auto forkjoin = progress->wrapper()->kill_progress_as_KillProgressForkJoinProfile();
    EXPECT_NE(forkjoin, nullptr);
    EXPECT_EQ(forkjoin->profile_id()->str(), "w0!prof_forkjoin");
    EXPECT_EQ(forkjoin->tasks_progress()->size(), 2u);

    auto atomic0 = forkjoin->tasks_progress()->Get(0)->kill_progress_as_KillProgressAtomicProfile();
    EXPECT_NE(atomic0, nullptr);
    EXPECT_EQ(atomic0->profile_id()->str(), "w0!prof");
    EXPECT_EQ(atomic0->progress(), 0.4);

    auto atomic1 = forkjoin->tasks_progress()->Get(1)->kill_progress_as_KillProgressAtomicProfile();
    EXPECT_NE(atomic1, nullptr);
    EXPECT_EQ(atomic1->profile_id()->str(), "w0!prof2");
    EXPECT_EQ(atomic1->progress(), 0.8);

    write_test_mb(mb);
}

TEST(example_jobs_killed, forkjoin_recursive)
{
    MessageBuilder mb(true);
    example_jobs_killed_forkjoin_recursive(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_JobsKilledEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_ids()->size(), 1u);
    EXPECT_EQ(event0->progresses()->size(), 1u);
    EXPECT_EQ(event0->job_ids()->Get(0)->str(), "w0!6");

    auto progress = event0->progresses()->Get(0);
    EXPECT_EQ(progress->job_id()->str(), "w0!6");
    auto forkjoin = progress->wrapper()->kill_progress_as_KillProgressForkJoinProfile();
    EXPECT_NE(forkjoin, nullptr);
    EXPECT_EQ(forkjoin->profile_id()->str(), "w0!prof_forkjoin2");
    EXPECT_EQ(forkjoin->tasks_progress()->size(), 1u);

    auto forkjoin2 = forkjoin->tasks_progress()->Get(0)->kill_progress_as_KillProgressForkJoinProfile();
    EXPECT_NE(forkjoin2, nullptr);
    EXPECT_EQ(forkjoin2->profile_id()->str(), "w0!prof_forkjoin3");
    EXPECT_EQ(forkjoin2->tasks_progress()->size(), 1u);

    auto atomic = forkjoin2->tasks_progress()->Get(0)->kill_progress_as_KillProgressAtomicProfile();
    EXPECT_NE(atomic, nullptr);
    EXPECT_EQ(atomic->profile_id()->str(), "w0!prof");
    EXPECT_EQ(atomic->progress(), 0.9);

    write_test_mb(mb);
}
