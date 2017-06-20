#include "filler.hpp"

#include "../json_workload.hpp"
#include "../decision.hpp"
#include "../pempek_assert.hpp"

using namespace std;

Filler::Filler(Workload *workload, SchedulingDecision * decision, Queue * queue,
               ResourceSelector *selector, double rjms_delay, rapidjson::Document *variant_options) :
    ISchedulingAlgorithm(workload, decision, queue, selector, rjms_delay, variant_options)
{
    if (variant_options->HasMember("fraction_of_machines_to_use"))
    {
        PPK_ASSERT_ERROR((*variant_options)["fraction_of_machines_to_use"].IsNumber(),
                "Invalid options: 'fraction_of_machines_to_use' should be a number");
        fraction_of_machines_to_use = (*variant_options)["fraction_of_machines_to_use"].GetDouble();
        PPK_ASSERT_ERROR(fraction_of_machines_to_use > 0 && fraction_of_machines_to_use <= 1,
                         "Invalid options: 'fraction_of_machines_to_use' should be in ]0,1] "
                         "but got value=%g", fraction_of_machines_to_use);
    }

    printf("fraction_of_machines_to_use: %g\n", fraction_of_machines_to_use);
}

Filler::~Filler()
{

}

void Filler::on_simulation_start(double date, const rapidjson::Value & batsim_config)
{
    (void) date;
    (void) batsim_config;

    available_machines.insert(MachineRange::ClosedInterval(0, _nb_machines - 1));
    PPK_ASSERT_ERROR(available_machines.size() == (unsigned int) _nb_machines);
}

void Filler::on_simulation_end(double date)
{
    (void) date;
}

void Filler::make_decisions(double date,
                            SortableJobOrder::UpdateInformation *update_info,
                            SortableJobOrder::CompareInformation *compare_info)
{
    // Let's update available machines
    for (const string & ended_job_id : _jobs_ended_recently)
    {
        int nb_available_before = available_machines.size();
        available_machines.insert(current_allocations[ended_job_id]);
        int nb_job_resources = ceil((*_workload)[ended_job_id]->nb_requested_resources * fraction_of_machines_to_use);
        PPK_ASSERT_ERROR(nb_available_before + nb_job_resources == (int)available_machines.size());
        current_allocations.erase(ended_job_id);
    }

    // Let's handle recently released jobs
    for (const string & new_job_id : _jobs_released_recently)
    {
        const Job * new_job = (*_workload)[new_job_id];

        if (new_job->nb_requested_resources > _nb_machines)
            _decision->add_reject_job(new_job_id, date);
        else
            _queue->append_job(new_job, update_info);
    }

    // Queue sorting
    _queue->sort_queue(update_info, compare_info);

    fill(date);
}

void Filler::fill(double date)
{
    if (_debug)
        printf("fill, availableMachines=%s\n", available_machines.to_string_hyphen().c_str());

    int nb_available = available_machines.size();
    for (auto job_it = _queue->begin(); job_it != _queue->end() && nb_available > 0; )
    {
        const Job * job = (*job_it)->job;

        // If it fits I sits (http://knowyourmeme.com/memes/if-it-fits-i-sits)
        MachineRange used_machines;

        if (_selector->fit(job, available_machines, used_machines))
        {
            // Fewer machines might be used that those selected by the fitting algorithm
            int nb_machines_to_allocate = ceil(fraction_of_machines_to_use * job->nb_requested_resources);
            PPK_ASSERT_ERROR(nb_machines_to_allocate > 0 && nb_machines_to_allocate <= job->nb_requested_resources);
            used_machines = used_machines.left(nb_machines_to_allocate);

            vector<int> executor_to_allocated_resource_mapping;
            executor_to_allocated_resource_mapping.resize(job->nb_requested_resources);
            for (int i = 0; i < job->nb_requested_resources; ++i)
                executor_to_allocated_resource_mapping[i] = used_machines[i % nb_machines_to_allocate];

            _decision->add_execute_job(job->id, used_machines, date, executor_to_allocated_resource_mapping);
            current_allocations[job->id] = used_machines;

            available_machines.remove(used_machines);
            PPK_ASSERT_ERROR(nb_available - used_machines.size() == available_machines.size());
            nb_available -= used_machines.size();

            job_it = _queue->remove_job(job);
        }
        else
            job_it++;
    }
}
