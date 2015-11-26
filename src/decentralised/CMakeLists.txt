set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")

set  (ADVANCED_FUNCTION_SOURCES      
      "main.cpp")

set  (MANAGER_SOURCES      
      "managers/manager_gui.cpp"
      "managers/manager_filesystem.cpp"
	  "managers/manager_network.cpp")

set  (MANAGER_HEADERS      
      "managers/manager_gui.h"
      "managers/manager_filesystem.h"
	  "managers/manager_network.h")

set  (MANAGER_FILES      
      ${MANAGER_SOURCES} ${MANAGER_HEADERS})

set  (DIALOG_SOURCES      
      "dialogs/dialog_about.cpp"
      "dialogs/dialog_createavatar.cpp")

set  (DIALOG_HEADERS      
      "dialogs/dialog_about.h"
      "dialogs/dialog_createavatar.h")

set  (DIALOG_FILES      
      ${DIALOG_SOURCES} ${DIALOG_HEADERS})

set  (CONTEXT_HEADERS      
      "context/context_dialogs.h"
      "context/context_gui.h")

SET( IRRLICHT_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/irrlicht/include" )
SET( IRRLICHT_SRC_DIR "${CMAKE_SOURCE_DIR}/dependencies/irrlicht/source/Irrlicht" )
SET( IRRLICHTEXT_INC_DIR "${CMAKE_SOURCE_DIR}/src/irrlichtextensions/include" )
SET( IRRLICHTEXT_SRC_DIR "${CMAKE_SOURCE_DIR}/src/irrlichtextensions/source" )
SET( LIBBITCOIN_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/libbitcoin/include" )
SET( BOOST_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost" )
SET( BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost/stage/lib" )
SET( SECP256K1_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/secp256k1/include" )

source_group("" FILES ${ADVANCED_FUNCTION_SOURCES})
source_group( managers FILES ${MANAGER_FILES})
source_group( dialogs FILES ${DIALOG_FILES})
source_group( context FILES ${CONTEXT_HEADERS})

include_directories (managers dialogs context ${IRRLICHT_INC_DIR} ${IRRLICHT_SRC_DIR} ${IRRLICHTEXT_INC_DIR} ${IRRLICHTEXT_SRC_DIR} ${LIBBITCOIN_INC_DIR} ${BOOST_INC_DIR} ${SECP256K1_INC_DIR} )
link_directories("/libs" ${BOOST_LIB_DIR} )

add_executable (decentralised ${ADVANCED_FUNCTION_SOURCES} ${MANAGER_FILES} ${DIALOG_FILES} ${CONTEXT_HEADERS})
target_link_libraries ( decentralised irrlicht_static irrlichtextensions libbitcoin "${CMAKE_SOURCE_DIR}/libs/secp256k1.lib" )

add_custom_command(TARGET decentralised POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:decentralised>/data)

IF( WIN32 )
	ADD_DEFINITIONS( -DBC_STATIC -D_SCL_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0501 -D_WINSOCK_DEPRECATED_NO_WARNINGS )
ENDIF(  )