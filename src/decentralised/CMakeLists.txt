set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")

set  (ADVANCED_FUNCTION_SOURCES      
      "decentralised_app.h"
      "decentralised_app.cpp"
      "main.cpp"
      "resource.h"
      "decentralised.rc")

set  (MANAGER_SOURCES      
      "managers/manager_gui.cpp"
      "managers/manager_world.cpp"
      "managers/manager_filesystem.cpp"
      "managers/manager_core.cpp")

set  (MANAGER_HEADERS      
      "managers/manager_gui.h"
      "managers/manager_world.h"
      "managers/manager_filesystem.h"
      "managers/manager_core.h")

set  (MANAGER_FILES      
      ${MANAGER_SOURCES} ${MANAGER_HEADERS})

set  (DIALOG_SOURCES      
      "dialogs/dialog_about.cpp"
      "dialogs/dialog_cam.cpp"
      "dialogs/dialog_createavatar.cpp")

set  (DIALOG_HEADERS      
      "dialogs/dialog_about.h"
      "dialogs/dialog_cam.h"
      "dialogs/dialog_createavatar.h")

set  (DIALOG_FILES      
      ${DIALOG_SOURCES} ${DIALOG_HEADERS})

set  (WORLD_SOURCES      
      "world/world_atmosphere.cpp"
      "world/world_terrain.cpp"
      "world/world_avatar.cpp")

set  (WORLD_HEADERS      
      "world/world_atmosphere.h"
      "world/world_terrain.h"
      "world/world_avatar.h")

set  (WORLD_FILES      
      ${WORLD_SOURCES} ${WORLD_HEADERS})

set  (CONTEXT_HEADERS      
      "context/context_dialogs.h"
      "context/context_gui.h"
      "context/context_application.h" )

set  (EH_HEADERS      
      "event_handlers/irrlicht_event_handler.h" )

set  (EH_SOURCES      
      "event_handlers/irrlicht_event_handler.cpp" )

set  (EH_FILES      
      ${EH_SOURCES} ${EH_HEADERS})

SET( OPENSSL_INC_DIR "${CMAKE_SOURCE_DIR}/build/openssl/include" )
SET( LEVELDB_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/leveldb/include" )
SET( OGRE_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/ogre/include/OGRE" )
SET( OGREOIS_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/ogre/include/OIS" )
SET( IRRLICHTEXT_INC_DIR "${CMAKE_SOURCE_DIR}/src/irrlicht-extensions/include" )
SET( IRRLICHTEXT_SRC_DIR "${CMAKE_SOURCE_DIR}/src/irrlicht-extensions/source" )
SET( BOOST_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost" )
SET( BOOST_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost/stage/lib" )
SET( SECP256K1_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/secp256k1/include" )
SET( CORE_INC_DIR "${CMAKE_SOURCE_DIR}/src/decentralised-core/include" )
SET( TINYXML2_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/tinyxml2" )

SET( D3D9_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/ogre/RenderSystems/Direct3D9/include" )
SET( D3D11_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/ogre/RenderSystems/Direct3D11/include" )
SET( GL_INC_DIR "${CMAKE_SOURCE_DIR}/dependencies/ogre/RenderSystems/GL/include" )

source_group( "" FILES ${ADVANCED_FUNCTION_SOURCES} )
source_group( managers FILES ${MANAGER_FILES} )
source_group( dialogs FILES ${DIALOG_FILES} )
source_group( context FILES ${CONTEXT_HEADERS} )
source_group( event_handlers FILES ${EH_FILES} )
source_group( world FILES ${WORLD_FILES} )

include_directories (managers world dialogs context event_handlers ${IRRLICHTEXT_INC_DIR} ${OGRE_INC_DIR} ${IRRLICHTEXT_INC_DIR} ${IRRLICHTEXT_SRC_DIR} ${BOOST_INC_DIR} ${SECP256K1_INC_DIR} ${CORE_INC_DIR} ${BLOCKCHAIN_INC_DIR} ${NODE_INC_DIR} ${LEVELDB_INC_DIR} ${OPENSSL_INC_DIR} ${D3D9_INC_DIR} ${D3D11_INC_DIR} ${GL_INC_DIR} ${TINYXML2_INC_DIR} ${OGREOIS_INC_DIR} )
link_directories("/libs" ${BOOST_LIB_DIR} "${CMAKE_SOURCE_DIR}/build/openssl/lib" "${CMAKE_SOURCE_DIR}/dependencies/ogre/lib/Debug" "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x86" "E:/Programs (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86" )

add_executable (decentralised ${ADVANCED_FUNCTION_SOURCES} ${MANAGER_FILES} ${WORLD_FILES} ${DIALOG_FILES} ${CONTEXT_HEADERS} ${EH_FILES} )
target_link_libraries ( decentralised libcurl snappy leveldb secp256k1 decentralised-core libeay32.lib OgreMainStatic_d.lib RenderSystem_Direct3D9Static_d.lib RenderSystem_Direct3D11Static_d.lib d3d9.lib d3dx9.lib DxErr.lib dxguid.lib d3d11.lib dxgi.lib RenderSystem_GLStatic_d.lib opengl32.lib glu32.lib tinyxml2_static )

add_custom_command(TARGET decentralised POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:decentralised>/data)

add_custom_command(TARGET decentralised POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/lang $<TARGET_FILE_DIR:decentralised>/data/Lang)

IF( WIN32 )
	ADD_DEFINITIONS( -DBC_STATIC -DBCB_STATIC -DBCN_STATIC -D_SCL_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0501 -D_WINSOCK_DEPRECATED_NO_WARNINGS )
ENDIF(  )