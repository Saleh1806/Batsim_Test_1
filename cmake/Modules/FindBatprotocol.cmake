# Find the batprotocol library (https://framagit.org/batsim/batprotocol)
#
# Sets the usual variables expected for find_package scripts:
#
# BATPROTOCOL_INCLUDE_DIR - header location
# BATPROTOCOL_LIBRARIES - library to link against
# BATPROTOCOL_FOUND - true if batprotocol was found.

# Find the header file for batprotocol
find_path(BATPROTOCOL_INCLUDE_DIR batprotocol/batprotocol.hpp)

# Find the library for batprotocol
find_library(BATPROTOCOL_LIBRARY batprotocol)

# Support the REQUIRED and QUIET arguments, and set BATPROTOCOL_FOUND if found.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(batprotocol DEFAULT_MSG BATPROTOCOL_LIBRARY
                                   BATPROTOCOL_INCLUDE_DIR)

# Mark these variables as advanced in CMake's GUI.
mark_as_advanced(BATPROTOCOL_LIBRARY BATPROTOCOL_INCLUDE_DIR)
