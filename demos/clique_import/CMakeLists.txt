SET(PLUGIN_NAME cliqueimport)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(PLUGIN_SRCS cliqueimport.cpp cliqueimport.h)
ADD_LIBRARY(${PLUGIN_NAME}-${TULIP_VERSION} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TULIP_VERSION} ${TulipLibs} ${QT_LIBRARIES})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS ${PLUGIN_NAME}-${TULIP_VERSION} DESTINATION demos)

