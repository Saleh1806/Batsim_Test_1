{ stdenv, lib
, cppMesonDevBase
, meson, ninja, pkgconfig
, intervalset
, boost, cppzmq, gmp, loguru, rapidjson, redox, zeromq
, doInternalTests ? true
, debug ? false
, werror ? false
, doCoverage ? false
}:

(cppMesonDevBase {
  inherit stdenv lib meson ninja pkgconfig debug werror doCoverage;
  coverageGcnoGlob = "batsched.p/*.gcno";
}).overrideAttrs(attrs: rec {
  name = "batsched";

  BOOST_INCLUDEDIR = "${lib.getDev boost}/include";
  BOOST_LIBRARYDIR = "${lib.getLib boost}/lib";

  src = lib.sourceByRegex ../. [
    "^meson\.build"
    "^src"
    "^src/.*\.?pp"
    "^src/algo"
    "^src/algo/.*\.?pp"
    "^src/external"
    "^src/external/.*\.?pp"
  ];

  buildInputs = [
    intervalset

    boost
    cppzmq
    gmp
    loguru
    rapidjson
    redox
    zeromq
  ];

  passthru = rec {
    hasDebugSymbols = debug;
    hasCoverage = doCoverage;
    GCOV_PREFIX_STRIP = "5";
    DEBUG_SRC_DIRS = [ "${src}/src" ] ++ intervalset.DEBUG_SRC_DIRS;
    GDB_DIR_ARGS = map (x: "--directory=" + x) DEBUG_SRC_DIRS;
  };
})
