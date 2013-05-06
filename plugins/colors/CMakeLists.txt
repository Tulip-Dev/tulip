INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} .)

##----------------------------------------------------------------------------------------------------------------------------
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
TARGET_LINK_LIBRARIES(ColorMapping-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS ColorMapping-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ColorMapping)
