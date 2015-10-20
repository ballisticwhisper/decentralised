set  (ADVANCED_FUNCTION_SOURCES      
      "main.cpp")

source_group("" FILES ${ADVANCED_FUNCTION_SOURCES})

include_directories (include)

add_executable (decentralised main.cpp)

install (TARGETS decentralised
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)