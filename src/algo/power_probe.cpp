#include "power_probe.hpp"
#include <stdexcept>
#include <cstdio>
#include <asprintf.h>

// Constructeur
PowerProbe::PowerProbe(Workload * workload, SchedulingDecision * decision, Queue * queue, ResourceSelector * selector,
                       double rjms_delay, rapidjson::Document * variant_options)
    : ISchedulingAlgorithm(workload, decision, queue, selector),
      min_power(95.0), max_power(190.738), last_call_time(-1), epsilon(1e-3),
      all_jobs_submitted(false), probes_running(false), currently_running_job(nullptr),
      all_hosts_energy(0.0), inter_stop_probe_delay(0.0), probe_deadline(500.0), behavior("unset")
{
    host_energy.resize(0);  // Ou la taille appropriée pour les hôtes

    #ifdef USE_SLEEPER
    sleeper = nullptr;  // Initialisation de sleeper si nécessaire
    #endif
}

// Destructeur
PowerProbe::~PowerProbe() {
    if (currently_running_job != nullptr) {
        delete currently_running_job;
    }

    for (auto * job : *jobs) {
        delete job;
    }
}

// Lors du démarrage de la simulation
void PowerProbe::on_simulation_start(double date, const rapidjson::Value & batsim_config) {
    if (batsim_config.HasMember("behavior") && batsim_config["behavior"].IsString()) {
        behavior = batsim_config["behavior"].GetString();
    }

    all_machines = IntervalSet::ClosedInterval(0, platform_nb_hosts - 1);
    available_machines = all_machines;
    host_energy.resize(platform_nb_hosts, 0.0);

    IntervalSet all_hosts = IntervalSet::ClosedInterval(0, platform_nb_hosts - 1);
    auto when = TemporalTrigger::make_periodic(1);
    auto cp = batprotocol::CreateProbe::make_temporal_triggerred(when);
    cp->set_resources_as_hosts(all_hosts.to_string_hyphen());
    cp->enable_accumulation_no_reset();
    mb->add_create_probe("hosts-vec", batprotocol::fb::Metrics_Power, cp);
}

// Lors de la fin de la simulation
void PowerProbe::on_simulation_end(double date) {
    // Traitez la fin de la simulation si nécessaire
}

// Prendre des décisions pour l'ordonnancement
void PowerProbe::make_decisions(double date,
                                SortableJobOrder::UpdateInformation * update_info,
                                SortableJobOrder::CompareInformation * compare_info) {
    if (currently_running_job == nullptr && !jobs->empty()) {
        currently_running_job = jobs->front();
        jobs->pop_front();
        auto hosts = IntervalSet(IntervalSet::ClosedInterval(0, currently_running_job->nb_hosts - 1));
        mb->add_execute_job(currently_running_job->job_id, hosts.to_string_hyphen());
    }

    update_job_decisions();
}

// Traiter les données d'énergie des probes
void PowerProbe::process_probe_data(double timestamp, const std::vector<double>& energy_data) {
    if (energy_data.size() != host_energy.size()) {
        throw std::runtime_error("Mismatch in energy data size.");
    }

    for (size_t i = 0; i < energy_data.size(); ++i) {
        host_energy[i] = energy_data[i];
    }
}

// Mettre à jour les décisions d'ordonnancement des jobs
void PowerProbe::update_job_decisions() {
    if (all_jobs_submitted && !jobs->empty()) {
        currently_running_job = jobs->front();
        jobs->pop_front();
        auto hosts = IntervalSet(IntervalSet::ClosedInterval(0, currently_running_job->nb_hosts - 1));
        mb->add_execute_job(currently_running_job->job_id, hosts.to_string_hyphen());
    }
}
