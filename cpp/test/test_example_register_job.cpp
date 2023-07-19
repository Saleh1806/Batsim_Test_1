#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_register_job_simple(MessageBuilder &);

TEST(example_register_job, simple)
{
    MessageBuilder mb(true);
    example_register_job_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RegisterJobEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "dyn!0");
    EXPECT_EQ(event0->job()->walltime(), 600.0);
    EXPECT_EQ(event0->job()->resource_request(), 8);
    EXPECT_EQ(event0->job()->profile_id()->str(), "w0!prof");
    EXPECT_EQ(event0->job()->rigid(), false);
    EXPECT_EQ(event0->job()->extra_data()->str(), "");

    write_test_mb(mb);
}
