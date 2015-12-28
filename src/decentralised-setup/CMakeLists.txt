
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")

SET( SETUP_ROOT_DIR "${CMAKE_SOURCE_DIR}/src/decentralised-setup" )
SET( SETUP_INC_DIR "${CMAKE_SOURCE_DIR}/src/decentralised-setup/include" )
SET( SETUP_SRC_DIR "${CMAKE_SOURCE_DIR}/src/decentralised-setup/src" )

FILE( GLOB SETUP_ROOT_FILES "${SETUP_ROOT_DIR}/*.rc" "${SETUP_ROOT_DIR}/*.xpm" )
FILE( GLOB SETUP_INC_FILES "${SETUP_INC_DIR}/*.h" )
FILE( GLOB SETUP_SRC_FILES "${SETUP_SRC_DIR}/*.cpp" )

SET( BOOST_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost" )
SET( BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost/stage/lib" )
SET( WX_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/wxWidgets/include" )
SET( WX_SETUP_DIR "${CMAKE_SOURCE_DIR}/dependencies/wxWidgets/lib/vc_lib/mswud" )

source_group( "" FILES ${SETUP_ROOT_FILES} )
source_group( "src" FILES ${SETUP_SRC_FILES} )
source_group( "include" FILES ${SETUP_INC_FILES} )

include_directories ( ${BOOST_INC_DIR} ${WX_INC_DIR} ${MAIN_INCLUDES} ${SETUP_INC_DIR} ${WX_SETUP_DIR} )
link_directories("${CMAKE_SOURCE_DIR}/libs" ${BOOST_LIB_DIR} "${CMAKE_SOURCE_DIR}/dependencies/wxWidgets/lib/vc_lib" )

IF(WIN32)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows") 
ENDIF(WIN32)

add_executable (decentralised-setup ${SETUP_ROOT_FILES} ${SETUP_SRC_FILES} ${SETUP_INC_FILES} )
target_link_libraries ( decentralised-setup wxbase31ud wxmsw31ud_core wxpngd wxzlibd wxtiffd wxscintillad wxregexud wxmsw31ud_adv wxjpegd wxexpatd winmm.lib shlwapi.lib comctl32.lib rpcrt4.lib version.lib wsock32.lib wininet.lib )

IF( WIN32 )
	ADD_DEFINITIONS( -DWIN32 -D_CRT_SECURE_NO_DEPRECATE=1 -D_CRT_NON_CONFORMING_SWPRINTFS=1 -D_SCL_SECURE_NO_WARNINGS=1 -D__WXMSW__ -D_UNICODE -D_WINDOWS -DNOPCH )
ENDIF(  )