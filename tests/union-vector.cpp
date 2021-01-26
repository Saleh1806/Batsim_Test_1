#include <stdio.h>
#include <flatbuffers/flatbuffers.h>

#include <schema1_generated.h>

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
                printf("    Unsupported: \n");
        }
        printf("  }\n");
    }

    printf("}\n");

    return 0;
}
