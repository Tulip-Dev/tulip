IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_SUBDIRECTORY(SVGExport)
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(GMLExport_SRCS
  GMLExport.cpp
)

SET(CSVExport_SRCS
  CSVExport.cpp
)

ADD_LIBRARY(GMLExport-${TulipVersion} SHARED ${GMLExport_SRCS})
TARGET_LINK_LIBRARIES(GMLExport-${TulipVersion} ${LibTulipCoreName})

ADD_LIBRARY(CSVExport-${TulipVersion} SHARED ${CSVExport_SRCS})
TARGET_LINK_LIBRARIES(CSVExport-${TulipVersion} ${LibTulipCoreName})

ADD_CORE_FILES(CMakeLists.txt GMLExport.cpp)

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(GMLExport-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(CSVExport-${TulipVersion} ${TulipPluginsInstallDir})
