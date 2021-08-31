// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// A single atomic profile got killed.
void example_jobs_killed_atomic_simple(MessageBuilder & builder)
{
    auto kill_progress = KillProgress::make("w0!prof!0");
    kill_progress->add_atomic("w0!prof!0", "w0!prof", 0.5);

    builder.add_jobs_killed({"w0!0"}, {{"w0!0", kill_progress}});
}

// Two single atomic profiles got killed.
void example_jobs_killed_atomic_several_jobs(MessageBuilder & builder)
{
    auto kill_progress1 = KillProgress::make("w0!prof!3");
    kill_progress1->add_atomic("w0!prof!3", "w0!prof", 0.3);

    auto kill_progress2 = KillProgress::make("w0!prof!4");
    kill_progress2->add_atomic("w0!prof!4", "w0!prof", 0.7);

    builder.add_jobs_killed({"w0!3", "w0!4"}, {
        {"w0!3", kill_progress1},
        {"w0!4", kill_progress2}
    });
}

// A sequential profile executing an atomic profil got killed.
void example_jobs_killed_sequential_simple(MessageBuilder & builder)
{
    auto kill_progress = KillProgress::make("w0!prof_seq!0");
    kill_progress->add_atomic("w0!prof!1", "w0!prof", 0.1);
    kill_progress->add_sequential("w0!prof_seq!0", "w0!prof_seq", 1, 2, "w0!prof!1");

    builder.add_jobs_killed({"w0!1"}, {{"w0!1", kill_progress}});
}

// A sequential profile executing another sequential profile got killed.
void example_jobs_killed_sequential_recursive(MessageBuilder & builder)
{
    auto kill_progress = KillProgress::make("w0!prof_seq_seq!0");
    kill_progress->add_atomic("w0!prof!2", "w0!prof", 0.2);
    kill_progress->add_sequential("w0!prof_seq!1", "w0!prof_seq", 1, 2, "w0!prof!2");
    kill_progress->add_sequential("w0!prof_seq_seq!0", "w0!prof_seq_seq", 0, 0, "w0!prof_seq!1");

    builder.add_jobs_killed({"w0!2"}, {{"w0!2", kill_progress}});
}

// A forkjoin profile executing atomic profiles got killed.
void example_jobs_killed_forkjoin_simple(MessageBuilder & builder)
{
    auto kill_progress = KillProgress::make("w0!prof_forkjoin!0");
    kill_progress->add_atomic("w0!prof!5", "w0!prof", 0.4);
    kill_progress->add_atomic("w0!prof2!0", "w0!prof2", 0.8);
    kill_progress->add_forkjoin("w0!prof_forkjoin!0", "w0!prof_forkjoin", {"w0!prof!5", "w0!prof2!0"});

    builder.add_jobs_killed({"w0!5"}, {{"w0!5", kill_progress}});
}

// A forkjoin profile executing another forkjoin profile got killed.
void example_jobs_killed_forkjoin_recursive(MessageBuilder & builder)
{
    auto kill_progress = KillProgress::make("w0!prof_forkjoin2!0");
    kill_progress->add_atomic("w0!prof!6", "w0!prof", 0.9);
    kill_progress->add_forkjoin("w0!prof_forkjoin3!0", "w0!prof_forkjoin3", {"w0!prof!6"});
    kill_progress->add_forkjoin("w0!prof_forkjoin2!0", "w0!prof_forkjoin2", {"w0!prof_forkjoin3!0"});

    builder.add_jobs_killed({"w0!6"}, {{"w0!6", kill_progress}});
}
