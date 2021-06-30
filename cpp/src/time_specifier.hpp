#pragma once

#include <memory>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;

    class TimeSpecifier
    {
    public:
        ~TimeSpecifier() = default;

        static std::shared_ptr<TimeSpecifier> make_one_time(double time_point);
        static std::shared_ptr<TimeSpecifier> make_periodic(double start_time_point, double period);

    private:
        friend class MessageBuilder;
        TimeSpecifier() = default;

        fb::TimeSpecifierUnion _type;
        double _time_point;
        double _period;
    };
} // end of namespace batprotocol
