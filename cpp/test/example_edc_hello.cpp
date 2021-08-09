// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>
#include <batprotocol.hpp>

// An external decision component says hello!
void example_external_decision_component_hello_simple(batprotocol::MessageBuilder & builder)
{
    builder.add_external_decision_component_hello("cpp-unit-test", "0.1.0", "unknown");
}
