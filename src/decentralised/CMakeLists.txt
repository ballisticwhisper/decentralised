set  (ADVANCED_FUNCTION_SOURCES      
      "main.cpp")

source_group("" FILES ${ADVANCED_FUNCTION_SOURCES})

include_directories (include)
link_directories("/libs")

add_executable (decentralised main.cpp)
target_link_libraries (decentralised irrlicht_static)
