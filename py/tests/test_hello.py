#!/usr/bin/env python
import flatbuffers

import Batprotocol.ByeEvent as ByeEvent
import Batprotocol.Event as Event
import Batprotocol.EventUnion as EventUnion
import Batprotocol.HelloEvent as HelloEvent
import Batprotocol.Message as Message

def test_hello():
    # Manually create a Message
    fbb = flatbuffers.Builder(1024)
    greeting = fbb.CreateString("coucou")

    HelloEvent.HelloEventStart(fbb)
    HelloEvent.HelloEventAddGreeting(fbb, greeting)
    hello = HelloEvent.HelloEventEnd(fbb)

    ByeEvent.ByeEventStart(fbb)
    bye = ByeEvent.ByeEventEnd(fbb)

    Event.EventStart(fbb)
    Event.EventAddTimestamp(fbb, 0)
    Event.EventAddEventType(fbb, EventUnion.EventUnion.hello)
    Event.EventAddEvent(fbb, hello)
    hello_event = Event.EventEnd(fbb)

    Event.EventStart(fbb)
    Event.EventAddTimestamp(fbb, 42)
    Event.EventAddEventType(fbb, EventUnion.EventUnion.bye)
    Event.EventAddEvent(fbb, bye)
    bye_event = Event.EventEnd(fbb)

    Message.MessageStartEventsVector(fbb, 2)
    # note the reversed push order
    fbb.PrependUOffsetTRelative(bye_event)
    fbb.PrependUOffsetTRelative(hello_event)
    events = fbb.EndVector(2)

    Message.MessageStart(fbb)
    Message.MessageAddNow(fbb, 51)
    Message.MessageAddEvents(fbb, events)
    msg = Message.MessageEnd(fbb)

    fbb.Finish(msg)

    # The message is now serialized
    buf = fbb.Output()
    print('Serialized a Message of size={} {{'.format(len(buf)))

    # Deserialize the buffer
    msg_copy = Message.Message.GetRootAsMessage(buf, 0)

    # Print its content
    print('  now={}'.format(msg_copy.Now()))
    for i in range(msg_copy.EventsLength()):
        event = msg_copy.Events(i)
        print('  Event {')
        print('    timestamp={}'.format(event.Timestamp()))

        if event.EventType() == EventUnion.EventUnion.hello:
            hello = HelloEvent.HelloEvent()
            hello.Init(event.Event().Bytes, event.Event().Pos)
            print('    HELLO {')
            print('      greeting = {}'.format(hello.Greeting()))
        elif event.EventType() == EventUnion.EventUnion.bye:
            print('    BYE')
        else:
            print('    Unsupported type')
        print('  }')
    print('}')
