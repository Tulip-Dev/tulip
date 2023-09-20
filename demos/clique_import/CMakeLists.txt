SET(PLUGIN_NAME cliqueimport)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(PLUGIN_SRCS cliqueimport.cpp cliqueimport.h)
ADD_LIBRARY(${PLUGIN_NAME}-${Tulip_VERSION} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS ${PLUGIN_NAME}-${Tulip_VERSION} DESTINATION demos)

