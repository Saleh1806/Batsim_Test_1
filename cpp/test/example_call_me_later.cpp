// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

using namespace batprotocol;

// Call me later, only once.
void example_call_me_later_one_shot(batprotocol::MessageBuilder & builder)
{
    auto when = TemporalTrigger::make_one_shot(42);
    builder.add_call_me_later("example_one_shot", when);
}

// Call me later, periodically (infinitely, every 10 seconds).
void example_call_me_later_periodic_10s(batprotocol::MessageBuilder & builder)
{
    auto when = TemporalTrigger::make_periodic(10);
    builder.add_call_me_later("example_infinite_period", when);
}

// Call me later, periodically (infinitely, every 500 milliseconds offset by 27 milliseconds).
void example_call_me_later_periodic_500ms(batprotocol::MessageBuilder & builder)
{
    auto when = TemporalTrigger::make_periodic(500);
    when->set_time_unit(batprotocol::fb::TimeUnit_Millisecond);
    when->set_offset(27);
    builder.add_call_me_later("example_infinite_period", when);
}

// Call me later, periodically (only 5 times).
void example_call_me_later_periodic_finite(batprotocol::MessageBuilder & builder)
{
    auto when = TemporalTrigger::make_periodic_finite(20, 5);
    builder.add_call_me_later("example_finite_period", when);
}
