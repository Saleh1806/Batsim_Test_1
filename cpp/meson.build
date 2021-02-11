project('batprotocol-cpp', 'cpp',
    version: '0.1.0',
    license: 'LGPL-3.0',
    meson_version: '>=0.47.0'
)

flatc = find_program('flatc', required: true)
flatc_gen_cpp_cmd = run_command(flatc, '--cpp', '-o', meson.build_root(), '../batprotocol.fbs', check: true)

install_headers(meson.build_root() + '/batprotocol_generated.h')

# Generate a pkg-config file for the library.
pkgc = import('pkgconfig')
pkgc.generate(name: 'batprotocol-cpp',
    version: meson.project_version(),
    requires_private: 'flatbuffers>=1.12.0',
    description: 'Batsim Protocol C++ Library.'
)
