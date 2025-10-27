INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR})

SET(LIB_SRCS
    MouseLassoNodesSelector.cpp
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    MouseLassoNodesSelectorResource.qrc
)

ADD_LIBRARY(MouseLassoNodesSelector-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(MouseLassoNodesSelector-${Tulip_VERSION}
            PUBLIC ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName}
            INTERFACE OpenGL::GL)

TULIP_INSTALL_PLUGIN(MouseLassoNodesSelector-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
