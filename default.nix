{ kapack ? import
    (fetchTarball "https://github.com/oar-team/nur-kapack/archive/master.tar.gz") {}
, werror ? false
, doCoverage ? true
, coverageCobertura ? false
, coverageCoveralls ? false
, coverageGcovTxt ? true
, coverageHtml ? true
, coverageSonarqube ? false
}:

let
  pkgs = kapack.pkgs;
  batprotocol_version = "0.1.0";
  jobs = rec {
    inherit pkgs;

    shell = pkgs.mkShell rec {
      buildInputs = [
        flatbuffers_for_cpp_json
        pkgs.ninja
        pkgs.pkgconfig
      ];
      FLATBUFFERS_LIB_PATH="${flatbuffers_for_cpp_json}/lib";
    };

    batprotocol-cpp = pkgs.stdenv.mkDerivation rec {
      name = "batprotocol-cpp";
      version = "${batprotocol_version}";
      nativeBuildInputs = [
        flatbuffers_for_cpp_json
        pkgs.meson
        pkgs.ninja
        pkgs.pkgconfig
      ];
      propagatedBuildInputs = [
        flatbuffers_for_cpp_json
      ];
      src = pkgs.lib.sourceByRegex ./. [
        "^batprotocol\.fbs"
        "^cpp"
        "^cpp/meson\.build"
        "^cpp/src"
        "^cpp/src/.*\.hpp"
        "^cpp/src/.*\.hpp\.in"
        "^cpp/src/.*\.cpp"
      ];
      preConfigure = "cd cpp";
      mesonFlags = [ "--warnlevel=3" ]
        ++ pkgs.lib.optional werror [ "--werror" ]
        ++ pkgs.lib.optional doCoverage [ "-Db_coverage=true" ];
      postInstall = pkgs.lib.optionalString doCoverage ''
        mkdir -p $out/gcno
        cp libbatprotocol-cpp.so.p/*.gcno $out/gcno/
      '';
    };

    cpp-test-binary = pkgs.stdenv.mkDerivation rec {
      name = "cpp-test-binary";
      version = "0.1.0";
      nativeBuildInputs = [
        pkgs.meson
        pkgs.ninja
        pkgs.pkgconfig
        pkgs.gtest.dev
      ];
      buildInputs = [
        batprotocol-cpp
      ];
      src = pkgs.lib.sourceByRegex ./cpp/test [
        "^meson\.build"
        "^.*?pp"
      ];
    };

    cpp-test = pkgs.stdenv.mkDerivation rec {
      name = "cpp-test";
      buildInputs = [ cpp-test-binary ];
      unpackPhase = "true"; # no src for this package
      buildPhase = pkgs.lib.optionalString doCoverage ''
        mkdir -p gcda
        export GCOV_PREFIX=$(realpath gcda)
        export GCOV_PREFIX_STRIP=5
      '' + ''
        getting-started
        mkdir output-files
        BATPROTOCOL_TEST_OUTPUT_PATH=output-files batprotocol-cpp-test
      '';
      installPhase = ''
        mkdir -p $out
        cp -r output-files $out/
      '' + pkgs.lib.optionalString doCoverage ''
        mv gcda $out/
      '';
    };

    cpp-coverage-report = pkgs.stdenv.mkDerivation rec {
      name = "cpp-coverage-report";
      buildInputs = batprotocol-cpp.buildInputs ++ [ kapack.gcovr ]
        ++ [ batprotocol-cpp cpp-test ];
      src = batprotocol-cpp.src;

      buildPhase = ''
        cd cpp
        mkdir cov-merged
        cd cov-merged
        cp ${batprotocol-cpp}/gcno/* ${cpp-test}/gcda/* ./
        gcov -p *.gcno
        mkdir report
      '' + pkgs.lib.optionalString coverageHtml ''
        mkdir -p report/html
      '' + pkgs.lib.optionalString coverageGcovTxt ''
        mkdir -p report/gcov-txt
        cp \^\#src\#*.gcov report/gcov-txt/
      '' + ''
        gcovr -g -k -r .. --filter '\.\./src/' \
          --txt report/file-summary.txt \
          --csv report/file-summary.csv \
          --json-summary report/file-summary.json \
        '' + pkgs.lib.optionalString coverageCobertura ''
          --xml report/cobertura.xml \
        '' + pkgs.lib.optionalString coverageCoveralls ''
          --coveralls report/coveralls.json \
        '' + pkgs.lib.optionalString coverageHtml ''
          --html-details report/html/index.html \
        '' + pkgs.lib.optionalString coverageSonarqube ''
          --sonarqube report/sonarqube.xml \
        '' + ''
          --print-summary
      '';
      installPhase = ''
        mkdir -p $out
        cp -r report/* $out/
      '';
    };

    batprotocol-py = pkgs.python3Packages.buildPythonPackage rec {
      version = "${batprotocol_version}";
      name = "batprotocol-py-${version}";
      nativeBuildInputs = [
        flatbuffers_for_cpp_json
      ];
      propagatedBuildInputs = [
        pkgs.python3Packages.setuptools_scm
        flatbuffers_python
      ];
      src = pkgs.lib.sourceByRegex ./. [
        "^batprotocol\.fbs"
        "^py"
        "^py/setup\.py"
      ];
      preConfigure = "cd py";
    };

    py-test-shell = pkgs.mkShell rec {
      buildInputs = [
        pkgs.python3Packages.ipython
        batprotocol-py
      ];
    };

    batprotocol-rust-src = pkgs.stdenv.mkDerivation rec {
      name = "batprotocol-rust-src";
      version = "${batprotocol_version}";
      src = pkgs.lib.sourceByRegex ./. [
        "^batprotocol\.fbs"
        "^rust"
        "^rust/Cargo\..*"
        "^rust/src"
        "^rust/src/lib\.rs"
        "^rust/tests"
        "^rust/tests/.*\.rs"
      ];
      nativeBuildInputs = [flatbuffers_for_rust];
      buildPhase = ''
        cd rust
        flatc --rust -o src ../batprotocol.fbs
      '';
      installPhase = ''
        mkdir -p $out
        cp -r * $out/
      '';
    };

    batprotocol-rust = pkgs.rustPlatform.buildRustPackage rec {
      name = "batprotocol-rust";
      version = batprotocol-rust-src.version;
      src = batprotocol-rust-src;
      cargoSha256 = "sha256:06ldr2ci896406jbs873a27qhhqrsxw4avi9w5gyykc6jlpx99as";
    };

    go-shell = pkgs.mkShell rec {
      buildInputs = [
        pkgs.go
      ];
      shellHook = ''
        export GOPATH=$(realpath ./tests/go)
      '';
    };

    rust-shell = pkgs.mkShell rec {
      buildInputs = [
        pkgs.cargo
        flatbuffers_for_rust
      ];
    };

    flatbuffers_for_cpp_json = pkgs.flatbuffers.overrideAttrs(attrs: rec {
      version = "2.0.0";
      src = pkgs.fetchFromGitHub {
        owner = "google";
        repo = "flatbuffers";
        rev = "v${version}";
        sha256 = "sha256:1zbf6bdpps8369r1ql00irxrp58jnalycc8jcapb8iqg654vlfz8";
      };
      patches = [];

      nativeBuildInputs = [pkgs.cmake pkgs.pkgconfig];
      cmakeBuildType = "Debug";
      cmakeFlags = [
        "-DFLATBUFFERS_BUILD_FLATLIB=OFF"
        "-DFLATBUFFERS_BUILD_SHAREDLIB=ON"
      ];
      dontStrip = true;
    });

    flatbuffers_for_rust = pkgs.flatbuffers.overrideAttrs(attrs: {
      src = pkgs.fetchgit {
        rev = "8008dde117ef165ef115564d58cb95a7d11ac918";
        url = "https://github.com/google/flatbuffers.git";
        sha256 = "sha256:1dbmidqhsrydfs3xjpv36h379dwi0wwp312mykfcffpp01qrxj8j";
      };
    });

    flatbuffers_python = pkgs.python3Packages.buildPythonPackage {
      name = "flatbuffers-1.12";
      doCheck = false;
      propagatedBuildInputs = [
        pkgs.python3Packages.pytest
        pkgs.python3Packages.setuptools_scm
      ];
      src = builtins.fetchurl {
        url = "https://files.pythonhosted.org/packages/4d/c4/7b995ab9bf0c7eaf10c386d29a03408dfcf72648df4102b1f18896c3aeea/flatbuffers-1.12.tar.gz";
        sha256 = "0426nirqv8wzj56ppk6m0316lvwan8sn28iyj40kfdsy5mr9mfv3";
      };
    };

    sphinx-tabs = pkgs.python3Packages.buildPythonPackage rec {
      pname = "sphinx-tabs";
      version = "3.1.0";
      name = "${pname}-${version}";

      src = pkgs.python3Packages.fetchPypi {
        inherit pname version;
        sha256 = "0kv935qhml40mly33rk5am128g2ygqkfvizh33vf29hjkf32mvjy";
      };

      propagatedBuildInputs = with pkgs.python3Packages; [
        docutils
        pygments
        sphinx
      ];
    };

    sphinx_doc = pkgs.stdenv.mkDerivation rec {
      name = "batprotocol-sphinx-documentation";
      src = pkgs.lib.sourceByRegex ./. [
        "^docs"
        "^docs/conf.py"
        "^docs/Makefile"
        "^docs/.*\.rst"
        "^docs/expected-output"
        "^docs/expected-output/.*\.json"
        "^batprotocol.fbs"
        "^cpp"
        "^cpp/test"
        "^cpp/test/getting-started\.cpp"
        "^cpp/test/example_.*\.cpp"
      ];
      buildInputs = with pkgs.python3Packages; [
        sphinx
        sphinx_rtd_theme
        sphinx-tabs
      ];
      buildPhase = "cd docs && make html";
      installPhase = ''
        mkdir -p $out
        cp -r _build/html $out/
      '';
    };
  };
in
  jobs
