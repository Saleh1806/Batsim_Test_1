project('batprotocol-cpp-tests', 'cpp',
  version: '0.1.0',
  license: 'LGPL-3.0',
  meson_version: '>=0.47.0',
  default_options : ['cpp_std=c++14'],
)

batprotocol_cpp_dep = dependency('batprotocol-cpp')
flatbuffers_dep = dependency('flatbuffers')
gtest_dep = dependency('gtest_main', version: '>=1.8.0', required: true)
deps = [
  batprotocol_cpp_dep,
  flatbuffers_dep,
  gtest_dep,
]

incdir = include_directories('.')
src = [
  'helpers.hpp',
  'helpers.cpp',

  'example_all_static_external_events_injected.cpp',
  'example_all_static_jobs_submitted.cpp',
  'example_batsim_hello.cpp',
  'example_call_me_later.cpp',
  'example_edc_hello.cpp',
  'example_execute_job.cpp',
  'example_finish_registration.cpp',
  'example_job_submitted.cpp',
  'example_jobs_killed.cpp',
  'example_kill_jobs.cpp',
  'example_register_job.cpp',
  'example_register_profile.cpp',
  'example_reject_job.cpp',
  'example_requested_call.cpp',
  'example_reset_probe.cpp',
  'example_simulation_begins.cpp',
  'example_stop_call_me_later.cpp',
  'example_stop_probe.cpp',

  'test_example_all_static_external_events_injected.cpp',
  'test_example_all_static_jobs_submitted.cpp',
  'test_example_batsim_hello.cpp',
  'test_example_call_me_later.cpp',
  'test_example_edc_hello.cpp',
  'test_example_execute_job.cpp',
  'test_example_finish_registration.cpp',
  'test_example_job_submitted.cpp',
  'test_example_jobs_killed.cpp',
  'test_example_kill_jobs.cpp',
  'test_example_register_job.cpp',
  'test_example_register_profile.cpp',
  'test_example_reject_job.cpp',
  'test_example_requested_call.cpp',
  'test_example_reset_probe.cpp',
  'test_example_simulation_begins.cpp',
  'test_example_stop_call_me_later.cpp',
  'test_example_stop_probe.cpp',
  'test_json.cpp',
  'test_timestamp.cpp',
]
batprotocol_cpp_test = executable('batprotocol-cpp-test', src,
  dependencies: deps,
  include_directories: [incdir],
  install: true
)
test('all-tests', batprotocol_cpp_test)

getting_started = executable('getting-started', ['getting-started.cpp'],
  dependencies: [batprotocol_cpp_dep],
  include_directories: [incdir],
  install: true
)
