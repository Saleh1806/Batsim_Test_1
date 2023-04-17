{ stdenv, lib
, gtest, batprotocol-cpp
, meson, ninja, pkgconfig
, debug ? false
}:

stdenv.mkDerivation rec {
  name = "cpp-test-binary";
  src = lib.sourceByRegex ../cpp/test [
    "^meson\.build"
    "^.*?pp"
  ];

  nativeBuildInputs = [
    meson
    ninja
    pkgconfig
    gtest.dev
  ];
  buildInputs = [
    batprotocol-cpp
  ];
  debugInputs = [
    batprotocol-cpp
  ];

  mesonBuildType = if debug then "debug" else "release";
  dontStrip = debug;
  CXXFLAGS = if debug then "-O0" else "";
  hardeningDisable = if debug then [ "fortify" ] else [];
  ninjaFlags = [ "-v" ];

  passthru = rec {
    DEBUG_SRC_DIRS = batprotocol-cpp.DEBUG_SRC_DIRS ++ [ src ];
    GDB_DIR_ARGS = map (x: "--directory=" + x) DEBUG_SRC_DIRS;
  };
}
