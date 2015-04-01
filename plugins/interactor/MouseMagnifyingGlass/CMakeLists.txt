INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
  MouseMagnifyingGlass.cpp
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
MouseMagnifyingGlassResource.qrc
)

ADD_LIBRARY(MouseMagnifyingGlass-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(MouseMagnifyingGlass-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})

INSTALL(TARGETS MouseMagnifyingGlass-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT MouseMagnifyingGlass)
