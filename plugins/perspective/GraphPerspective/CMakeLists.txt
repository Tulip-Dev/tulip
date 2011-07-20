INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS
AlgorithmRunner.cpp
ExpandableGroupBox.cpp
GraphPerspective.cpp
HeaderFrame.cpp)

QT4_ADD_RESOURCES(RCC_SRCS
resources/GraphPerspective.qrc)

QT4_WRAP_UI(UI_SRCS
designer/AlgorithmRunner.ui
designer/GraphPerspectiveMainWindow.ui
designer/HeaderFrame.ui)

QT4_WRAP_CPP(MOC_SRCS
AlgorithmRunner.h
ExpandableGroupBox.h
HeaderFrame.h)

ADD_LIBRARY(GraphPerspective-${TulipVersion} SHARED ${LIB_SRCS} ${RCC_SRCS} ${UI_SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

INSTALL(TARGETS GraphPerspective-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT GraphPerspective-${TulipVersion})
