<<<<<<< HEAD
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
=======
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder

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
TARGET_LINK_LIBRARIES(MatrixView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName} ${OPENGL_gl_LIBRARY})

INSTALL_TULIP_PLUGIN(MatrixView-${TulipVersion} ${TulipViewPluginsInstallDir})
