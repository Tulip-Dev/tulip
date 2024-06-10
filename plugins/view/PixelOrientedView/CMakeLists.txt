INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${GLEW_INCLUDE_DIRS})

SET(LIB_SRCS
    PixelOrientedView.cpp
    PixelOrientedOverview.cpp
    PixelOrientedViewNavigator.cpp
    PixelOrientedInteractors.cpp
    PixelOrientedOptionsWidget.cpp
    PixelOrientedViewQuickAccessBar.cpp
    POLIB/FishEyesScreen.cpp
    POLIB/FishEyesScreenFurnas.cpp
    POLIB/HilbertLayout.cpp
    POLIB/HSIColorMapping.cpp
    POLIB/HSIColorSpace.cpp
    POLIB/LinearMappingColor.cpp
    POLIB/LogarithmicMappingColor.cpp
    POLIB/PixelOrientedMediator.cpp
    POLIB/SpiralLayout.cpp
    POLIB/SquareLayout.cpp
    POLIB/TulipGraphDimension.cpp
    POLIB/TulipNodeColorMapping.cpp
    POLIB/TulipNodeMetricSorter.cpp
    POLIB/UniformDeformationScreen.cpp
    POLIB/ZOrderLayout.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    PixelOrientedOptionsWidget.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
    PixelOrientedView.h
    PixelOrientedOptionsWidget.h
    PixelOrientedViewQuickAccessBar.h
)

QTX_ADD_RESOURCES(RCC_SRCS PixelOrientedView.qrc)

ADD_LIBRARY(PixelOrientedView-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${RCC_SRCS})
TARGET_LINK_LIBRARIES(PixelOrientedView-${Tulip_VERSION}
        PUBLIC ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName}
        INTERFACE OpenGL::GL)

TULIP_INSTALL_PLUGIN(PixelOrientedView-${Tulip_VERSION} ${TulipViewPluginsInstallDir})
