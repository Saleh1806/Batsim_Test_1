#pragma once

#include "../isalgorithm.hpp"

#include "../locality.hpp"
#include <intervalset.hpp>

class Workload;
class SchedulingDecision;

class SequencerDVFS : public ISchedulingAlgorithm
{
public:
    SequencerDVFS(Workload * workload, SchedulingDecision * decision, Queue * queue, ResourceSelector * selector,
             double rjms_delay, rapidjson::Document * variant_options);

    virtual ~SequencerDVFS();

    virtual void on_simulation_start(double date, const rapidjson::Value & batsim_config);

    virtual void on_simulation_end(double date);

    virtual void on_requested_call(double date);

    virtual void make_decisions(double date,
                                SortableJobOrder::UpdateInformation * update_info,
                                SortableJobOrder::CompareInformation * compare_info);

private:
    IntervalSet _machines;
    IntervalSet _allocated_machines;
    bool _is_job_running = false;
    double _monitoring_period = -1;

    int _pstate_compute_fast = -1;
    int _pstate_compute_slow = -1;
    bool _currently_fast = true;
};
