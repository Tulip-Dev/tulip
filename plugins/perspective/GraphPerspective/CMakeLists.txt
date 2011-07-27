INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(LIB_SRCS
src/AlgorithmRunner.cpp
src/ExpandableGroupBox.cpp
src/GraphHierarchiesEditor.cpp
src/GraphHierarchiesModel.cpp
src/GraphPerspective.cpp
src/HeaderFrame.cpp)

QT4_ADD_RESOURCES(RCC_SRCS
resources/GraphPerspective.qrc)

QT4_WRAP_UI(UI_SRCS
designer/AlgorithmRunner.ui
designer/AlgorithmRunnerItem.ui
designer/GraphHierarchiesEditor.ui
designer/GraphPerspectiveMainWindow.ui
designer/HeaderFrame.ui)

QT4_WRAP_CPP(MOC_SRCS
include/AlgorithmRunner.h
include/GraphHierarchiesEditor.h
include/GraphHierarchiesModel.h
include/GraphPerspective.h
include/ExpandableGroupBox.h
include/HeaderFrame.h)

ADD_LIBRARY(GraphPerspective-${TulipVersion} SHARED ${LIB_SRCS} ${RCC_SRCS} ${UI_SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

INSTALL(TARGETS GraphPerspective-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT GraphPerspective-${TulipVersion})

