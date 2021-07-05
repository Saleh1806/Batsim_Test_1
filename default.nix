{ pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/21.05.tar.gz") {}
}:

let
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
        "^cpp/src/.*\.cpp"
      ];
      preConfigure = "cd cpp";
    };

    cpp-test = pkgs.stdenv.mkDerivation rec {
      name = "cpp-test";
      version = "0.1.0";
      nativeBuildInputs = [
        pkgs.meson
        pkgs.ninja
        pkgs.pkgconfig
      ];
      buildInputs = [
        batprotocol-cpp
      ];
      src = pkgs.lib.sourceByRegex ./cpp/tests [
        "^meson\.build"
        "^.*?pp"
      ];
    };

    batprotocol-py = pkgs.python3Packages.buildPythonPackage rec {
      version = "${batprotocol_version}";
      name = "batprotocol-py-${version}";
      nativeBuildInputs = [
        flatbuffers_for_cpp_json
      ];
      propagatedBuildInputs = [
        pkgs.python3Packages.setuptools_scm
        pkgs.python3Packages.pytest
        flatbuffers_python
      ];
      src = pkgs.lib.sourceByRegex ./. [
        "^batprotocol\.fbs"
        "^py"
        "^py/setup\.py"
        "^py/tests"
        "^py/tests/.*\.py"
      ];
      preConfigure = "cd py";
      checkPhase = "python -m pytest tests";
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
  };
in
  jobs
