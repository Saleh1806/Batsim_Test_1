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

        static std::shared_ptr<TemporalTrigger> make_one_shot(uint64_t time);
        static std::shared_ptr<TemporalTrigger> make_periodic(uint64_t period);
        static std::shared_ptr<TemporalTrigger> make_periodic_finite(uint64_t period, uint32_t nb_periods);

        void set_time_unit(fb::TimeUnit unit);
        void set_offset(uint64_t offset);

    private:
        friend class MessageBuilder;
        TemporalTrigger() = default;

        fb::TemporalTrigger _type;
        uint64_t _time;
        uint64_t _period;
        uint64_t _offset = 0;
        fb::TimeUnit _time_unit = fb::TimeUnit_Second;
        bool _is_finite = false;
        uint32_t _nb_periods = 0;
    };
} // end of namespace batprotocol
