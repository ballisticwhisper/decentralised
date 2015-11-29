cmake_minimum_required (VERSION 3.4)

project (Decentralised)

# The version number.
set (Decentralised_VERSION_MAJOR 1)
set (Decentralised_VERSION_MINOR 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/dc-config.h.in"
  "${PROJECT_BINARY_DIR}/dc-config.h"
  )
include_directories("${PROJECT_BINARY_DIR}"
		    "${PROJECT_BINARY_DIR}/../dependencies/irrlicht/include")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../libs)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

add_subdirectory(dependencies/snappy)
add_subdirectory(dependencies/irrlicht)
add_subdirectory(dependencies/secp256k1)
add_subdirectory(dependencies/libbitcoin)
add_subdirectory(dependencies/libbitcoin-blockchain)
add_subdirectory(src/irrlichtextensions)
add_subdirectory(src/decentralised_core)
add_subdirectory(src/decentralised)

set( python "python" )
set( conf "dc-configure.py" )
