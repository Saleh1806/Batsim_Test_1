{ stdenv, lib
, cppMesonDevBase
, gtest, batprotocol-cpp
, meson, ninja, pkgconfig
, debug ? false
, werror ? false
}:

(cppMesonDevBase {
  inherit stdenv lib meson ninja pkgconfig debug werror;
  doCoverage = false;
}).overrideAttrs(attrs: rec {
  name = "cpp-test-binary";
  src = lib.sourceByRegex ../cpp/test [
    "^meson\.build"
    "^.*?pp"
  ];

  nativeBuildInputs = attrs.nativeBuildInputs ++ [
    gtest.dev
  ];
  buildInputs = [
    batprotocol-cpp
  ];

  passthru = rec {
    DEBUG_SRC_DIRS = batprotocol-cpp.DEBUG_SRC_DIRS ++ [ src ];
    GDB_DIR_ARGS = map (x: "--directory=" + x) DEBUG_SRC_DIRS;
  };
})
