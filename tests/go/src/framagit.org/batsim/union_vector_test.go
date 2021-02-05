package Protocol

import (
	"fmt"
	"testing"
	flatbuffers "github.com/google/flatbuffers/go"
)

func TestHello(t *testing.T) {
	fbb := flatbuffers.NewBuilder(1024)
	greeting := fbb.CreateString("coucou")

	HelloEventStart(fbb)
	HelloEventAddGreeting(fbb, greeting)
	hello := HelloEventEnd(fbb)

	ByeEventStart(fbb)
	bye := ByeEventEnd(fbb)

	EventStart(fbb)
	EventAddTimestamp(fbb, 0)
	EventAddEventType(fbb, EventUnionhello)
	EventAddEvent(fbb, hello)
	hello_event := EventEnd(fbb)

	EventStart(fbb)
	EventAddTimestamp(fbb, 42)
	EventAddEventType(fbb, EventUnionbye)
	EventAddEvent(fbb, bye)
	bye_event := ByeEventEnd(fbb)

	MessageStartEventsVector(fbb, 2)
	fbb.PrependUOffsetT(bye_event)
	fbb.PrependUOffsetT(hello_event)
	events := fbb.EndVector(2)

	MessageStart(fbb)
	MessageAddNow(fbb, 51)
	MessageAddEvents(fbb, events)
	msg := MessageEnd(fbb)
	fbb.Finish(msg)

	// The message is now serialized
	buf := fbb.FinishedBytes()
	fmt.Printf("Serialized a Message of size=%+v\n", len(buf))

	// Deserialize the buffer
	msg_copy := GetRootAsMessage(buf, 0)

	// Print its content
	fmt.Printf("  now=%v\n", msg_copy.Now())
	for i := 0; i < msg_copy.EventsLength(); i++ {
		event := new(Event)
		if msg_copy.Events(event, i) {
			fmt.Printf("  Event {\n")
			fmt.Printf("    timestamp=%v\n", event.Timestamp())

			union_table := new(flatbuffers.Table)
			if event.Event(union_table) {
				union_type := event.EventType()

				if union_type == EventUnionhello {
					union_event := new(HelloEvent)
					union_event.Init(union_table.Bytes, union_table.Pos)
					fmt.Printf("    HELLO {\n")
					fmt.Printf("      greeting=%s\n", union_event.Greeting())
					fmt.Printf("    }\n")
				} else if union_type == EventUnionbye {
					fmt.Printf("    BYE\n")
				}

			}

			fmt.Printf("  }\n")
		}
	}
}
