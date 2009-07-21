INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(GMLExport_SRCS
    GMLExport.cpp
)

ADD_LIBRARY(GMLExport-${TulipVersion} SHARED ${GMLExport_SRCS})
TARGET_LINK_LIBRARIES(GMLExport-${TulipVersion} tulip)
ADD_DEPENDENCIES(GMLExport-${TulipVersion} tulip)

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	GMLExport-${TulipVersion}	
	DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/)
