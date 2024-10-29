#include "temporal_trigger.hpp"

#include "assert.hpp"

namespace batprotocol
{

std::shared_ptr<TemporalTrigger> TemporalTrigger::make_one_shot(uint64_t time)
{
    std::shared_ptr<TemporalTrigger> temporal_trigger(new TemporalTrigger());
    temporal_trigger->_type = fb::TemporalTrigger_OneShot;
    temporal_trigger->_time = time;

    return temporal_trigger;
}

std::shared_ptr<TemporalTrigger> TemporalTrigger::make_periodic(
    uint64_t period)
{
    BAT_ENFORCE(period > 0, "period should be strictly positive (got %ld)", period);

    std::shared_ptr<TemporalTrigger> temporal_trigger(new TemporalTrigger());
    temporal_trigger->_type = fb::TemporalTrigger_Periodic;
    temporal_trigger->_period = period;
    temporal_trigger->_is_finite = false;

    return temporal_trigger;
}

std::shared_ptr<TemporalTrigger> TemporalTrigger::make_periodic_finite(
    uint64_t period,
    uint32_t nb_periods)
{
    BAT_ENFORCE(period > 0, "period should be strictly positive (got %ld)", period);
    BAT_ENFORCE(nb_periods > 0, "nb_periods should not be 0 (use one_shot for this behavior)");

    std::shared_ptr<TemporalTrigger> temporal_trigger(new TemporalTrigger());
    temporal_trigger->_type = fb::TemporalTrigger_Periodic;
    temporal_trigger->_period = period;
    temporal_trigger->_nb_periods = nb_periods;
    temporal_trigger->_is_finite = true;

    return temporal_trigger;
}

void TemporalTrigger::set_time_unit(fb::TimeUnit unit)
{
    _time_unit = unit;
}

void TemporalTrigger::set_offset(uint64_t offset)
{
    _offset = offset;
}


} // end of namespace batprotocol
