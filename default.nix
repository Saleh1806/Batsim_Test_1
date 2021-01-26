{ pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/20.09.tar.gz") {}
}:

let
  jobs = rec {
    inherit pkgs;

    shell = pkgs.mkShell rec {
      buildInputs = [
        pkgs.flatbuffers
        pkgs.ninja
      ];
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
