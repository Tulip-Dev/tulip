INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
    ScatterPlot2DView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.cpp
    ScatterPlot2D.cpp
    ScatterPlot2DViewNavigator.cpp
    ScatterPlotTrendLine.cpp
    ScatterPlot2DOptionsWidget.cpp
    ScatterPlot2DInteractors.cpp
    ScatterPlotCorrelCoeffSelector.cpp
    ScatterPlotCorrelCoeffSelectorOptionsWidget.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.ui
    ScatterPlot2DOptionsWidget.ui
    ScatterPlotCorrelCoeffSelectorOptionsWidget.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
    ScatterPlot2DView.h
    ScatterPlot2DOptionsWidget.h
    ScatterPlotCorrelCoeffSelectorOptionsWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    ScatterPlotResource.qrc
)

ADD_LIBRARY(ScatterPlot2DView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})

TARGET_LINK_LIBRARIES(ScatterPlot2DView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS ScatterPlot2DView-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT ScatterPlot2DView)
