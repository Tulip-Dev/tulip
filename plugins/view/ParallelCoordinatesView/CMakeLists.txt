<<<<<<< HEAD
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
=======
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder

SET(LIB_SRCS
    ParallelCoordinatesView.cpp
      ParallelCoordinatesDrawing.cpp
      ParallelCoordinatesGraphProxy.cpp
      ParallelAxis.cpp
      NominalParallelAxis.cpp
      AxisConfigDialogs.cpp
      QuantitativeParallelAxis.cpp
      ParallelCoordsElementShowInfo.cpp
      ParallelCoordsElementsSelector.cpp
      ParallelCoordsGlEntitiesSelector.cpp
      ParallelCoordsElementDeleter.cpp
      ParallelCoordsElementHighlighter.cpp
      ParallelCoordsAxisSwapper.cpp
      ParallelCoordsAxisSliders.cpp
      ParallelCoordsAxisBoxPlot.cpp
      ParallelCoordsAxisSpacer.cpp
      ParallelCoordinatesInteractors.cpp
      ParallelTools.cpp
      ParallelCoordsDrawConfigWidget.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    ParallelCoordsDrawConfigWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
      ParallelCoordinatesView.h
      AxisConfigDialogs.h
      ParallelCoordsDrawConfigWidget.h
      ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
ParallelResource.qrc
)

ADD_LIBRARY(ParallelCoordinatesView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(ParallelCoordinatesView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

INSTALL_TULIP_PLUGIN(ParallelCoordinatesView-${TulipVersion} ${TulipViewPluginsInstallDir})
