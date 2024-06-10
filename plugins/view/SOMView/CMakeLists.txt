ADD_SUBDIRECTORY(SOMLIB)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SOMLIB/)

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

ADD_LIBRARY(SOMView-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(SOMView-${Tulip_VERSION}
    PUBLIC som ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName}
    INTERFACE OpenGL::GL)

TULIP_INSTALL_PLUGIN(SOMView-${Tulip_VERSION} ${TulipViewPluginsInstallDir})
