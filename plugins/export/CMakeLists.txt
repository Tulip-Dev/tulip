IF(NOT BUILD_CORE_ONLY)
ADD_SUBDIRECTORY(SVGExport)
ENDIF(NOT BUILD_CORE_ONLY)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(GMLExport_SRCS
  GMLExport.cpp
)

ADD_LIBRARY(GMLExport-${TulipVersion} SHARED ${GMLExport_SRCS})
TARGET_LINK_LIBRARIES(GMLExport-${TulipVersion} ${LibTulipCoreName})

ADD_CORE_FILES(CMakeLists.txt GMLExport.cpp)

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS GMLExport-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT GMLExport)
