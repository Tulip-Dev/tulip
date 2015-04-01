ADD_SUBDIRECTORY(SOMLIB)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SOMLIB/)

SET(LIB_SRCS
    ColorScalePreview.cpp
    SOMMapElement.cpp
    SOMWidget.cpp
    ComputeSOMWidget.cpp
    SOMPreviewComposite.cpp
    EditColorScaleInteractor.cpp
    GlLabelledColorScale.cpp
    SOMPropertiesWidget.cpp
    GradientManager.cpp
    SOMView.cpp
    ThresholdInteractor.cpp
    SOMViewInteractor.cpp
    ZoomUtils.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    SOMPropertiesWidget.ui
    ComputeSOMWidget.ui
    SOMWidget.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
    ColorScalePreview.h
    SOMPropertiesWidget.h
    SOMWidget.h
    ComputeSOMWidget.h
    SOMView.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    SOMViewResource.qrc
)

ADD_LIBRARY(SOMView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(SOMView-${TulipVersion} som ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS SOMView-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT SOMView)
