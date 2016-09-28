<<<<<<< HEAD
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${GLEW_INCLUDE_DIR})
=======
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${GLEW_INCLUDE_DIR})
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder

SET(LIB_SRCS
  MouseMagnifyingGlass.cpp
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
MouseMagnifyingGlassResource.qrc
)

ADD_LIBRARY(MouseMagnifyingGlass-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(MouseMagnifyingGlass-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})

INSTALL_TULIP_PLUGIN(MouseMagnifyingGlass-${TulipVersion} ${TulipInteractorPluginsInstallDir})
