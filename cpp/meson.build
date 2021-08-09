project('batprotocol-cpp', 'cpp',
  version: '0.1.0',
  license: 'LGPL-3.0',
  meson_version: '>=0.57.0'
)

fs = import('fs')

flatc = find_program('flatc', required: true)
fbs_file = '../batprotocol.fbs'
flatc_gen_cpp_cmd = run_command(flatc, '--cpp', '-o', meson.project_build_root(), fbs_file)
if flatc_gen_cpp_cmd.returncode() != 0
  error('flatbuffers compiler (flatc) failed with the following output.\n' + flatc_gen_cpp_cmd.stdout().strip())
endif

# transform the fbs file into a C++ string
fbs_content = fs.read(fbs_file)
conf_data = configuration_data()
conf_data.set('schema_file_content', fbs_content)
configure_file(
  input: 'src/schema.hpp.in',
  output: 'schema.hpp',
  configuration: conf_data,
)


flatbuffers_dep = dependency('flatbuffers')
deps = [
  flatbuffers_dep,
]

src = [
  'src/create_probe.cpp',
  'src/execute_job.cpp',
  'src/job.cpp',
  'src/kill.cpp',
  'src/message_builder.cpp',
  'src/profile.cpp',
  'src/simulation_begins.cpp',
  'src/temporal_trigger.cpp',
  'src/version.cpp',
]

inc = [
  'src/assert.hpp',
  'src/create_probe.hpp',
  'src/execute_job.hpp',
  'src/job.hpp',
  'src/kill.hpp',
  'src/message_builder.hpp',
  'src/profile.hpp',
  'src/simulation_begins.hpp',
  'src/temporal_trigger.hpp',
  'src/version.hpp',
  meson.project_build_root() + '/batprotocol_generated.h',
  meson.project_build_root() + '/schema.hpp',
]

include_dirs = include_directories('src')

batprotocollib = library('batprotocol-cpp', src + inc,
  include_directories: include_dirs,
  dependencies: deps,
  install: true,
)

install_headers(inc, subdir:'batprotocol-cpp')
install_headers('src/batprotocol.hpp')

# Generate a pkg-config file for the library.
pkgc = import('pkgconfig')
pkgc.generate(name: 'batprotocol-cpp',
  libraries: batprotocollib,
  version: meson.project_version(),
  requires_private: 'flatbuffers>=2.0.0',
  description: 'Batsim Protocol C++ Library.',
)
