project('batprotocol-cpp-test', 'cpp',
    version: '0.1.0',
    license: 'LGPL-3.0',
    meson_version: '>=0.47.0'
)

batprotocol_cpp_dep = dependency('batprotocol-cpp')
flatbuffers_dep = dependency('flatbuffers')

executable('hello-and-json', ['hello-and-json.cpp'],
    dependencies: [batprotocol_cpp_dep, flatbuffers_dep],
    install: true
)
