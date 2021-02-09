extern crate flatbuffers;

#[allow(dead_code, unused_imports)]
#[path = "../../../gen-rust/schema1_generated.rs"]
#[allow(clippy::approx_constant)]  // We use low precision PI as a default value.
mod batprotocol_generated;
pub use batprotocol_generated::batsim::protocol::{EventUnion,
    HelloEvent, HelloEventArgs,
    ByeEvent, ByeEventArgs,
    Event, EventArgs,
    Message, MessageArgs};

fn main() {
    let mut fbb = flatbuffers::FlatBufferBuilder::new_with_capacity(1024);

    let greeting = fbb.create_string("coucou");
    let hello = HelloEvent::create(&mut fbb, &HelloEventArgs{
        greeting: Some(greeting),
    });
    let bye = ByeEvent::create(&mut fbb, &ByeEventArgs{});
    let hello_event = Event::create(&mut fbb, &EventArgs{
        timestamp: 0.0,
        event_type: EventUnion::hello,
        event: Some(hello.as_union_value()),
    });
    let bye_event = Event::create(&mut fbb, &EventArgs{
        timestamp: 42.0,
        event_type: EventUnion::bye,
        event: Some(bye.as_union_value()),
    });

    let mut events_vec = Vec::new();
    events_vec.push(hello_event);
    events_vec.push(bye_event);
    let events = fbb.create_vector(&events_vec);

    let msg = Message::create(&mut fbb, &MessageArgs{
        now: 51.0,
        events: Some(events),
    });
    fbb.finish(msg, None);

    // The message is now serialized
    let buf = fbb.finished_data();
    println!("Serialized a Message of size={}", buf.len());

    // Deserialize the buffer
    unsafe {
        let msg_copy = flatbuffers::root_unchecked::<Message>(buf);

        // Print its content
        println!("  now={}", msg_copy.now());

        for event in msg_copy.events().unwrap().iter() {
            match event.event_type() {
                EventUnion::hello => {
                    println!("  HELLO {{");
                    println!("    timestamp={}", event.timestamp());
                    println!("    greeting={}", event.event_as_hello().unwrap().greeting().unwrap());
                    println!("  }}")
                },
                EventUnion::bye => {
                    println!("  BYE {{");
                    println!("    timestamp={}", event.timestamp());
                    println!("  }}");
                },
                _ => println!("  unhandled message type"),
            }
        }
    }
}
