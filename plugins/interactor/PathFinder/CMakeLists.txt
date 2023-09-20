INCLUDE_DIRECTORIES(BEFORE ${TulipCoreBuildInclude})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}  ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude})

SET(LIB_SRCS
    PathFinder.cpp
    PathFinderComponent.cpp
    PathFinderConfigurationWidget.cpp
    PathFinderTools.cpp
    PathFinding/PathAlgorithm.cpp
    PathFinding/DFS/DFS.cpp
    highlighters/EnclosingCircleHighlighter.cpp
    highlighters/PathHighlighter.cpp
    highlighters/ZoomAndPanHighlighter.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    designer/EnclosingCircleConfiguration.ui
    designer/PathFinderConfiguration.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
      PathFinderConfigurationWidget.h
      PathFinder.h
      highlighters/EnclosingCircleHighlighter.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
	designer/resources.qrc
)

ADD_LIBRARY(PathFinder-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(PathFinder-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

TULIP_INSTALL_PLUGIN(PathFinder-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
