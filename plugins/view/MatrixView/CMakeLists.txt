INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS
    MatrixView.cpp
    PropertyValuesDispatcher.cpp
    MatrixViewConfigurationWidget.cpp
    colorbutton.cpp
    GlMatrixBackgroundGrid.cpp
)

QT4_WRAP_UI(LIB_UI_HEADERS
    MatrixViewConfigurationWidget.ui
)

QT4_WRAP_CPP(LIB_MOC_SRCS
    MatrixView.h
    colorbutton.h
    MatrixViewConfigurationWidget.h
)

QT4_ADD_RESOURCES(RCC_SRCS MatrixView.qrc)

ADD_LIBRARY(MatrixView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${RCC_SRCS})
TARGET_LINK_LIBRARIES(MatrixView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS MatrixView-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT MatrixView)

