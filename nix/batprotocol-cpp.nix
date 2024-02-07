{ stdenv, lib
, cppMesonDevBase
, flatbuffers
, meson, ninja, pkg-config
, debug ? false
, doCoverage ? false
, werror ? false
}:

(cppMesonDevBase {
  inherit stdenv lib meson ninja pkg-config debug werror doCoverage;
  coverageGcnoGlob = "libbatprotocol-cpp.so.p/*.gcno";
}).overrideAttrs(attrs: rec {
  name = "batprotocol-cpp";
  src = lib.sourceByRegex ../. [
    "^batprotocol\.fbs"
    "^cpp"
    "^cpp/meson\.build"
    "^cpp/src"
    "^cpp/src/.*\.hpp"
    "^cpp/src/.*\.hpp\.in"
    "^cpp/src/.*\.cpp"
  ];

  propagatedBuildInputs = [
    flatbuffers
  ];

  CXXFLAGS = "${attrs.CXXFLAGS} -Wno-gnu-zero-variadic-macro-arguments";
  preConfigure = "cd cpp";

  passthru = rec {
    GCOV_PREFIX_STRIP = "6";
    DEBUG_SRC_DIRS = [ "${src}/cpp/src" ];
    GDB_DIR_ARGS = map (x: "--directory=" + x) DEBUG_SRC_DIRS;
  };
})
