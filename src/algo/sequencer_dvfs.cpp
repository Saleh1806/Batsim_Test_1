#include "sequencer_dvfs.hpp"

#include <loguru.hpp>

#include "../pempek_assert.hpp"

SequencerDVFS::SequencerDVFS(Workload *workload,
                   SchedulingDecision *decision,
                   Queue *queue,
                   ResourceSelector *selector,
                   double rjms_delay,
                   rapidjson::Document *variant_options) :
    ISchedulingAlgorithm(workload, decision, queue, selector, rjms_delay, variant_options)
{
    PPK_ASSERT_ERROR(variant_options->HasMember("monitoring_period"), "invalid options: 'monitoring_period' is missing");
    PPK_ASSERT_ERROR((*variant_options)["monitoring_period"].IsNumber(), "invalid options: 'monitoring_period' is not a number");
    _monitoring_period = (*variant_options)["monitoring_period"].GetDouble();
    PPK_ASSERT_ERROR(_monitoring_period > 0, "invalid options: 'monitoring period' should be strictly positive but got %g", _monitoring_period);

    PPK_ASSERT_ERROR(variant_options->HasMember("pstate_compute_fast"), "invalid options: 'pstate_compute_fast' is missing");
    PPK_ASSERT_ERROR((*variant_options)["pstate_compute_fast"].IsNumber(), "invalid options: 'pstate_compute_fast' is not a number");
    _pstate_compute_fast = (*variant_options)["pstate_compute_fast"].GetInt();
    PPK_ASSERT_ERROR(_pstate_compute_fast >= 0, "invalid options: 'pstate_compute_fast period' should be positive but got %d", _pstate_compute_fast);

    PPK_ASSERT_ERROR(variant_options->HasMember("pstate_compute_slow"), "invalid options: 'pstate_compute_slow' is missing");
    PPK_ASSERT_ERROR((*variant_options)["pstate_compute_slow"].IsNumber(), "invalid options: 'pstate_compute_slow' is not a number");
    _pstate_compute_slow = (*variant_options)["pstate_compute_slow"].GetInt();
    PPK_ASSERT_ERROR(_pstate_compute_slow >= 0, "invalid options: 'pstate_compute_slow period' should be positive but got %d", _pstate_compute_slow);

    PPK_ASSERT_ERROR(_pstate_compute_fast != _pstate_compute_slow, "invalid options: 'pstate_compute_fast' and 'pstate_compute_slow' should be different but both have value %d", _pstate_compute_fast);

    LOG_SCOPE_FUNCTION(INFO);
    LOG_F(INFO, "monitoring_period: %g", _monitoring_period);
    LOG_F(INFO, "pstate_compute_fast: %d", _pstate_compute_fast);
    LOG_F(INFO, "pstate_compute_slow: %d", _pstate_compute_slow);
}

SequencerDVFS::~SequencerDVFS()
{

}

void SequencerDVFS::on_simulation_start(double date, const rapidjson::Value &batsim_config)
{
    (void) date;
    (void) batsim_config;

    _machines.insert(IntervalSet::ClosedInterval(0, _nb_machines - 1));
    PPK_ASSERT_ERROR(_machines.size() == (unsigned int) _nb_machines);

    // Request the initial future call. Future ones will be requested in the on_requested_call function.
    _decision->add_call_me_later(date + _monitoring_period, date);
}

void SequencerDVFS::on_simulation_end(double date)
{
}

void SequencerDVFS::on_requested_call(double date)
{
    LOG_SCOPE_FUNCTION(INFO);
    LOG_F(INFO, "requested call received!");

    // Request a future call at current date + period (unless simulation is about to finish)
    if (!_no_more_static_job_to_submit_received || _is_job_running)
    {
        _decision->add_call_me_later(date + _monitoring_period, date);
    }

    // Toggle DVFS state of allocated machines if a job is being run
    if (_is_job_running)
    {
        int new_pstate = _currently_fast ? _pstate_compute_slow : _pstate_compute_fast;
        _decision->add_set_resource_state(_allocated_machines, new_pstate, date);
    }
}

void SequencerDVFS::make_decisions(double date,
                              SortableJobOrder::UpdateInformation *update_info,
                              SortableJobOrder::CompareInformation *compare_info)
{
    // This algorithm executes all the jobs, one after the other.
    // At any time, either 0 or 1 job is running on the platform.
    // The order of the sequence depends on the queue order.

    // Up to one job finished since last call.
    PPK_ASSERT_ERROR(_jobs_ended_recently.size() <= 1);
    if (!_jobs_ended_recently.empty())
    {
        PPK_ASSERT_ERROR(_is_job_running);
        _is_job_running = false;
    }

    // Add valid jobs into the queue
    for (const std::string & job_id : _jobs_released_recently)
    {
        const Job * job = (*_workload)[job_id];

        if (job->nb_requested_resources <= _nb_machines)
            _queue->append_job(job, update_info);
        else
            _decision->add_reject_job(job->id, date);
    }

    // Sort queue if needed
    _queue->sort_queue(update_info, compare_info);

    // Execute the first job if the machine is empty
    const Job * job = _queue->first_job_or_nullptr();
    if (job != nullptr && !_is_job_running)
    {
        _allocated_machines = _machines.left(job->nb_requested_resources);
        _decision->add_execute_job(job->id, _allocated_machines, date);
        _is_job_running = true;
        _queue->remove_job(job);
    }
}
