#pragma once

#include <list>

#include "../isalgorithm.hpp"
#include "../json_workload.hpp"
#include "../locality.hpp"

class Sleeper : public ISchedulingAlgorithm
{
public:
    Sleeper(Workload * workload, SchedulingDecision * decision, Queue * queue, ResourceSelector * selector,
            double rjms_delay, rapidjson::Document * variant_options);

    virtual ~Sleeper();

    virtual void on_simulation_start(double date);

    virtual void on_simulation_end(double date);

    void make_decisions(double date,
                        SortableJobOrder::UpdateInformation * update_info,
                        SortableJobOrder::CompareInformation * compare_info);


private:
    MachineRange all_machines;

    MachineRange available_machines;
    MachineRange computing_machines;
    MachineRange sleeping_machines;
    MachineRange machines_being_switched_on;
    MachineRange machines_being_switched_off;

    std::string job_being_computed = "";

    int compute_pstate;
    int sleep_pstate;
};
