{ stdenv, lib
, cppMesonDevBase
, flatbuffers
, meson, ninja, pkgconfig
, debug ? false
, doCoverage ? false
, werror ? false
}:

(cppMesonDevBase {
  inherit stdenv lib meson ninja pkgconfig debug werror doCoverage;
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

  preConfigure = "cd cpp";

  passthru = rec {
    GCOV_PREFIX_STRIP = "6";
    DEBUG_SRC_DIRS = [ "${src}/cpp/src" ];
    GDB_DIR_ARGS = map (x: "--directory=" + x) DEBUG_SRC_DIRS;
  };
})
