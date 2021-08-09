#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_external_decision_component_hello_simple(MessageBuilder &);

TEST(example_external_decision_component_hello, simple)
{
    MessageBuilder mb(true);
    example_external_decision_component_hello_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_ExternalDecisionComponentHelloEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->batprotocol_version()->str(), batprotocol::version());
    EXPECT_EQ(event0->decision_component_name()->str(), "cpp-unit-test");
    EXPECT_EQ(event0->decision_component_version()->str(), "0.1.0");
    EXPECT_EQ(event0->decision_component_commit()->str(), "unknown");

    write_test_mb(mb);
}
