ADD_CORE_FILES(CMakeLists.txt ColorMapping.cpp)

<<<<<<< HEAD
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} .)
=======
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} .)
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder

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
