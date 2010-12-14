INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${CMAKE_CURRENT_BINARY_DIR} .)

##----------------------------------------------------------------------------------------------------------------------------
SET(ColorMapping_SRCS
    ColorMapping.cpp
    DoubleStringsListRelationDialog.cpp
)

set(ColorMapping_UI
    DoubleStringsListRelationDialog.ui
)

set(ColorMapping_MOC
    DoubleStringsListRelationDialog.h
)

TULIP_QT4_WRAP_UI(ColorMapping_UI_SRCS ${ColorMapping_UI})
TULIP_QT4_WRAP_CPP(ColorMapping_MOC_SRCS ${ColorMapping_MOC})

ADD_LIBRARY(ColorMapping-${TulipVersion} SHARED ${ColorMapping_SRCS} ${ColorMapping_UI_SRCS} ${ColorMapping_MOC_SRCS})
TARGET_LINK_LIBRARIES(ColorMapping-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(ColorMapping-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(ColorMapping-${TulipVersion} ${LibTulipQtName})
ADD_DEPENDENCIES(ColorMapping-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
  ColorMapping-${TulipVersion}
  DESTINATION ${TulipPluginsInstallDir}/
  COMPONENT libtulip_qt_plugins)
