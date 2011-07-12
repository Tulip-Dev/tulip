INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude})

SET(LIB_SRCS
GraphPerspective.cpp)

QT4_ADD_RESOURCES(RCC_SRCS
resources/GraphPerspective.qrc)

ADD_LIBRARY(GraphPerspective-${TulipVersion} SHARED ${LIB_SRCS} ${RCC_SRCS})

TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

INSTALL(TARGETS GraphPerspective-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT GraphPerspective-${TulipVersion})
