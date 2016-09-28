<<<<<<< HEAD
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_SOURCE_DIR})
=======
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_SOURCE_DIR})
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(SVGExport-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(SVGExport-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

INSTALL_TULIP_PLUGIN(SVGExport-${TulipVersion} ${TulipPluginsInstallDir})
