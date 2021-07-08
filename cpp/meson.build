project('batprotocol-cpp', 'cpp',
  version: '0.1.0',
  license: 'LGPL-3.0',
  meson_version: '>=0.47.0'
)

flatc = find_program('flatc', required: true)
flatc_gen_cpp_cmd = run_command(flatc, '--cpp', '-o', meson.build_root(), '../batprotocol.fbs')
if flatc_gen_cpp_cmd.returncode() != 0
  error('flatbuffers compiler (flatc) failed with the following output.\n' + flatc_gen_cpp_cmd.stdout().strip())
endif

flatbuffers_dep = dependency('flatbuffers')
deps = [
  flatbuffers_dep
]

src = [
  'src/execute_job.cpp',
  'src/job.cpp',
  'src/kill.cpp',
  'src/message_builder.cpp',
  'src/profile.cpp',
  'src/simulation_begins.cpp',
  'src/time_specifier.cpp'
]

inc = [
  'src/assert.hpp',
  'src/execute_job.hpp',
  'src/job.hpp',
  'src/kill.hpp',
  'src/message_builder.hpp',
  'src/profile.hpp',
  'src/simulation_begins.hpp',
  'src/time_specifier.hpp',
  meson.build_root() + '/batprotocol_generated.h'
]

include_dirs = include_directories('src')

batprotocollib = library('batprotocol-cpp', src + inc,
  include_directories: include_dirs,
  dependencies: deps,
  install: true
)

install_headers(inc, subdir:'batprotocol-cpp')
install_headers('src/batprotocol.hpp')

# Generate a pkg-config file for the library.
pkgc = import('pkgconfig')
pkgc.generate(name: 'batprotocol-cpp',
  libraries: batprotocollib,
  version: meson.project_version(),
  requires_private: 'flatbuffers>=2.0.0',
  description: 'Batsim Protocol C++ Library.'
)
