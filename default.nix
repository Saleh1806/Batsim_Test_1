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
  };
in
  jobs
