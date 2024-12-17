#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

// Empty messages (without events). Only 'now' varies.
TEST(timestamp, no_event)
{
    using namespace batprotocol;
    MessageBuilder mb;

    const std::vector<double> times = {0.0, 1.0, 42.0, 1.5e9};
    for (const auto & time : times)
    {
        mb.finish_message(time);

        auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
        EXPECT_EQ(parsed->now(), time);
        EXPECT_NE(parsed->events(), nullptr);
        EXPECT_EQ(parsed->events()->size(), 0u);
    }
}

// Messages with events. All events and 'now' have the same value. clear() is used.
TEST(timestamp, events_and_now_similar_clear)
{
    using namespace batprotocol;
    MessageBuilder mb;

    const std::vector<double> times = {0.0, 42.0};
    for (const auto & time : times)
    {
        mb.clear(time);
        mb.add_all_static_jobs_have_been_submitted();
        mb.add_simulation_ends();
        mb.finish_message(time);

        auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
        EXPECT_EQ(parsed->now(), time);
        EXPECT_NE(parsed->events(), nullptr);
        EXPECT_EQ(parsed->events()->size(), 2u);

        EXPECT_EQ((*parsed->events())[0]->timestamp(), time);
        EXPECT_NE((*parsed->events())[0]->event_as_AllStaticJobsHaveBeenSubmittedEvent(), nullptr);

        EXPECT_EQ((*parsed->events())[1]->timestamp(), time);
        EXPECT_NE((*parsed->events())[1]->event_as_SimulationEndsEvent(), nullptr);
    }
}

// Messages with events. All events and 'now' have the same value. set_current_time() is used.
TEST(timestamp, events_and_now_similar_set_current_time)
{
    using namespace batprotocol;
    MessageBuilder mb;

    const std::vector<double> times = {0.0, 42.0};
    for (const auto & time : times)
    {
        mb.clear(0.0);
        mb.set_current_time(time);
        mb.add_all_static_jobs_have_been_submitted();
        mb.add_simulation_ends();
        mb.finish_message(time);

        auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
        EXPECT_EQ(parsed->now(), time);
        EXPECT_NE(parsed->events(), nullptr);
        EXPECT_EQ(parsed->events()->size(), 2u);

        EXPECT_EQ((*parsed->events())[0]->timestamp(), time);
        EXPECT_NE((*parsed->events())[0]->event_as_AllStaticJobsHaveBeenSubmittedEvent(), nullptr);

        EXPECT_EQ((*parsed->events())[1]->timestamp(), time);
        EXPECT_NE((*parsed->events())[1]->event_as_SimulationEndsEvent(), nullptr);
    }
}

// 'now' is strictly after events. events are at the same timestamp.
TEST(timestamp, now_after_similar_events)
{
    using namespace batprotocol;
    MessageBuilder mb;

    const double now = 73.5;
    const std::vector<double> times = {0.0, 42.0};
    for (const auto & time : times)
    {
        mb.clear(time);
        mb.add_all_static_jobs_have_been_submitted();
        mb.add_simulation_ends();
        mb.finish_message(now);

        auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
        EXPECT_EQ(parsed->now(), now);
        EXPECT_NE(parsed->events(), nullptr);
        EXPECT_EQ(parsed->events()->size(), 2u);

        EXPECT_EQ((*parsed->events())[0]->timestamp(), time);
        EXPECT_NE((*parsed->events())[0]->event_as_AllStaticJobsHaveBeenSubmittedEvent(), nullptr);

        EXPECT_EQ((*parsed->events())[1]->timestamp(), time);
        EXPECT_NE((*parsed->events())[1]->event_as_SimulationEndsEvent(), nullptr);
    }
}

// 'now' is strictly after events. events are at strictly increasing events.
TEST(timestamp, now_after_increasing_events)
{
    using namespace batprotocol;
    MessageBuilder mb;

    const double now = 73.5;
    const std::vector<double> times = {0.0, 42.0};
    for (const auto & time : times)
    {
        mb.clear(time);
        mb.add_all_static_jobs_have_been_submitted();
        mb.set_current_time(time+1.0);
        mb.add_simulation_ends();
        mb.finish_message(now);

        auto parsed = flatbuffers::GetRoot<fb::Message>(mb.buffer_pointer());
        EXPECT_EQ(parsed->now(), now);
        EXPECT_NE(parsed->events(), nullptr);
        EXPECT_EQ(parsed->events()->size(), 2u);

        EXPECT_EQ((*parsed->events())[0]->timestamp(), time);
        EXPECT_NE((*parsed->events())[0]->event_as_AllStaticJobsHaveBeenSubmittedEvent(), nullptr);

        EXPECT_EQ((*parsed->events())[1]->timestamp(), time+1.0);
        EXPECT_NE((*parsed->events())[1]->event_as_SimulationEndsEvent(), nullptr);
    }
}

// trying to add events in non increasing time should fail.
TEST(timestamp, INVALID_decreasing_events)
{
    using namespace batprotocol;
    using ::testing::get;
    MessageBuilder mb;

    const std::vector<double> times = {1.0, 42.0};
    for (const auto & time : times)
    {
        mb.clear(time);
        EXPECT_THROW({
            try {
                mb.set_current_time(time-1.0);
            }
            catch (const batprotocol::Error & e) {
                EXPECT_THAT(e.what(), testing::MatchesRegex(R"(Bad call to set_current_time: new time \([0-9]+\) is before current time \([0-9]+\), which is forbidden\. Please add your events in chronological order\. Also make sure that you called clear\(\) with a correct `now` value\.)"));
                throw;
            }
            catch(...) {
                FAIL() << "Expected batprotocol::Error";
            }
        }, batprotocol::Error);
    }
}

// trying to set 'now' before an event or a clear time should fail.
TEST(timestamp, INVALID_now_before_events)
{
    using namespace batprotocol;
    MessageBuilder mb;

    const std::vector<double> times = {1.0, 42.0};
    for (const auto & time : times)
    {
        mb.clear(time);
        EXPECT_THROW({
            try {
                mb.finish_message(time-1.0);
            }
            catch (const batprotocol::Error & e) {
                EXPECT_THAT(e.what(), testing::MatchesRegex(R"(Bad call to finish_message: new time \([0-9]+\) is before current time \([0-9]+\), which is forbidden\. The time at which your message is sent cannot be before the last event you added\. Also make sure that you called clear\(\) with a correct `now` value\.)"));
                throw;
            }
            catch(...) {
                FAIL() << "Expected batprotocol::Error";
            }
        }, batprotocol::Error);
    }
}
