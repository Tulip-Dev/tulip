CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(cliqueimport)
SET(PLUGIN_NAME cliqueimport)

FIND_PACKAGE( Qt4 REQUIRED)
FIND_PACKAGE( TULIP REQUIRED)
include(${QT_USE_FILE})

include_directories(${TULIP_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(PLUGIN_SRCS cliqueimport.cpp cliqueimport.h)
ADD_LIBRARY(${PLUGIN_NAME}-${TULIP_VERSION} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TULIP_VERSION} ${TULIP_CORE_LIBRARY} ${QT_QTCORE_LIBRARY})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS ${PLUGIN_NAME}-${TULIP_VERSION} DESTINATION demos)

