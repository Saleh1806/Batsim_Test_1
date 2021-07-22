#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

#include "helpers.hpp"

using namespace batprotocol;

void example_reject_job_simple(MessageBuilder &);

TEST(example_reject_job, simple)
{
    MessageBuilder mb(true);
    example_reject_job_simple(mb);
    mb.finish_message(0.0);

    auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
    EXPECT_EQ(parsed->now(), 0.0);
    EXPECT_EQ(parsed->events()->size(), 1u);

    auto event0 = (*parsed->events())[0]->event_as_RejectJobEvent();
    EXPECT_NE(event0, nullptr);
    EXPECT_EQ(event0->job_id()->str(), "w0!0");

    write_test_mb(mb);
}
