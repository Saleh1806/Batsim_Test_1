{ kapack ? import
    (fetchTarball "https://github.com/oar-team/nur-kapack/archive/master.tar.gz")
  {}
, doCheck ? false
, doCoverage ? true
, batsim ? kapack.batsim
, batsim-master ? kapack.batsim-master
, batexpe ? kapack.batexpe
}:

let
  pkgs = kapack.pkgs;
  pythonPackages = pkgs.python37Packages;
  buildPythonPackage = pythonPackages.buildPythonPackage;

  jobs = rec {
    # Batsched executable file (built from local sources)
    batsched = kapack.batsched.overrideAttrs (attr: rec {
      src = pkgs.lib.sourceByRegex ./. [
        "^src"
        "^src/.*\.?pp"
        "^src/algo"
        "^src/algo/.*\.?pp"
        "^src/external"
        "^src/external/.*\.?pp"
        "^meson\.build"
      ];
      mesonFlags = []
        ++ pkgs.lib.optional doCoverage [ "-Db_coverage=true" ];
      nativeBuildInputs = with kapack; [pkgs.meson pkgs.ninja pkgs.pkgconfig
        pkgs.boost pkgs.gmp pkgs.rapidjson intervalset loguru pkgs.cppzmq pkgs.zeromq];
      # Debug build, without any Nix stripping magic.
      mesonBuildType = "debug";
      hardeningDisable = [ "all" ];
      dontStrip = true;
      # Keep files generated by GCOV, so depending jobs can use them.
      postInstall = pkgs.lib.optionalString doCoverage ''
        mkdir -p $out/gcno
        cp batsched.p/*.gcno $out/gcno/
      '';
    });

    # Batsched integration tests.
    # integration_tests = pkgs.stdenv.mkDerivation rec {
    #   pname = "batsched-integration-tests";
    #   version = toString builtins.currentTime; # Forces rebuild
    #   src = pkgs.lib.sourceByRegex ./. [
    #     "^test"
    #     "^test/.*\.py"
    #     "^test/platforms"
    #     "^test/platforms/.*\.xml"
    #     "^test/workloads"
    #     "^test/workloads/.*\.json"
    #   ];
    #   buildInputs = with pkgs.python37Packages; [
    #     batsim batsched batexpe
    #     pytest pytest_html pandas];
    #   preBuild = pkgs.lib.optionalString doCoverage ''
    #     mkdir -p gcda
    #     export GCOV_PREFIX=$(realpath gcda)
    #     export GCOV_PREFIX_STRIP=5
    #   '';
    #   buildPhase = ''
    #     runHook preBuild
    #     set +e
    #     (cd test && pytest -ra --html=../report/pytest_report.html)
    #     echo $? > ./pytest_returncode
    #     set -e
    #   '';
    #   checkPhase = ''
    #     pytest_return_code=$(cat ./pytest_returncode)
    #     echo "pytest return code: $pytest_return_code"
    #     if [ $pytest_return_code -ne 0 ] ; then
    #       exit 1
    #     fi
    #   '';
    #   inherit doCheck;
    #   installPhase = ''
    #     mkdir -p $out
    #     mv ./report/* ./pytest_returncode $out/
    #   '' + pkgs.lib.optionalString doCoverage ''
    #     mv ./gcda $out/
    #   '';
    # };
    # Essentially the same as integration_tests, but with an up-to-date Batsim.
    # integration_tests_batlatest = integration_tests.overrideAttrs (attr: rec {
    #   buildInputs = with pkgs.python37Packages; [
    #     batsim-master batsched batexpe
    #     pytest pytest_html pandas];
    # });

    # Batsched doxygen documentation.
    # doxydoc = pkgs.stdenv.mkDerivation rec {
    #   name = "batsim-doxygen-documentation";
    #   src = pkgs.lib.sourceByRegex ./. [
    #     "^src"
    #     "^src/.*\.?pp"
    #     "^doc"
    #     "^doc/Doxyfile"
    #     "^doc/doxygen_mainpage.md"
    #   ];
    #   buildInputs = [pkgs.doxygen];
    #   buildPhase = "(cd doc && doxygen)";
    #   installPhase = ''
    #     mkdir -p $out
    #     mv doc/doxygen_doc/html/* $out/
    #   '';
    #   checkPhase = ''
    #     nb_warnings=$(cat doc/doxygen_warnings.log | wc -l)
    #     if [[ $nb_warnings -gt 0 ]] ; then
    #       echo "FAILURE: There are doxygen warnings!"
    #       cat doc/doxygen_warnings.log
    #       exit 1
    #     fi
    #   '';
    #   doCheck = true;
    # };

    # Dependencies not in nixpkgs as I write these lines.
    # pytest_metadata = buildPythonPackage {
    #   name = "pytest-metadata-1.8.0";
    #   doCheck = false;
    #   propagatedBuildInputs = [
    #     pythonPackages.pytest
    #     pythonPackages.setuptools_scm
    #   ];
    #   src = builtins.fetchurl {
    #     url = "https://files.pythonhosted.org/packages/12/38/eed3a1e00c765e4da61e4e833de41c3458cef5d18e819d09f0f160682993/pytest-metadata-1.8.0.tar.gz";
    #     sha256 = "1fk6icip2x1nh4kzhbc8cnqrs77avpqvj7ny3xadfh6yhn9aaw90";
    #   };
    # };

    # pytest_html = buildPythonPackage {
    #   name = "pytest-html-1.20.0";
    #   doCheck = false;
    #   propagatedBuildInputs = [
    #     pythonPackages.pytest
    #     pytest_metadata
    #   ];
    #   src = builtins.fetchurl {
    #     url = "https://files.pythonhosted.org/packages/08/3e/63d998f26c7846d3dac6da152d1b93db3670538c5e2fe18b88690c1f52a7/pytest-html-1.20.0.tar.gz";
    #     sha256 = "17jyn4czkihrs225nkpj0h113hc03y0cl07myb70jkaykpfmrim7";
    #   };
    # };
  };
in
  jobs
