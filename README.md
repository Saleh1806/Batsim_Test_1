batprotocol
===========
batprotocol is a set of libraries in various programming languages to help reading and writing messages of the [Batsim protocol](https://batsim.readthedocs.io/en/latest/protocol.html).

Currently supported implementations: C++


Architecture
------------
batprotocol strongly relies on [FlatBuffers](https://flatbuffers.dev/) to serialize and deserialize messages.
The data structure definitions used by batprotocol are available in the [batprotocol.fbs file](./batprotocol.fbs).

The data structure definition file is used by the FlatBuffers compiler to generate low-level de/serialization API in various programming languages. This code is made directly available in the batprotocol library for each supported programming language.

In addition to the low-level API, a higher-level API may be provided by batprotocol depending on the programming language.
Currently, the C++ implementation provides a higher-level API to write messages, but not to read them.


Design rationale
----------------
Here are the criteria that we think are the most important for Batsim simulations and the Batsim protocol

- Portability / Interoperability: Decision-making components should be able to be written in any programming language without modifying Batsim
- Robustness: Malformed messages and invalid decisions should be detected and cause the simulation to fail
- Performance: Simulation is used to reduce time and cost of experimentation. Communication between Batsim and the decision-making component should have small overhead.
- Maintainability: Our manpower is very limited. Our code should not require a lot of regular maintenance work.

We chose to use a serialization library as we think this it provides a better tradeoff of the above criteria than doing it ourselves.

FlatBuffers has been chosen as it is maintained, focuses on performance of binary de/serialization, and since its C++ implementation enables to use serialize and deserialize messages in JSON, which is convenient for users in random languages without direct FlatBuffers support and to log, debug and replay Batsim simulations.


Usage
-----
You can give a look at the examples and tests.
- C++: look at the [./cpp/test](./cpp/test) directory


Packaging and building the software yourself
--------------------------------------------
This repository self provides [Nix](https://nixos.org/) packages (both in optimized and debuggable states) via the [Nix flake](https://nixos.wiki/wiki/Flakes) at the repository root.

You can of course build the project without Nix, but you must in this case make sure that all the required dependencies are usable in your environment.

```sh
# C++. Assuming you are in the 'cpp' directory
meson setup builddir # --prefix=/path/to/install/prefix if you want non-standard install directory
meson compile -C builddir
meson install -C builddir
```
