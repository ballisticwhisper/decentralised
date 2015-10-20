cmake_minimum_required (VERSION 2.6)

project (Decentralised)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

add_subdirectory(src/decentralised)
