INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${GLEW_INCLUDE_DIRS})

SET(LIB_SRCS
  MouseMagnifyingGlass.cpp
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
MouseMagnifyingGlassResource.qrc
)

ADD_LIBRARY(MouseMagnifyingGlass-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(MouseMagnifyingGlass-${Tulip_VERSION}
    PUBLIC ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName}
    INTERFACE OpenGL::GL GLEW::GLEW)

TULIP_INSTALL_PLUGIN(MouseMagnifyingGlass-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
