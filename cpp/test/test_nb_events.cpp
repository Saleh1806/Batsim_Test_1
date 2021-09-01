#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

// Make sure the number of events in a MessageBuilder is consistent.
TEST(nb_events, simple)
{
    using namespace batprotocol;

    MessageBuilder mb;
    EXPECT_EQ(mb.nb_events(), 0);
    EXPECT_EQ(mb.has_events(), false);

    mb.add_all_static_jobs_have_been_submitted();
    EXPECT_EQ(mb.nb_events(), 1);
    EXPECT_EQ(mb.has_events(), true);
    mb.add_all_static_external_events_have_been_injected();
    EXPECT_EQ(mb.nb_events(), 2);
    EXPECT_EQ(mb.has_events(), true);
    mb.finish_message(0.0);
    EXPECT_EQ(mb.nb_events(), 2);
    EXPECT_EQ(mb.has_events(), true);

    mb.clear(0.0);
    EXPECT_EQ(mb.nb_events(), 0);
    EXPECT_EQ(mb.has_events(), false);
    mb.add_simulation_ends();
    EXPECT_EQ(mb.nb_events(), 1);
    EXPECT_EQ(mb.has_events(), true);
    mb.finish_message(0.0);
    EXPECT_EQ(mb.nb_events(), 1);
    EXPECT_EQ(mb.has_events(), true);
}
