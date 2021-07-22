High-level API
==============

.. todo::

    Document high-level API.

MessageBuilder
--------------

This is the main class to serialize messages.

Execute job
~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_execute_job.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_execute_job-simple.json
         :language: json

      .. literalinclude:: expected-output/example_execute_job-predefined_placement.json
         :language: json

      .. literalinclude:: expected-output/example_execute_job-custom_placement.json
         :language: json

      .. literalinclude:: expected-output/example_execute_job-storage_mapping.json
         :language: json

      .. literalinclude:: expected-output/example_execute_job-subprofile_placement.json
         :language: json

Reject job
~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_reject_job.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_reject_job-simple.json
         :language: json

Kill jobs
~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_kill_jobs.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_kill_jobs-simple.json
         :language: json

      .. literalinclude:: expected-output/example_kill_jobs-multiple.json
         :language: json
