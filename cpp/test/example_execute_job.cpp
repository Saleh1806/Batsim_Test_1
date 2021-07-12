// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;
using namespace std;

// Execute a job by just stating on which hosts it should be allocated.
// Default placement policy will be done.
void execute_job_simple(MessageBuilder & builder)
{
    builder.add_execute_job("w0!0", "0-1");
}

// ExecuteJobOptions enables you to customize how the execution should be done.
// Here, a predefined placement strategy is used explicitly.
void execute_job_predefined_placement(MessageBuilder & builder)
{
    // This is the default predefined placement strategy.
    // It will put executor0 on host0, executor1 on host1... and so on.
    // If all hosts are filled this way, executorN will be put on host0, executorN+1 on host1...
    builder.add_execute_job("w0!1", "0-4", ExecuteJobOptions()
        .set_predefined_placement_strategy(fb::PredefinedExecutorPlacementStrategy_SpreadOverHostsFirst)
    );

    // This is another predefined placement strategy.
    // Here, the cores of each host will be filled before traversing all the hosts.
    // executor0 will be on host0, executor1 on host0... until all cores of host0 are filled.
    // Then it will do the same on other hosts, traversing them in order (host1, then host2...).
    builder.add_execute_job("w0!2", "0-4", ExecuteJobOptions()
        .set_predefined_placement_strategy(fb::PredefinedExecutorPlacementStrategy_FillOneHostCoresFirst)
    );
}

// Predefined placement strategies are convenient but do not fit all cases.
// You can use a custom executor to host mapping to create any fancy placement.
void execute_job_custom_placement(MessageBuilder & builder)
{
    // Here, we want to reserve two hosts but only use the first allocated one.
    // As we allocate hosts 13 and 17, this means only host 13 will be used.
    //
    // The number of required resources by the job is important to generate a custom placement.
    // Here, let us assume the job requires 4 resources.
    // Every one of the associated SimGrid executors must be placed.
    shared_ptr<vector<uint32_t> > placement(new vector<uint32_t>({0,0,0,0}));
    builder.add_execute_job("w0!3", "13 17", ExecuteJobOptions()
        .set_custom_placement(placement)
    );

    // Now, in this second example, we want to use the two allocated hosts.
    // The job is still assumed to required 4 resources, so we will place 4 executors.
    // Executor0 and 3 will be on the first allocated host (37).
    // Executor1 and 2 will be on the second allocated host (51).
    placement.reset(new vector<uint32_t>({0,1,1,0}));
    builder.add_execute_job("w0!4", "37 51", ExecuteJobOptions()
        .set_custom_placement(placement)
    );
}

// If the profiles of your job use abstract storage names,
// you can decide which storage to use when executing the job.
void execute_job_storage_mapping(MessageBuilder & builder)
{
    // Let us assume the profile uses a "pfs" abstract storage name.
    // You can decide at runtime that for this job, host 19 should be used as "pfs".
    builder.add_execute_job("w0!5", "0-3 19", ExecuteJobOptions()
        .override_storage_placement("pfs", 19)
    );

    // You can define the placement of several abstract storages if needed.
    builder.add_execute_job("w0!6", "0-3 13 21", ExecuteJobOptions()
        .override_storage_placement("burst_buffer", 13)
        .override_storage_placement("pfs", 19)
    );
}

// If your job uses a composed profiles (which means it has sub-profiles),
// you can define a specific host allocation or executor placement for some sub-profiles.
void execute_job_subprofile_placement(MessageBuilder & builder)
{
    // Let us assume your main profile is a sequential composition profile named "seq",
    // which uses two parallel tasks "ptask0" and "ptask1" as subprofiles.
    //
    // You can decide to let ptask0 use the job host allocation and executor placement,
    // but override it for ptask1 so that it only uses host0.
    shared_ptr<vector<uint32_t> > placement(new vector<uint32_t>({0,0,0,0}));
    builder.add_execute_job("w0!7", "0-3", ExecuteJobOptions()
        .override_profile_alloc_custom_placement("w0!ptask1", "0-3", placement)
    );

    // If you use the same profile several times in your composed profiles,
    // you can give a custom placement for specific ones with the advanced
    // profile syntax "workload!profile!instance".
    // For example, let us assume a "seq2" profile uses "ptask1" then "ptask1".
    // You can override the placement of these two instances separately, so that
    // the first ptask1 will only be on host13,
    // while the second ptask1 will only be on host21.
    //
    // For complex compositions (trees), instances of the same profiles are
    // numbered using an in-order walk starting at 0.
    builder.add_execute_job("w0!8", "13 21", ExecuteJobOptions()
        .override_profile_alloc_custom_placement("w0!ptask1!0", "13", placement)
        .override_profile_alloc_custom_placement("w0!ptask1!1", "21", placement)
    );

    // You can also use a different predefined strategy for some sub-profiles.
    builder.add_execute_job("w0!9", "13 21", ExecuteJobOptions()
        .override_profile_alloc_predefined_placement("w0!ptask1!0", "13 21", fb::PredefinedExecutorPlacementStrategy_FillOneHostCoresFirst)
    );
}
