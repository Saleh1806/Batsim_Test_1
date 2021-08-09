#pragma once

#include <memory>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;

    class TemporalTrigger
    {
    public:
        ~TemporalTrigger() = default;

        static std::shared_ptr<TemporalTrigger> make_one_shot(double time_point);
        static std::shared_ptr<TemporalTrigger> make_periodic(double start_time_point, double period);
        static std::shared_ptr<TemporalTrigger> make_periodic_finite(double start_time_point, double period, uint32_t nb_periods);

    private:
        friend class MessageBuilder;
        TemporalTrigger() = default;

        fb::TemporalTrigger _type;
        double _time_point;
        double _period;
        bool _is_finite = false;
        uint32_t _nb_periods = 0;
    };
} // end of namespace batprotocol
