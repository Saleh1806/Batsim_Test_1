project('batprotocol-cpp-tests', 'cpp',
  version: '0.1.0',
  license: 'LGPL-3.0',
  meson_version: '>=0.47.0'
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
  'test_timestamp.cpp',
]
batprotocol_cpp_test = executable('batprotocol-cpp-test', src,
  dependencies: deps,
  include_directories: [incdir],
  install: true
)
test('all-tests', batprotocol_cpp_test)
