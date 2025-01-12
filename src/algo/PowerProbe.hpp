#pragma once

#include <list>
#include <string>
#include <vector>

#include "../isalgorithm.hpp"
#include "../json_workload.hpp"
#include "../locality.hpp"

// Inclure les bibliothèques nécessaires
#include "batprotocol.hpp"
#include <intervalset.hpp>
#include <rapidjson/document.h>  // Inclusion de RapidJSON

#ifdef USE_SLEEPER
#include "sleeper.hpp"  // Si Sleeper est nécessaire à l'initialisation
#endif

using namespace batprotocol;
using namespace rapidjson;

class PowerProbe : public ISchedulingAlgorithm
{
public:
    PowerProbe(Workload * workload, SchedulingDecision * decision, Queue * queue, ResourceSelector * selector,
               double rjms_delay, rapidjson::Document * variant_options);

    virtual ~PowerProbe();

    virtual void on_simulation_start(double date, const rapidjson::Value & batsim_config);

    virtual void on_simulation_end(double date);

    void make_decisions(double date,
                        SortableJobOrder::UpdateInformation * update_info,
                        SortableJobOrder::CompareInformation * compare_info);

    void process_probe_data(double timestamp, const std::vector<double>& energy_data);
    void update_job_decisions();

private:
    // Éléments de gestion des machines et de l'énergie
    IntervalSet all_machines;
    IntervalSet available_machines;
    IntervalSet computing_machines;
    IntervalSet sleeping_machines;

    // Propriétés spécifiques à la gestion d'énergie
    std::vector<double> host_energy;
    double all_hosts_energy;
    double min_power;
    double max_power;
    double last_call_time;
    double epsilon;

    std::list<SchedJob*> * jobs;
    SchedJob * currently_running_job;
    bool probes_running;
    bool all_jobs_submitted;

    // Paramètres spécifiques au comportement et aux délais
    double inter_stop_probe_delay;
    double probe_deadline;
    std::string behavior;

    // Initialisation conditionnelle de Sleeper si USE_SLEEPER est défini
    #ifdef USE_SLEEPER
    Sleeper *sleeper;
    #endif
};

