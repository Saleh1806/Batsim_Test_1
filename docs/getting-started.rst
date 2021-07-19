Getting started
===============

Batprotocol is a set of libraries in various languages that helps in manipulating Batsim_'s protocol.
In particular, it enables Batsim clients (typically scheduling algorithms) to serialize messages with a high-level API, and to parse messages coming from Batsim.

Serialization usage example
---------------------------

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/getting-started.cpp
         :language: cpp

Scheduler example (FCFS)
------------------------

.. todo::

    Write an FCFS example using the library.

.. _Batsim: https://batsim.readthedocs.io/en/latest/
