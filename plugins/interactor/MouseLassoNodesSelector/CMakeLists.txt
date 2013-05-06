INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude})

SET(LIB_SRCS
    MouseLassoNodesSelector.cpp
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    MouseLassoNodesSelectorResource.qrc
)

ADD_LIBRARY(MouseLassoNodesSelector-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(MouseLassoNodesSelector-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS MouseLassoNodesSelector-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT MouseLassoNodesSelector)
