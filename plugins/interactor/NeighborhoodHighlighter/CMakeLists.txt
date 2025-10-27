
INCLUDE_DIRECTORIES(BEFORE ${TulipCoreBuildInclude})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIR} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude})
 
SET(LIB_SRCS
  NeighborhoodHighlighterConfigWidget.cpp
  NeighborhoodHighlighterInteractor.cpp
  NodeNeighborhoodView.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
NeighborhoodHighlighterConfigWidget.ui  
)

QTX_WRAP_CPP(LIB_MOC_SRCS
NeighborhoodHighlighterInteractor.h
NeighborhoodHighlighterConfigWidget.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
NeighborhoodHighlighterResource.qrc
)

ADD_LIBRARY(NeighborhoodHighlighter-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(NeighborhoodHighlighter-${Tulip_VERSION}
    PUBLIC ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName}
    INTERFACE OpenGL::GL)

TULIP_INSTALL_PLUGIN(NeighborhoodHighlighter-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
