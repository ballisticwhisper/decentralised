set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")


set  (SOURCES      
      "${CMAKE_SOURCE_DIR}/src/decentralised_core/src/decentralised_client.cpp" )

set  (HEADERS      
      "${CMAKE_SOURCE_DIR}/src/decentralised_core/src/decentralised_client.h")

SET( LIBBITCOIN_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/libbitcoin/include" )
SET( BLOCKCHAIN_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/libbitcoin-blockchain/include" )
SET( BOOST_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost" )
SET( BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost/stage/lib" )
SET( SECP256K1_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/secp256k1/include" )

source_group( src FILES ${SOURCES} ${HEADERS} )
source_group( src\\blockchain FILES ${BLOCKCHAIN_FILES} )

include_directories (src ${LIBBITCOIN_INC_DIR} ${BOOST_INC_DIR} ${SECP256K1_INC_DIR} ${BLOCKCHAIN_INC_DIR} )

ADD_LIBRARY( decentralised_core STATIC ${SOURCES} ${HEADERS} )

SET_TARGET_PROPERTIES( decentralised_core PROPERTIES LIBRARY_OUTPUT_NAME decentralised_core ARCHIVE_OUTPUT_NAME decentralised_core )

if ( MSVC )
    set_target_properties( decentralised_core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)
    set_target_properties( decentralised_core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/libs)
    set_target_properties( decentralised_core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/libs)
endif ( MSVC )

IF( WIN32 )
	ADD_DEFINITIONS( -DBC_STATIC -DBCB_STATIC -D_SCL_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0501 -D_WINSOCK_DEPRECATED_NO_WARNINGS )
ENDIF(  )