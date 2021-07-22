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

  'example_execute_job.cpp',
  'example_reject_job.cpp',

  'test_example_execute_job.cpp',
  'test_example_reject_job.cpp',
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
