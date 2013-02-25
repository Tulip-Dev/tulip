
FIND_PACKAGE(Qt4  REQUIRED)
SET(QT_USE_QTOPENGL true)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE( TULIP REQUIRED )
FIND_PACKAGE( OpenGL REQUIRED)

INCLUDE_DIRECTORIES(${TULIP_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})	

STRING(REGEX REPLACE ".*/" "" PLUGIN_NAME ${CMAKE_CURRENT_SOURCE_DIR})
STRING(TOLOWER ${PLUGIN_NAME} PLUGIN_NAME)

SET(LIB_SRCS
    ParallelCoordinatesView.cpp
      ParallelCoordinatesDrawing.cpp
      ParallelCoordinatesGraphProxy.cpp
      ParallelAxis.cpp
      ParallelCoordinatesConfigDialog.cpp
      NominalParallelAxis.cpp
      AxisConfigDialogs.cpp 
      QuantitativeParallelAxis.cpp
      ParallelCoordsElementShowInfos.cpp
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

QT4_WRAP_UI(LIB_UI_HEADERS
    ParallelCoordinatesConfigDialog.ui
    ParallelCoordsDrawConfigWidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.ui
)

QT4_WRAP_CPP(LIB_MOC_SRCS
      ParallelCoordinatesView.h
      ParallelCoordinatesConfigDialog.h
      AxisConfigDialogs.h
      ParallelCoordsDrawConfigWidget.h 
      ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.h
)

QT4_ADD_RESOURCES(LIB_RCC_SRCS
ParallelResource.qrc
)

ADD_LIBRARY(${PLUGIN_NAME}-${TULIP_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TULIP_VERSION} ${TULIP_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS ${PLUGIN_NAME}-${TULIP_VERSION} DESTINATION ${TULIP_VIEW_PLUGINS_DIR} COMPONENT ${PLUGIN_NAME})
