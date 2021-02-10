{ pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/20.09.tar.gz") {}
}:

let
  jobs = rec {
    inherit pkgs;

    shell = pkgs.mkShell rec {
      buildInputs = [
        flatbuffers_for_cpp_json
        pkgs.ninja
      ];
      FLATBUFFERS_LIB_PATH="${flatbuffers_for_cpp_json}/lib";
    };

    py-shell = pkgs.mkShell rec {
      buildInputs = [
        pkgs.python3Packages.ipython
        flatbuffers_python
      ];
      shellHook = ''
        export PYTHONPATH=$PYTHONPATH:$(realpath ./gen-py)
        ipython
      '';
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
      #patches = [];
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
