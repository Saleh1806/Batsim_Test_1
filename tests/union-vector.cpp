#include <stdio.h>
#include <string>

#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>

#include <schema1_generated.h>

void manually_print_buffer(const uint8_t* buf, int buf_size)
{
    using namespace Batsim::Protocol;
    printf("Serialized a Message of size=%d {\n", buf_size);

    // Deserialize the buffer
    auto msg_copy = flatbuffers::GetRoot<Message>(buf);

    // Print its content
    printf("  now=%g\n", msg_copy->now());
    for (auto it = msg_copy->events()->cbegin(); it != msg_copy->events()->cend(); it++) {
        printf("  Event {\n");
        printf("    timestamp=%g\n", it->timestamp());

        switch(it->event_type()) {
            case EventUnion_hello: {
                printf("    HELLO {\n");
                printf("      greeting: '%s'\n", it->event_as_hello()->greeting()->c_str());
                printf("    }\n");
            } break;
            case EventUnion_bye:
                printf("    BYE\n");
                break;
            default:
                printf("    Unsupported type\n");
        }
        printf("  }\n");
    }

    printf("}\n");
}

int main()
{
    using namespace Batsim::Protocol;

    // Manually create a Message
    flatbuffers::FlatBufferBuilder fbb;
    auto greeting = fbb.CreateString("coucou");
    auto hello = CreateHelloEvent(fbb, greeting);
    auto bye = CreateByeEvent(fbb);
    auto hello_event = CreateEvent(fbb, 0.0, EventUnion_hello, hello.Union());
    auto bye_event = CreateEvent(fbb, 42, EventUnion_bye, bye.Union());

    std::vector<flatbuffers::Offset<Batsim::Protocol::Event>> events;
    events.push_back(hello_event);
    events.push_back(bye_event);

    auto msg = CreateMessageDirect(fbb, 51, &events);
    fbb.Finish(msg);

    // The Message is now serialized
    const uint8_t * buf = fbb.GetBufferPointer();
    const int buf_size = fbb.GetSize();
    manually_print_buffer(buf, buf_size);

    /// json parsing
const char * schema = R"raw(namespace Batsim.Protocol;

table HelloEvent { greeting: string; }
table ByeEvent {}

union EventUnion {
  hello: HelloEvent,
  bye: ByeEvent
}

/// Event doc
table Event {
  timestamp: float64;
  event: EventUnion;
}

table Message {
  now: float64;
  events: [Event];
}

root_type Message;
)raw";

    flatbuffers::Parser parser;
    if (!parser.Parse(schema)) {
        printf("Could not parse schema! Aborting.\n");
        fflush(stdout);
        abort();
    }
    printf("\nSchema:\n###\n%s###\n\n", schema);

    std::string generated_json;
    GenerateText(parser, fbb.GetBufferPointer(), &generated_json);
    printf("Manually crafted buffer as JSON:\n%s\n", generated_json.c_str());

const char * json_msg = R"raw(
{
  now: 73.0,
  events: [
    {
      event_type: "hello",
      event: {
        greeting: "slt"
      }
    },
    {
      timestamp: 1.0,
      event_type: "hello",
      event: {
        greeting: "sa"
      }
    },
    {
      timestamp: 2.0,
      event_type: "hello",
      event: {
        greeting: "va"
      }
    },
    {
      timestamp: 3.0,
      event_type: "hello",
      event: {
        greeting: "???"
      }
    },
    {
      timestamp: 51.0,
      event_type: "bye",
      event: {
      }
    }
  ]
}
)raw";

    if (!parser.Parse(json_msg)) {
        printf("Could not parse json message. Aborting.\n");
        fflush(stdout);
        abort();
    }
    manually_print_buffer(parser.builder_.GetBufferPointer(),
        parser.builder_.GetSize());
    return 0;
}
