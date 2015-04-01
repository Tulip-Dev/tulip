INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(SVGExport-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(SVGExport-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

INSTALL(TARGETS SVGExport-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT LinLog)
