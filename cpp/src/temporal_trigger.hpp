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

        static std::shared_ptr<TemporalTrigger> make_one_time(double time_point);
        static std::shared_ptr<TemporalTrigger> make_periodic(double start_time_point, double period);

    private:
        friend class MessageBuilder;
        TemporalTrigger() = default;

        fb::TemporalTrigger _type;
        double _time_point;
        double _period;
    };
} // end of namespace batprotocol
