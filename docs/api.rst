High-level API
==============

Overview
--------

A high-level API is provided to generate messages in the batprotocol.
This API is composed of the following.

- A main ``MessageBuilder`` class that manages a list of events and their serialization.
- Many helper classes to store temporary data for complex events.

Message management API
----------------------

.. tabs::

   .. group-tab:: C++

      Please refer to :ref:`getting_started` for now.

      .. todo::

         Document ``MessageBuilder`` message management functions with doxygen and include them here.


Job and profile management events
---------------------------------

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

Register job
~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_register_job.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_register_job-simple.json
         :language: json

Register profile
~~~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_register_profile.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_register_profile-delay.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_no_comm.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_no_comp.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_empty.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_hg.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_hg_total.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_hg_storage.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_hg_storage_total.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_staging_between_storages.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-trace_replay_smpi.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-trace_replay_fractional_comp.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-sequential.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-forkjoin.json
         :language: json

      .. literalinclude:: expected-output/example_register_profile-ptask_merge.json
         :language: json

Job submitted
~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_job_submitted.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_job_submitted-host.json
         :language: json

      .. literalinclude:: expected-output/example_job_submitted-core.json
         :language: json

      .. literalinclude:: expected-output/example_job_submitted-ghost.json
         :language: json

Job completed
~~~~~~~~~~~~~

.. todo::

   Test and document ``JobCompletedEvent``

Jobs killed
~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_jobs_killed.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_jobs_killed-atomic_simple.json
         :language: json

      .. literalinclude:: expected-output/example_jobs_killed-atomic_several_jobs.json
         :language: json

      .. literalinclude:: expected-output/example_jobs_killed-sequential_simple.json
         :language: json

      .. literalinclude:: expected-output/example_jobs_killed-sequential_recursive.json
         :language: json

      .. literalinclude:: expected-output/example_jobs_killed-forkjoin_simple.json
         :language: json

      .. literalinclude:: expected-output/example_jobs_killed-forkjoin_recursive.json
         :language: json

Resource management events
--------------------------

Create probe
~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_create_probe.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_create_probe-one_shot.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-periodic.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-resource_aggregation_sum.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-resource_aggregation_arithmetic_mean.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-resource_aggregation_median.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-resource_aggregation_min.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-resource_aggregation_max.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-resource_aggregation_quantile_func.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-emission_filtering_threshold.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-accumulate_sum_reset.json
         :language: json

      .. literalinclude:: expected-output/example_create_probe-accumulate_sum_no_reset.json
         :language: json

Stop probe
~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_stop_probe.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_stop_probe-simple.json
         :language: json

Reset probe
~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_reset_probe.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_reset_probe-simple.json
         :language: json

Trigger probe
~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_trigger_probe.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_trigger_probe-force.json
         :language: json

      .. literalinclude:: expected-output/example_trigger_probe-no_force.json
         :language: json

Probe data emitted
~~~~~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_probe_data_emitted.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_probe_data_emitted-aggregated.json
         :language: json

      .. literalinclude:: expected-output/example_probe_data_emitted-vectorial.json
         :language: json

Simulation management events
----------------------------

Call me later
~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_call_me_later.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_call_me_later-one_shot.json
         :language: json

      .. literalinclude:: expected-output/example_call_me_later-periodic.json
         :language: json

      .. literalinclude:: expected-output/example_call_me_later-periodic_finite.json
         :language: json

Stop call me later
~~~~~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_stop_call_me_later.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_stop_call_me_later-simple.json
         :language: json


External decision component hello
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_edc_hello.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_external_decision_component_hello-simple.json
         :language: json

Finish registration
~~~~~~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_finish_registration.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_finish_registration-simple.json
         :language: json

Batsim hello
~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_batsim_hello.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_batsim_hello-simple.json
         :language: json

Simulation begins
~~~~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_simulation_begins.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_simulation_begins-full.json
         :language: json

Simulation ends
~~~~~~~~~~~~~~~

.. todo::

   Test and document ``SimulationEndsEvent``

Requested call
~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_requested_call.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_requested_call-simple.json
         :language: json

All static jobs have been submitted
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_all_static_jobs_submitted.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_all_static_jobs_submitted-simple.json
         :language: json

All static external events have been injected
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. tabs::

   .. group-tab:: C++

      .. literalinclude:: ../cpp/test/example_all_static_external_events_injected.cpp
         :language: cpp

   .. group-tab:: JSON

      .. literalinclude:: expected-output/example_all_static_external_events_injected-simple.json
         :language: json
