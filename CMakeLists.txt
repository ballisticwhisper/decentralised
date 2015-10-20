cmake_minimum_required (VERSION 2.6)
project (Decentralised)

# The version number.
set (Decentralised_VERSION_MAJOR 1)
set (Decentralised_VERSION_MINOR 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/dc-config.h.in"
  "${PROJECT_BINARY_DIR}/dc-config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

add_subdirectory(src/decentralised)
