{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=22.11";
    nur-kapack = {
      url = "github:oar-team/nur-kapack/master";
      inputs.nixpkgs.follows = "nixpkgs"; # tell kapack to use the nixpkgs that is defined above
    };
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, nur-kapack, flake-utils }:
    flake-utils.lib.eachSystem [ "x86_64-linux" ] (system:
      let
        pkgs = import nixpkgs { inherit system; };
        kapack = nur-kapack.packages.${system};
        release-options = {
          debug = false;
          doCoverage = false;
        };
        debug-options = {
          debug = true;
          doCoverage = true;
        };
        callPackage = mergedPkgs: deriv-func: attrset: options: pkgs.lib.callPackageWith(mergedPkgs // options) deriv-func attrset;
      in rec {
        functions = rec {
          batprotocol-cpp = import ./nix/batprotocol-cpp.nix;
          cpp-test-binary = import ./nix/cpp-test-binary.nix;
          cpp-test = import ./nix/cpp-test.nix;
          cpp-coverage-report = import ./nix/cpp-coverage-report.nix;
          generate-packages = mergedPkgs: options: {
            batprotocol-cpp = callPackage mergedPkgs batprotocol-cpp {} options;
            cpp-test-binary = callPackage mergedPkgs cpp-test-binary {} options;
            cpp-test = callPackage mergedPkgs cpp-test {} options;
            cpp-coverage-report = callPackage mergedPkgs cpp-coverage-report {} options;
          };
        };
        packages-debug = functions.generate-packages (pkgs // packages-debug) debug-options;
        packages-release = functions.generate-packages (pkgs // packages-release) release-options;
        packages = packages-release // {
          ci-batprotocol-cpp-werror-gcc = callPackage pkgs functions.batprotocol-cpp { stdenv = pkgs.gccStdenv; werror = true; } release-options;
          ci-batprotocol-cpp-werror-clang = callPackage pkgs functions.batprotocol-cpp { stdenv = pkgs.clangStdenv; werror = true; } release-options;
        };

        devShells = {
          cpp-test = pkgs.mkShell rec {
            buildInputs = with packages-debug; [
              batprotocol-cpp
              cpp-test-binary
            ];
            DEBUG_SRC_DIRS = packages-debug.cpp-test-binary.DEBUG_SRC_DIRS;
            GDB_DIR_ARGS = packages-debug.cpp-test-binary.GDB_DIR_ARGS;
            shellHook = ''
              echo Found debug_info source paths. ${builtins.concatStringsSep ":" DEBUG_SRC_DIRS}
              echo Run the following command to automatically load these directories to gdb.
              echo gdb \$\{GDB_DIR_ARGS\}

              # create directory for gcov output files, and set gcov env vars accordingly
              mkdir -p /tmp/cpp-test-gcda
              export GCOV_PREFIX=/tmp/cpp-test-gcda
              export GCOV_PREFIX_STRIP=${packages-debug.batprotocol-cpp.GCOV_PREFIX_STRIP}
            '';
          };
        };
      }
    );
}


