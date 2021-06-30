#include "time_specifier.hpp"

namespace batprotocol
{

std::shared_ptr<TimeSpecifier> TimeSpecifier::make_one_time(double time_point)
{
    std::shared_ptr<TimeSpecifier> time_specifier(new TimeSpecifier());
    time_specifier->_type = fb::TimeSpecifierUnion_OneTime;
    time_specifier->_time_point = time_point;

    return time_specifier;
}

std::shared_ptr<TimeSpecifier> TimeSpecifier::make_periodic(double start_time_point, double period)
{
    std::shared_ptr<TimeSpecifier> time_specifier(new TimeSpecifier());
    time_specifier->_type = fb::TimeSpecifierUnion_Periodic;
    time_specifier->_time_point = start_time_point;
    time_specifier->_period = period;

    return time_specifier;
}

} // end of namespace batprotocol
