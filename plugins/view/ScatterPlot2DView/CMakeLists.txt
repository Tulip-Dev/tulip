INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${GLEW_INCLUDE_DIRS})

SET(LIB_SRCS
    ScatterPlot2DView.cpp
    ScatterPlot2D.cpp
    ScatterPlot2DViewNavigator.cpp
    ScatterPlotTrendLine.cpp
    ScatterPlot2DOptionsWidget.cpp
    ScatterPlot2DInteractors.cpp
    ScatterPlotCorrelCoeffSelector.cpp
    ScatterPlotCorrelCoeffSelectorOptionsWidget.cpp
    ScatterPlotQuickAccessBar.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    ScatterPlot2DOptionsWidget.ui
    ScatterPlotCorrelCoeffSelectorOptionsWidget.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
    ScatterPlot2DView.h
    ScatterPlot2DOptionsWidget.h
    ScatterPlotCorrelCoeffSelectorOptionsWidget.h
    ScatterPlotQuickAccessBar.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    ScatterPlotResource.qrc
)

ADD_LIBRARY(ScatterPlot2DView-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})

TARGET_LINK_LIBRARIES(ScatterPlot2DView-${Tulip_VERSION}
    PUBLIC ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName}
    INTERFACE OpenGL::GL)

TULIP_INSTALL_PLUGIN(ScatterPlot2DView-${Tulip_VERSION} ${TulipViewPluginsInstallDir})
