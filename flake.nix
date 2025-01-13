{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=22.11";
    nur-kapack = {
      url = "github:oar-team/nur-kapack/master";
      inputs.nixpkgs.follows = "nixpkgs"; # tell kapack to use the nixpkgs that is defined above
    };
    flake-utils.url = "github:numtide/flake-utils";
    intervalset = {
      url = "git+https://framagit.org/batsim/intervalset";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.nur-kapack.follows = "nur-kapack";
      inputs.flake-utils.follows = "flake-utils";
    };
    batprotocol = {
      url = "git+https://framagit.org/batsim/batprotocol";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.nur-kapack.follows = "nur-kapack";
      inputs.flake-utils.follows = "flake-utils";
    };
  };

  outputs = { self, nixpkgs, nur-kapack, flake-utils, intervalset, batprotocol }:
    flake-utils.lib.eachSystem [ "x86_64-linux" ] (system:
      let
        pkgs = import nixpkgs { inherit system; };
        kapack = nur-kapack.packages.${system};
        release-options = {
          debug = false;
          doCoverage = false;
          intervalset = intervalset.packages-release.${system}.intervalset;
          batprotocol-cpp = batprotocol.packages-release.${system}.batprotocol-cpp;
        };
        debug-options = {
          debug = true;
          doCoverage = false;
          intervalset = intervalset.packages-debug.${system}.intervalset;
          batprotocol-cpp = batprotocol.packages-debug.${system}.batprotocol-cpp;
        };
        debug-cov-options = debug-options // {
          doCoverage = true;
        };
        base-defs = {
          cppMesonDevBase = nur-kapack.lib.${system}.cppMesonDevBase;
          loguru = kapack.loguru;
          redox = kapack.redox;
        };
        callPackage = mergedPkgs: deriv-func: attrset: options: pkgs.lib.callPackageWith(mergedPkgs // options) deriv-func attrset;
      in rec {
        functions = rec {
          batsched = import ./nix/batsched.nix;
          generate-packages = mergedPkgs: options: {
            batsched = callPackage mergedPkgs batsched {} options;
          };
        };
        packages-debug = functions.generate-packages (pkgs // base-defs // packages-debug) debug-options;
        packages-debug-cov = functions.generate-packages (pkgs // base-defs // packages-debug-cov) debug-cov-options;
        packages-release = functions.generate-packages (pkgs // base-defs // packages-release) release-options;
        packages = packages-release // {
          ci-batsched-werror-gcc = callPackage pkgs functions.batsched ({ stdenv = pkgs.gccStdenv; werror = true; } // base-defs) release-options;
          ci-batsched-werror-clang = callPackage pkgs functions.batsched ({ stdenv = pkgs.clangStdenv; werror = true; } // base-defs) release-options;
        };
        devShells = {};
      }
    );
}
