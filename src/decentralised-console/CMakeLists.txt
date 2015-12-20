
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")

set  (MAIN_SOURCES      
      "src/main.cpp")

SET( OPENSSL_INC_DIR "${CMAKE_SOURCE_DIR}/build/openssl/include" )
SET( LEVELDB_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/leveldb/include" )
SET( BOOST_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost" )
SET( BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost/stage/lib" )
SET( SECP256K1_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/secp256k1/include" )
SET( CORE_INC_DIR "${CMAKE_SOURCE_DIR}/src/decentralised-core/include" )
SET( CORE_INC_CORE_DIR "${CMAKE_SOURCE_DIR}/src/decentralised-core/include/core" )

source_group( "" FILES ${MAIN_SOURCES} )

include_directories (managers dialogs context event_handlers ${CORE_INC_CORE_DIR} ${BOOST_INC_DIR} ${CORE_INC_DIR} ${SECP256K1_INC_DIR} ${CORE_INC_DIR} ${LEVELDB_INC_DIR} ${OPENSSL_INC_DIR} )
link_directories("/libs" ${BOOST_LIB_DIR} "${CMAKE_SOURCE_DIR}/build/openssl/lib" )

add_executable (decentralised-console ${MAIN_SOURCES} )
target_link_libraries ( decentralised-console snappy leveldb secp256k1 decentralised-core libeay32.lib )

IF( WIN32 )
	ADD_DEFINITIONS( -DBC_STATIC -DBCB_STATIC -DBCN_STATIC -D_SCL_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0501 -D_WINSOCK_DEPRECATED_NO_WARNINGS )
ENDIF(  )