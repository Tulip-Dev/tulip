## -----------------------------------------------------------------------------------------------
## ***********************************************************************************************
## -----------------------------------------------------------------------------------------------
##
## Options for cmake (use -D):
## PLUGINS_DIR=path_to_install_plugins (if no set : tulip install dir)
## SERVER_INSTALL=1 activate server style installation
## PLUGINS_SERVER_EXT=i386 extension use with server style installation
##
## -----------------------------------------------------------------------------------------------
## ***********************************************************************************************
## -----------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )
FIND_PACKAGE(OpenGL)

INCLUDE("${TulipSharePath}UseTulip.cmake")

## -----------------------------------------------------------------------------------------------
## tulip-config 
## -----------------------------------------------------------------------------------------------
EXEC_PROGRAM(sh ARGS "tulip-config --cxxflags" OUTPUT_VARIABLE TulipCxxFlags)
EXEC_PROGRAM(sh ARGS "tulip-config --version" OUTPUT_VARIABLE TulipVersion)
EXEC_PROGRAM(sh ARGS "tulip-config --cmakepluginpath" OUTPUT_VARIABLE PluginsInstallationDir)
EXEC_PROGRAM(sh ARGS "tulip-config --pluginextension" OUTPUT_VARIABLE PluginsExtension)
EXEC_PROGRAM(sh ARGS "tulip-config --cmakelib_tulip" OUTPUT_VARIABLE LibTulip)
EXEC_PROGRAM(sh ARGS "tulip-config --cmakelib_ogl" OUTPUT_VARIABLE LibTulipOgl)
EXEC_PROGRAM(sh ARGS "tulip-config --cmakelib_qt" OUTPUT_VARIABLE LibTulipQt)


## -----------------------------------------------------------------------------------------------
## debug mode
## -----------------------------------------------------------------------------------------------
SET(DebugFlags "-DNDEBUG")
if (DEFINED DEBUG)
  if (DEBUG EQUAL 1)
    SET(DebugFlags "-g")
  endif (DEBUG EQUAL 1)
endif (DEFINED DEBUG)

## -----------------------------------------------------------------------------------------------
## installation directory and name
## -----------------------------------------------------------------------------------------------
SET(PluginInstallName "lib${PluginName}-${TulipVersion}.${PluginsExtension}")
if (DEFINED PLUGINS_DIR)
    message("Error  : ${PLUGINS_DIR}")
    SET(PluginsInstallationDir "${PLUGINS_DIR}/${InstallSubDir}")
    if (DEFINED SERVER_INSTALL)
        SET(PluginsInstallationDir "${PLUGINS_DIR}/${PluginName}.${TulipVersion}/")
	SET(PluginInstallName "lib${PluginName}.${TulipVersion}.${PluginsExtension}.${TulipVersion}.${PluginRelease}.${PLUGINS_SERVER_EXT}")
    endif  (DEFINED SERVER_INSTALL)
else(DEFINED PLUGINS_DIR)
  SET(PluginsInstallationDir "${PluginsInstallationDir}/${InstallSubDir}")
endif(DEFINED PLUGINS_DIR)

## -----------------------------------------------------------------------------------------------
## compilation flags
## -----------------------------------------------------------------------------------------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TulipCxxFlags} ${DebugFlags}")

## -----------------------------------------------------------------------------------------------
## Qt Rules
## -----------------------------------------------------------------------------------------------
TULIP_QT4_WRAP_UI(UI_SRCS ${UI_PRE_SRCS})
TULIP_QT4_WRAP_CPP(MOC_SRCS ${MOC_PRE_SRCS})
QT4_ADD_RESOURCES(RCC_SRCS ${RCC_PRE_SRCS})

## -----------------------------------------------------------------------------------------------
## Compilation 
## -----------------------------------------------------------------------------------------------
ADD_LIBRARY(${PluginName} SHARED ${UI_SRCS} ${MOC_SRCS} ${RCC_SRCS} ${LIB_SRCS})
TARGET_LINK_LIBRARIES(${PluginName} ${OPENGL_glu_LIBRARY})
TARGET_LINK_LIBRARIES(${PluginName} ${OPENGL_gl_LIBRARY})
TARGET_LINK_LIBRARIES(${PluginName} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(${PluginName} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(${PluginName} ${QT_QTOPENGL_LIBRARY})
TARGET_LINK_LIBRARIES(${PluginName} ${LibTulip})
TARGET_LINK_LIBRARIES(${PluginName} ${LibTulipOgl})
TARGET_LINK_LIBRARIES(${PluginName} ${LibTulipQt})

## -----------------------------------------------------------------------------------------------
## Install
## -----------------------------------------------------------------------------------------------
install(FILES lib${PluginName}.${PluginsExtension} 
        DESTINATION ${PluginsInstallationDir}
	RENAME ${PluginInstallName})    

