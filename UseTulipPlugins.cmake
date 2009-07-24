FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${TulipSharePath}UseTulip.cmake")

cmake_minimum_required(VERSION 2.6)

## -----------------------------------------------------------------------------------------------
## tulip-config 
## -----------------------------------------------------------------------------------------------
execute_process(COMMAND tulip-config --cxxflags
                OUTPUT_VARIABLE TulipCxxFlags OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND tulip-config --version
                OUTPUT_VARIABLE TulipVersion OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND tulip-config --pluginpath
                OUTPUT_VARIABLE PluginsInstallationDir OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND tulip-config --pluginextension
                OUTPUT_VARIABLE PluginsExtension OUTPUT_STRIP_TRAILING_WHITESPACE)

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

## -----------------------------------------------------------------------------------------------
## Install
## -----------------------------------------------------------------------------------------------
install(FILES lib${PluginName}.${PluginsExtension} 
        DESTINATION ${PluginsInstallationDir}
	RENAME ${PluginInstallName})    

