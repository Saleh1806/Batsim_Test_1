{ stdenv, lib
, findutils, gnused, jq
, batprotocol-cpp, cpp-test-binary
, doCoverage ? false
}:

stdenv.mkDerivation rec {
  name = "cpp-test";

  buildInputs = [
    cpp-test-binary
    findutils
    gnused
    jq
  ];

  unpackPhase = "true"; # no src for this package
  buildPhase = lib.optionalString doCoverage ''
    mkdir -p gcda
    export GCOV_PREFIX=$(realpath gcda)
    export GCOV_PREFIX_STRIP=${batprotocol-cpp.GCOV_PREFIX_STRIP}
  '' + ''
    getting-started
    mkdir output-files
    BATPROTOCOL_TEST_OUTPUT_PATH=output-files batprotocol-cpp-test
  '';
  installPhase = ''
    mkdir -p $out/output-files

    # copy output-files, but indent json ones via jq
    cp output-files/*.bin $out/output-files/
    echo "indenting all JSON output files..."
    find output-files -name '*.json' | sed -E "sW(output-files/(.*))Wcat \1 | jq > $out/output-files/\2W" | bash
  '' + lib.optionalString doCoverage ''
    mv gcda $out/
  '';
}
