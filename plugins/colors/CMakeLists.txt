ADD_CORE_FILES(CMakeLists.txt ColorMapping.cpp)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} .)

##----------------------------------------------------------------------------------------------------------------------------

IF(NOT BUILD_CORE_ONLY)

SET(ColorMapping_SRCS
  ColorMapping.cpp
  DoubleStringsListRelationDialog.cpp
)

QTX_WRAP_UI(ColorMapping_UI_SRCS
  DoubleStringsListRelationDialog.ui
)

QTX_WRAP_CPP(ColorMapping_MOC_SRCS
  DoubleStringsListRelationDialog.h
)

ADD_LIBRARY(ColorMapping-${TulipVersion} SHARED ${ColorMapping_SRCS} ${ColorMapping_UI_SRCS} ${ColorMapping_MOC_SRCS})
TARGET_LINK_LIBRARIES(ColorMapping-${TulipVersion} ${LibTulipCoreName} ${LibTulipGUIName})

ELSE(NOT BUILD_CORE_ONLY)

SET(ColorMapping_SRCS
  ColorMapping.cpp
)

SET_SOURCE_FILES_PROPERTIES(ColorMapping.cpp
                            PROPERTIES COMPILE_DEFINITIONS BUILD_CORE_ONLY)

ADD_LIBRARY(ColorMapping-${TulipVersion} SHARED ${ColorMapping_SRCS})
TARGET_LINK_LIBRARIES(ColorMapping-${TulipVersion} ${LibTulipCoreName})

ENDIF(NOT BUILD_CORE_ONLY)

##----------------------------------------------------------------------------------------------------------------------------
INSTALL_TULIP_PLUGIN(ColorMapping-${TulipVersion} ${TulipPluginsInstallDir})
