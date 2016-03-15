CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )
PROJECT( decentralised-core )

IF(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
	SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib" )
ENDIF(  )
IF(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY )
	SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib" )
ENDIF(  )

SET( CURL_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/curl/include" )
SET( OPENSSL_INC_DIR "${CMAKE_SOURCE_DIR}/build/openssl/include" )
SET( LEVELDB_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/leveldb/include" )
SET( BOOST_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost" )
SET( SECP256K1_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/secp256k1/include" )
SET( DCCORE_INC_DIR "${CMAKE_SOURCE_DIR}/src/decentralised-core/include" )
SET( DCCORE_SRC_DIR "${CMAKE_SOURCE_DIR}/src/decentralised-core/src" )

SET( BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost/stage/lib" )

INCLUDE_DIRECTORIES( ${SECP256K1_INC_DIR} 
		     ${BOOST_INC_DIR} 
		     ${DCCORE_INC_DIR}
		     ${LEVELDB_INC_DIR}
		     ${OPENSSL_INC_DIR}
		     ${CURL_INC_DIR} )

FILE( GLOB DCCORE_INC_ROOT_FILES "${DCCORE_INC_DIR}/*.hpp" )
FILE( GLOB DCCORE_CORE_INC_FILES "${DCCORE_INC_DIR}/core/*.hpp" )
FILE( GLOB DCCORE_CORE_UTIL_INC_FILES "${DCCORE_INC_DIR}/core/utility/*.hpp" )
FILE( GLOB DCCORE_CORE_UTIL_EXT_INC_FILES "${DCCORE_INC_DIR}/core/utility/external/*.h" )
FILE( GLOB DCCORE_CORE_BC_INC_FILES "${DCCORE_INC_DIR}/core/blockchain/*.hpp" )
FILE( GLOB DCCORE_CORE_BC_DB_INC_FILES "${DCCORE_INC_DIR}/core/blockchain/database/*.hpp" )
FILE( GLOB DCCORE_CORE_IMPL_INC_FILES "${DCCORE_INC_DIR}/core/impl/*.ipp" )
FILE( GLOB DCCORE_CORE_IMPL_UTIL_INC_FILES "${DCCORE_INC_DIR}/core/impl/utility/*.ipp" )
FILE( GLOB DCCORE_CORE_IMPL_SERIAL_INC_FILES "${DCCORE_INC_DIR}/core/impl/serialize/*.ipp" )
FILE( GLOB DCCORE_CORE_NET_INC_FILES "${DCCORE_INC_DIR}/core/network/*.hpp" )

FILE( GLOB DCCORE_SRC_ROOT_FILES "${DCCORE_SRC_DIR}/*.cpp" )
FILE( GLOB DCCORE_UTIL_SRC_ROOT_FILES "${DCCORE_SRC_DIR}/utility/*.cpp" )
FILE( GLOB DCCORE_UTIL_EXT_SRC_FILES "${DCCORE_SRC_DIR}/utility/external/*.c" )
FILE( GLOB DCCORE_BC_SRC_FILES "${DCCORE_SRC_DIR}/blockchain/*.cpp" )
FILE( GLOB DCCORE_BC_DB_SRC_FILES "${DCCORE_SRC_DIR}/blockchain/database/*.cpp" )
FILE( GLOB DCCORE_BC_LEVELDB_SRC_FILES "${DCCORE_SRC_DIR}/blockchain/leveldb/*.cpp" "${DCCORE_SRC_DIR}/blockchain/leveldb/*.hpp" )
FILE( GLOB DCCORE_NET_SRC_FILES "${DCCORE_SRC_DIR}/network/*.cpp" )

source_group( include FILES ${DCCORE_INC_ROOT_FILES} )
source_group( include\\core FILES ${DCCORE_CORE_INC_FILES} )
source_group( include\\core\\utility FILES ${DCCORE_CORE_UTIL_INC_FILES} )
source_group( include\\core\\utility\\external FILES ${DCCORE_CORE_UTIL_EXT_INC_FILES} )
source_group( include\\core\\impl FILES ${DCCORE_CORE_IMPL_INC_FILES} )
source_group( include\\core\\impl\\utility FILES ${DCCORE_CORE_IMPL_UTIL_INC_FILES} )
source_group( include\\core\\impl\\serialize FILES ${DCCORE_CORE_IMPL_SERIAL_INC_FILES} )
source_group( include\\core\\blockchain FILES ${DCCORE_CORE_BC_INC_FILES} )
source_group( include\\core\\blockchain\\database FILES ${DCCORE_CORE_BC_DB_INC_FILES} )
source_group( include\\core\\network FILES ${DCCORE_CORE_NET_INC_FILES} )

source_group( src FILES ${DCCORE_SRC_ROOT_FILES} )
source_group( src\\utility FILES ${DCCORE_UTIL_SRC_ROOT_FILES} )
source_group( src\\utility\\external FILES ${DCCORE_UTIL_EXT_SRC_FILES} )
source_group( src\\blockchain FILES ${DCCORE_BC_SRC_FILES} )
source_group( src\\blockchain\\database FILES ${DCCORE_BC_DB_SRC_FILES} )
source_group( src\\blockchain\\leveldb FILES ${DCCORE_BC_LEVELDB_SRC_FILES} )
source_group( src\\network FILES ${DCCORE_NET_SRC_FILES} )

ADD_LIBRARY( decentralised-core STATIC ${DCCORE_INC_ROOT_FILES} ${DCCORE_CORE_INC_FILES} ${DCCORE_CORE_UTIL_INC_FILES} ${DCCORE_SRC_ROOT_FILES} ${DCCORE_UTIL_SRC_ROOT_FILES} ${DCCORE_CORE_IMPL_INC_FILES} ${DCCORE_CORE_IMPL_UTIL_INC_FILES} ${DCCORE_CORE_IMPL_SERIAL_INC_FILES} ${DCCORE_CORE_UTIL_EXT_INC_FILES} ${DCCORE_UTIL_EXT_SRC_FILES} ${DCCORE_CORE_BC_INC_FILES} ${DCCORE_BC_SRC_FILES} ${DCCORE_CORE_BC_DB_INC_FILES} ${DCCORE_BC_DB_SRC_FILES} ${DCCORE_BC_LEVELDB_SRC_FILES} ${DCCORE_CORE_NET_INC_FILES} ${DCCORE_NET_SRC_FILES} )

SET_TARGET_PROPERTIES( decentralised-core PROPERTIES LIBRARY_OUTPUT_NAME decentralised-core ARCHIVE_OUTPUT_NAME decentralised-core )

if ( MSVC )
    set_target_properties( decentralised-core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)
    set_target_properties( decentralised-core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/libs)
    set_target_properties( decentralised-core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/libs)
endif ( MSVC )

IF( WIN32 )
	ADD_DEFINITIONS( -DBC_STATIC -DBCB_STATIC -DBCN_STATIC -D_SCL_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0501 -D_WINSOCK_DEPRECATED_NO_WARNINGS -DBUILDING_LIBCURL -DCURL_DISABLE_LDAP )
ENDIF(  )