ADD_CORE_FILES(CMakeLists.txt ColorMapping.cpp)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} .)

##----------------------------------------------------------------------------------------------------------------------------

IF(NOT TULIP_BUILD_CORE_ONLY)

QTX_SET_INCLUDES_AND_DEFINITIONS()

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

ADD_LIBRARY(ColorMapping-${Tulip_VERSION} SHARED ${ColorMapping_SRCS} ${ColorMapping_UI_SRCS} ${ColorMapping_MOC_SRCS})
TARGET_LINK_LIBRARIES(ColorMapping-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipGUIName})

ELSE(NOT TULIP_BUILD_CORE_ONLY)

SET(ColorMapping_SRCS
  ColorMapping.cpp
)

SET_SOURCE_FILES_PROPERTIES(ColorMapping.cpp
                            PROPERTIES COMPILE_DEFINITIONS TULIP_BUILD_CORE_ONLY)

ADD_LIBRARY(ColorMapping-${Tulip_VERSION} SHARED ${ColorMapping_SRCS})
TARGET_LINK_LIBRARIES(ColorMapping-${Tulip_VERSION} ${LibTulipCoreName})

ENDIF(NOT TULIP_BUILD_CORE_ONLY)

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(ColorMapping-${Tulip_VERSION} ${TulipPluginsInstallDir})
