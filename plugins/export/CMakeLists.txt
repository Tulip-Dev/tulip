IF(NOT TULIP_BUILD_CORE_ONLY)
ADD_SUBDIRECTORY(SVGExport)
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(GMLExport_SRCS
  GMLExport.cpp
)

SET(CSVExport_SRCS
  CSVExport.cpp
)

ADD_LIBRARY(GMLExport-${Tulip_VERSION} SHARED ${GMLExport_SRCS})
TARGET_LINK_LIBRARIES(GMLExport-${Tulip_VERSION} ${LibTulipCoreName})

ADD_LIBRARY(CSVExport-${Tulip_VERSION} SHARED ${CSVExport_SRCS})
TARGET_LINK_LIBRARIES(CSVExport-${Tulip_VERSION} ${LibTulipCoreName})

ADD_CORE_FILES(CMakeLists.txt GMLExport.cpp)

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(GMLExport-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(CSVExport-${Tulip_VERSION} ${TulipPluginsInstallDir})
