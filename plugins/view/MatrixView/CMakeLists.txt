INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS
    MatrixView.cpp
    PropertyValuesDispatcher.cpp
    MatrixViewConfigurationWidget.cpp
    GlMatrixBackgroundGrid.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    MatrixViewConfigurationWidget.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
    MatrixView.h
    MatrixViewConfigurationWidget.h
)

QTX_ADD_RESOURCES(RCC_SRCS MatrixView.qrc)

ADD_LIBRARY(MatrixView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${RCC_SRCS})
TARGET_LINK_LIBRARIES(MatrixView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS MatrixView-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT MatrixView)

