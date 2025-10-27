INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} include)

SET(LIB_SRCS
    src/ParallelCoordinatesView.cpp
      src/ParallelCoordinatesDrawing.cpp
      src/ParallelCoordinatesGraphProxy.cpp
      src/ParallelAxis.cpp
      src/NominalParallelAxis.cpp
      src/AxisConfigDialogs.cpp
      src/QuantitativeParallelAxis.cpp
      src/ParallelCoordsElementShowInfo.cpp
      src/ParallelCoordsElementsSelector.cpp
      src/ParallelCoordsGlEntitiesSelector.cpp
      src/ParallelCoordsElementDeleter.cpp
      src/ParallelCoordsElementHighlighter.cpp
      src/ParallelCoordsAxisSwapper.cpp
      src/ParallelCoordsAxisSliders.cpp
      src/ParallelCoordsAxisBoxPlot.cpp
      src/ParallelCoordsAxisSpacer.cpp
      src/ParallelCoordinatesInteractors.cpp
      src/ParallelTools.cpp
      src/ParallelCoordsDrawConfigWidget.cpp
      src/ParallelCoordinatesViewQuickAccessbar.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    designer/ParallelCoordsDrawConfigWidget.ui
    designer/AxisSlidersOptions.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
      include/ParallelCoordinatesView.h
      include/AxisConfigDialogs.h
      include/ParallelCoordsDrawConfigWidget.h
      include/ParallelCoordinatesViewQuickAccessbar.h
      include/ParallelCoordinatesInteractors.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
resources/ParallelResource.qrc
)

ADD_LIBRARY(ParallelCoordinatesView-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(ParallelCoordinatesView-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

TULIP_INSTALL_PLUGIN(ParallelCoordinatesView-${Tulip_VERSION} ${TulipViewPluginsInstallDir})
