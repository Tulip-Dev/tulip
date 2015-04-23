SET(PLUGIN_NAME cliqueimport)
SET(TULIP_VERSION ${TulipVersion})

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(PLUGIN_SRCS cliqueimport.cpp cliqueimport.h)
ADD_LIBRARY(${PLUGIN_NAME}-${TULIP_VERSION} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TULIP_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS ${PLUGIN_NAME}-${TULIP_VERSION} DESTINATION demos)

