{ stdenv, lib
, flatbuffers
, meson, ninja, pkgconfig
, debug ? false
, doCoverage ? false
, werror ? false
}:

stdenv.mkDerivation rec {
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

  nativeBuildInputs = [
    meson
    ninja
    pkgconfig
  ];
  propagatedBuildInputs = [
    flatbuffers
  ];

  preConfigure = "cd cpp";

  mesonBuildType = if debug then "debug" else "release";
  dontStrip = debug;
  CXXFLAGS = if debug then "-O0" else "";
  hardeningDisable = if debug then [ "fortify" ] else [];
  ninjaFlags = [ "-v" ];

  passthru = rec {
    GCOV_PREFIX_STRIP = "6";
    DEBUG_SRC_DIRS = [ "${src}/cpp/src" ];
    GDB_DIR_ARGS = map (x: "--directory=" + x) DEBUG_SRC_DIRS;
  };

  mesonFlags = [ "--warnlevel=3" ]
    ++ lib.optional werror [ "--werror" ]
    ++ lib.optional doCoverage [ "-Db_coverage=true" ];

  postInstall = lib.optionalString doCoverage ''
    mkdir -p $out/gcno
    cp libbatprotocol-cpp.so.p/*.gcno $out/gcno/
  '';
}
