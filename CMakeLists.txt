cmake_minimum_required (VERSION 2.6)
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
add_subdirectory(dependencies/libbitcoin)
add_subdirectory(src/irrlichtextensions)
add_subdirectory(src/decentralised)
# INCLUDE_EXTERNAL_MSPROJECT(libbitcoin "../dependencies/libbitcoin/builds/msvc/vs2013/libbitcoin/libbitcoin.vcxproj")