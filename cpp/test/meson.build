project('batprotocol-cpp-examples', 'cpp',
  version: '0.1.0',
  license: 'LGPL-3.0',
  meson_version: '>=0.47.0'
)

batprotocol_cpp_dep = dependency('batprotocol-cpp')
flatbuffers_dep = dependency('flatbuffers')
deps = [
  batprotocol_cpp_dep,
  flatbuffers_dep
]

executable('getting-started', ['getting-started.cpp'],
  dependencies: deps,
  install: true
)
