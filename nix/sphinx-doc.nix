{ stdenv, lib
, sphinx
, python3Packages
}:

stdenv.mkDerivation rec {
  name = "batprotocol-doc";
  src = lib.sourceByRegex ./.. [
    "^cpp"
    "^cpp/test"
    "^cpp/test/getting-started\.cpp"
    "^cpp/test/example.*\.cpp"
    "^docs"
    "^docs/Makefile"
    "^docs/.*\.rst"
    "^docs/.*\.py"
    "^docs/.*?pp"
    "^docs/expected-output"
    "^docs/expected-output/.*\.json"
  ];
  buildInputs = with python3Packages; [
    sphinx
    sphinx-tabs
    sphinx-rtd-theme
  ];
  buildPhase = "cd docs && make html";
  installPhase = ''
    mkdir -p $out
    mv _build/html/* $out/
  '';
}