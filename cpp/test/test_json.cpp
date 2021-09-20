#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <batprotocol.hpp>

// Serialize in JSON an empty message
TEST(json, serialize_no_events)
{
    using namespace batprotocol;
    MessageBuilder mb(true);
    mb.finish_message(0.0);
    auto json = mb.buffer_as_json();
    EXPECT_NE(json, nullptr);
    EXPECT_GT(json->size(), 0);
}

// Serialize in JSON a message with events
TEST(json, serialize_events)
{
    using namespace batprotocol;
    MessageBuilder mb(true);
    mb.add_job_completed("w0!0", fb::FinalJobState_COMPLETED_SUCCESSFULLY);
    mb.add_all_static_jobs_have_been_submitted();
    mb.set_current_time(1.0);
    mb.add_simulation_ends();
    mb.finish_message(2.0);
    auto json = mb.buffer_as_json();
    EXPECT_NE(json, nullptr);
    EXPECT_GT(json->size(), 0);
    // printf("%s\n", json->c_str());
}

// Make sure the generated string is null-terminated
TEST(json, serialize_null_terminated)
{
    using namespace batprotocol;
    MessageBuilder mb(true);
    mb.finish_message(0.0);
    auto json = mb.buffer_as_json();
    EXPECT_NE(json, nullptr);
    EXPECT_GT(json->size(), 0);
    EXPECT_EQ(json->c_str()[json->size()], '\0');
}

// Make sure you can generate valid JSON objects with the same MessageBuilder
TEST(json, several_messages)
{
    using namespace batprotocol;
    MessageBuilder mb(true);
    mb.finish_message(0.0);
    auto json0 = mb.buffer_as_json();
    EXPECT_NE(json0, nullptr);
    auto size0 = json0->size();
    EXPECT_GT(size0, 0);

    // should be the exact same message
    mb.clear(0.0);
    mb.finish_message(0.0);
    auto json1 = mb.buffer_as_json();
    EXPECT_NE(json1, nullptr);
    auto size1 = json1->size();
    EXPECT_GT(size1, 0);
    EXPECT_EQ(size1, size0);

    // should be longer, as 123456.0 is longer to serialize than 0.0 in json.
    mb.clear(123456.0);
    mb.finish_message(123456.0);
    auto json2 = mb.buffer_as_json();
    EXPECT_NE(json2, nullptr);
    auto size2 = json2->size();
    EXPECT_GT(size2, 0);
    EXPECT_GT(size2, size1);
}

// Try to serialize an unfinished JSON message
TEST(json, INVALID_serialize_unfinished)
{
    using namespace batprotocol;
    MessageBuilder mb(true);

    EXPECT_THROW({
        try {
            mb.buffer_as_json();
        }
        catch (const batprotocol::Error & e) {
            EXPECT_THAT(e.what(), testing::MatchesRegex(R"(^Cannot call buffer_as_json\(\) while buffer is not finished.*)"));
            throw;
        }
        catch(...) {
            FAIL() << "Expected batprotocol::Error";
        }
    }, batprotocol::Error);
}

// Try to serialize a message while JSON is disabled
TEST(json, INVALID_serialize_json_disabled)
{
    using namespace batprotocol;
    MessageBuilder mb(false);
    mb.finish_message(0.0);

    EXPECT_THROW({
        try {
            mb.buffer_as_json();
        }
        catch (const batprotocol::Error & e) {
            EXPECT_THAT(e.what(), testing::MatchesRegex(R"(^Cannot call buffer_as_json\(\) while json is disabled.*)"));
            throw;
        }
        catch(...) {
            FAIL() << "Expected batprotocol::Error";
        }
    }, batprotocol::Error);
}

// Deserialize a message in JSON
TEST(json, deserialize)
{
    const char * json_msg = R"(
{
  "now": 2.0,
  "events": [
    {
      "timestamp": 0.0,
      "event_type": "JobCompletedEvent",
      "event": {
        "job_id": "w0!0",
        "state": "COMPLETED_SUCCESSFULLY",
        "return_code": 0
      }
    },
    {
      "timestamp": 0.0,
      "event_type": "AllStaticJobsHaveBeenSubmittedEvent",
      "event": {
      }
    },
    {
      "timestamp": 1.0,
      "event_type": "SimulationEndsEvent",
      "event": {
      }
    }
  ]
}
)";

    using namespace batprotocol;
    MessageBuilder mb(true);

    uint8_t * buffer_pointer;
    mb.parse_json_message(std::string(json_msg), buffer_pointer);
    auto parsed = flatbuffers::GetRoot<fb::Message>(buffer_pointer);
    EXPECT_EQ(parsed->now(), 2.0);
    EXPECT_NE(parsed->events(), nullptr);
    EXPECT_EQ(parsed->events()->size(), 3);

    EXPECT_EQ((*parsed->events())[0]->timestamp(), 0.0);
    EXPECT_NE((*parsed->events())[0]->event_as_JobCompletedEvent(), nullptr);
    auto event0 = (*parsed->events())[0]->event_as_JobCompletedEvent();
    EXPECT_EQ(event0->job_id()->str(), "w0!0");
    EXPECT_EQ(event0->state(), fb::FinalJobState_COMPLETED_SUCCESSFULLY);
    EXPECT_EQ(event0->return_code(), 0);

    EXPECT_EQ((*parsed->events())[1]->timestamp(), 0.0);
    EXPECT_NE((*parsed->events())[1]->event_as_AllStaticJobsHaveBeenSubmittedEvent(), nullptr);

    EXPECT_EQ((*parsed->events())[2]->timestamp(), 1.0);
    EXPECT_NE((*parsed->events())[2]->event_as_SimulationEndsEvent(), nullptr);
}

// Try to deserialize a message in JSON while JSON is disabled
TEST(json, INVALID_deserialize_json_disabled)
{
    const char * json_msg = R"({})";

    using namespace batprotocol;
    MessageBuilder mb(false);

    EXPECT_THROW({
        try {
            uint8_t * buffer_pointer;
            mb.parse_json_message(json_msg, buffer_pointer);
        }
        catch (const batprotocol::Error & e) {
            EXPECT_THAT(e.what(), testing::MatchesRegex(R"(^Cannot call buffer_as_json\(\) while json is disabled.*)"));
            throw;
        }
        catch(...) {
            FAIL() << "Expected batprotocol::Error";
        }
    }, batprotocol::Error);
}

// Try to deserialize an invalid (not JSON) message in JSON
TEST(json, INVALID_deserialize_not_json)
{
    const char * json_msg = R"(hi!)";

    using namespace batprotocol;
    MessageBuilder mb(true);

    EXPECT_THROW({
        try {
            uint8_t * buffer_pointer;
            mb.parse_json_message(json_msg, buffer_pointer);
        }
        catch (const batprotocol::Error & e) {
            EXPECT_THAT(e.what(), testing::MatchesRegex(R"(^Could not parse the provided json message with batprotocol's flatbuffers schema: .* declaration expected$)"));
            throw;
        }
        catch(...) {
            FAIL() << "Expected batprotocol::Error";
        }
    }, batprotocol::Error);
}

// Try to deserialize an invalid (a mandatory field is missing) message in JSON
TEST(json, INVALID_deserialize_missing_field)
{
    const char * json_msg = R"(
{
  "now": 2.0,
  "events": [
    {
      "timestamp": 0.0,
      "event_type": "JobCompletedEvent",
      "event": {
        "state": "COMPLETED_SUCCESSFULLY",
        "return_code": 0
      }
    }
  ]
}
)";

    using namespace batprotocol;
    MessageBuilder mb(true);

    EXPECT_THROW({
        try {
            uint8_t * buffer_pointer;
            mb.parse_json_message(json_msg, buffer_pointer);
        }
        catch (const batprotocol::Error & e) {
            EXPECT_THAT(e.what(), testing::MatchesRegex(R"(^Could not parse the provided json message with batprotocol's flatbuffers schema: .* required field is missing: job_id in JobCompletedEvent$)"));
            throw;
        }
        catch(...) {
            FAIL() << "Expected batprotocol::Error";
        }
    }, batprotocol::Error);
}

// Try to deserialize an invalid (an unknown field is present) message in JSON
TEST(json, INVALID_deserialize_unknown_field)
{
    const char * json_msg = R"({"unexpected-field": 10})";

    using namespace batprotocol;
    MessageBuilder mb(true);

    EXPECT_THROW({
        try {
            uint8_t * buffer_pointer;
            mb.parse_json_message(json_msg, buffer_pointer);
        }
        catch (const batprotocol::Error & e) {
            EXPECT_THAT(e.what(), testing::MatchesRegex(R"(^Could not parse the provided json message with batprotocol's flatbuffers schema: .* unknown field: unexpected-field$)"));
            throw;
        }
        catch(...) {
            FAIL() << "Expected batprotocol::Error";
        }
    }, batprotocol::Error);
}
