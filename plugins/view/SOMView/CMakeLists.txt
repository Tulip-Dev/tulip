ADD_SUBDIRECTORY(SOMLIB)

<<<<<<< HEAD
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SOMLIB/)
=======
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SOMLIB/)
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder

SET(LIB_SRCS
    ColorScalePreview.cpp
    ComputeSOMWidget.cpp
    EditColorScaleInteractor.cpp
    GlLabelledColorScale.cpp
    GradientManager.cpp
    ThresholdInteractor.cpp
    SOMMapElement.cpp
    SOMPreviewComposite.cpp
    SOMPropertiesWidget.cpp
    SOMView.cpp
    SOMWidget.cpp
    SOMViewInteractor.cpp
    ZoomUtils.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    ComputeSOMWidget.ui
    SOMPropertiesWidget.ui
    SOMWidget.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
    ColorScalePreview.h
    ComputeSOMWidget.h
    SOMPropertiesWidget.h
    SOMWidget.h
    SOMView.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    SOMViewResource.qrc
)

ADD_LIBRARY(SOMView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(SOMView-${TulipVersion} som ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName} ${OPENGL_gl_LIBRARY})

INSTALL_TULIP_PLUGIN(SOMView-${TulipVersion} ${TulipViewPluginsInstallDir})
