#include "temporal_trigger.hpp"

#include "assert.hpp"

namespace batprotocol
{

std::shared_ptr<TemporalTrigger> TemporalTrigger::make_one_shot(double time_point)
{
    std::shared_ptr<TemporalTrigger> temporal_trigger(new TemporalTrigger());
    temporal_trigger->_type = fb::TemporalTrigger_OneShot;
    temporal_trigger->_time_point = time_point;

    return temporal_trigger;
}

std::shared_ptr<TemporalTrigger> TemporalTrigger::make_periodic(
    double start_time_point,
    double period)
{
    BAT_ENFORCE(period > 0, "period should be strictly positive (got %g)", period);

    std::shared_ptr<TemporalTrigger> temporal_trigger(new TemporalTrigger());
    temporal_trigger->_type = fb::TemporalTrigger_Periodic;
    temporal_trigger->_time_point = start_time_point;
    temporal_trigger->_period = period;
    temporal_trigger->_is_finite = false;

    return temporal_trigger;
}

std::shared_ptr<TemporalTrigger> TemporalTrigger::make_periodic_finite(
    double start_time_point,
    double period,
    uint32_t nb_periods)
{
    BAT_ENFORCE(period > 0, "period should be strictly positive (got %g)", period);
    BAT_ENFORCE(nb_periods > 0, "nb_periods should not be 0 (use one_shot for this behavior)");

    std::shared_ptr<TemporalTrigger> temporal_trigger(new TemporalTrigger());
    temporal_trigger->_type = fb::TemporalTrigger_Periodic;
    temporal_trigger->_time_point = start_time_point;
    temporal_trigger->_period = period;
    temporal_trigger->_nb_periods = nb_periods;
    temporal_trigger->_is_finite = true;

    return temporal_trigger;
}


} // end of namespace batprotocol
