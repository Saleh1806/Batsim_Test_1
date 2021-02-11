{ pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/20.09.tar.gz") {}
}:

let
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
      version = "0.1.0";
      nativeBuildInputs = [
        flatbuffers_for_cpp_json
        pkgs.meson
        pkgs.ninja
      ];
      propagatedBuildInputs = [
        flatbuffers_for_cpp_json
      ];
      src = pkgs.lib.sourceByRegex ./. [
        "^batprotocol\.fbs"
        "^cpp"
        "^cpp/meson\.build"
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
      version = "0.1.0";
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

    flatbuffers_for_cpp_json = pkgs.flatbuffers.overrideAttrs(attrs: {
      patches = [
        # Enables version retrieval when .git is absent.
        (pkgs.fetchpatch {
          url = "https://github.com/google/flatbuffers/commit/7e4124d6e6ccafb267f80f3e57e3780913d5cbe5.patch";
          sha256 = "sha256:0h74bra1dlhy5ml9axjmmbq4b9kj6d43rf7rxbwm1ww35k0ajhzl";
        })
        (pkgs.fetchpatch {
          # Generates pkg-config files.
          url = "https://github.com/mpoquet/flatbuffers/commit/322a90d0819f9dff0a441b229fa4012a7383bbcb.patch";
          sha256 = "sha256:01783vidf3lm2kpm7arr71s7pc8mq6czqip6k64ry18658dgyypx";
        })
      ];
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
