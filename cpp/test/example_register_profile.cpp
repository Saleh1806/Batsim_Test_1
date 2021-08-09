// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <memory>
#include <vector>

#include <batprotocol.hpp>

using namespace batprotocol;

// Register a delay profile.
void example_register_profile_delay(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_delay(10.0);
    builder.add_register_profile("dyn!delay0", profile);
}

// Register a parallel task profile.
void example_register_profile_ptask(batprotocol::MessageBuilder & builder)
{
    auto compute = std::vector<double>({1e7, 1e7, 1e7, 1e7});
    auto comm = std::vector<double>({
           0, 8192, 8192,    0,
        8192,    0,    0, 8192,
        8192,    0,    0, 8192,
           0, 8192, 8192,    0
    });
    auto profile = Profile::make_parallel_task(
        std::make_shared<std::vector<double> >(std::move(compute)),
        std::make_shared<std::vector<double> >(std::move(comm))
    );

    builder.add_register_profile("dyn!ptask0", profile);
}

// Register a parallel task profile (no communication).
void example_register_profile_ptask_no_comm(batprotocol::MessageBuilder & builder)
{
    auto compute = std::vector<double>({1e7, 1e7, 1e7, 1e7});
    auto profile = Profile::make_parallel_task(
        std::make_shared<std::vector<double> >(std::move(compute)),
        nullptr
    );

    builder.add_register_profile("dyn!ptask1", profile);
}

// Register a parallel task profile (no computation).
void example_register_profile_ptask_no_comp(batprotocol::MessageBuilder & builder)
{
    auto comm = std::vector<double>({
           0, 8192, 8192,    0,
        8192,    0,    0, 8192,
        8192,    0,    0, 8192,
           0, 8192, 8192,    0
    });
    auto profile = Profile::make_parallel_task(
        nullptr,
        std::make_shared<std::vector<double> >(std::move(comm))
    );

    builder.add_register_profile("dyn!ptask2", profile);
}

// Register a parallel task profile (no computation nor communication).
void example_register_profile_ptask_empty(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_parallel_task(
        nullptr,
        nullptr
    );

    builder.add_register_profile("dyn!ptask3", profile);
}

// Register an homogeneous parallel task profile.
void example_register_profile_ptask_hg(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_parallel_task_homogeneous(
        fb::HomogeneousParallelTaskGenerationStrategy_DefinedAmountsUsedForEachValue,
        1e7,
        8192
    );

    builder.add_register_profile("dyn!ptask_hg0", profile);
}

// Register an homogeneous parallel task (with a total amount given) profile.
void example_register_profile_ptask_hg_total(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_parallel_task_homogeneous(
        fb::HomogeneousParallelTaskGenerationStrategy_DefinedAmountsSpreadUniformly,
        1e7,
        8192
    );

    builder.add_register_profile("dyn!ptask_hg1", profile);
}

void example_register_profile_ptask_hg_storage(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_parallel_task_on_storage_homogeneous(
        "pfs",
        fb::HomogeneousParallelTaskGenerationStrategy_DefinedAmountsUsedForEachValue,
        0.0,
        4e6
    );

    builder.add_register_profile("dyn!ptask_hg_storage0", profile);
}

void example_register_profile_ptask_hg_storage_total(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_parallel_task_on_storage_homogeneous(
        "pfs",
        fb::HomogeneousParallelTaskGenerationStrategy_DefinedAmountsSpreadUniformly,
        1e9,
        0.0
    );

    builder.add_register_profile("dyn!ptask_hg_storage1", profile);
}

void example_register_profile_ptask_staging_between_storages(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_parallel_task_data_staging_between_storages(
        1e6,
        "pfs",
        "burst_buffer"
    );

    builder.add_register_profile("dyn!ptask_staging_storages0", profile);
}

void example_register_profile_trace_replay_smpi(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_trace_replay_smpi(
        "npb-lu/traces-smpi.txt"
    );

    builder.add_register_profile("dyn!replay_smpi0", profile);
}

void example_register_profile_trace_replay_fractional_comp(batprotocol::MessageBuilder & builder)
{
    auto profile = Profile::make_trace_replay_fractional_computation(
        "npb-lu/traces-fractional-comp.txt"
    );

    builder.add_register_profile("dyn!replay_frac_comp0", profile);
}

void example_register_profile_sequential(batprotocol::MessageBuilder & builder)
{
    auto sub_profiles = std::vector<std::string>({
        "w0!ptask",
        "dyn!ptask_hg0"
    });
    auto profile = Profile::make_sequential_composition(
        std::make_shared<std::vector<std::string> >(std::move(sub_profiles)),
        3
    );

    builder.add_register_profile("dyn!sequential0", profile);
}

void example_register_profile_forkjoin(batprotocol::MessageBuilder & builder)
{
    auto sub_profiles = std::vector<std::string>({
        "w0!ptask",
        "dyn!ptask_hg0"
    });
    auto profile = Profile::make_forkjoin_composition(
        std::make_shared<std::vector<std::string> >(std::move(sub_profiles))
    );

    builder.add_register_profile("dyn!forkjoin0", profile);
}

void example_register_profile_ptask_merge(batprotocol::MessageBuilder & builder)
{
    auto sub_profiles = std::vector<std::string>({
        "w0!ptask",
        "dyn!ptask_hg0"
    });
    auto profile = Profile::make_parallel_task_merge_composition(
        std::make_shared<std::vector<std::string> >(std::move(sub_profiles))
    );

    builder.add_register_profile("dyn!ptask_merge0", profile);
}
