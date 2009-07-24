INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})	

SET(parallel_LIB_SRCS
    ParallelCoordinatesView.h 
    ParallelCoordinatesDrawing.h 
    ParallelCoordinatesGraphProxy.h 
    ParallelAxis.h 
    ParallelTools.h 
    ParallelCoordinatesConfigDialog.h 
    NominalParallelAxis.h 
    QuantitativeParallelAxis.h 
    AxisConfigDialogs.h 
    ParallelCoordinatesInteractors.h 
    ParallelCoordsElementHighlighter.h 
    ParallelCoordsAxisSwapper.h 
    ParallelCoordinatesView.cpp 
    ParallelCoordinatesDrawing.cpp 
    ParallelCoordinatesGraphProxy.cpp 
    ParallelAxis.cpp 
    ParallelCoordinatesConfigDialog.cpp 
    NominalParallelAxis.cpp 
    AxisConfigDialogs.cpp  
    QuantitativeParallelAxis.cpp 
    ParallelCoordsElementShowInfos.h 
    ParallelCoordsElementShowInfos.cpp 
    ParallelCoordsElementsSelector.h 
    ParallelCoordsElementsSelector.cpp 
    ParallelCoordsGlEntitiesSelector.h 
    ParallelCoordsGlEntitiesSelector.cpp 
    ParallelCoordsElementDeleter.cpp 
    ParallelCoordsElementDeleter.h 
    ParallelCoordsElementHighlighter.cpp 
    ParallelCoordsElementHighlighter.h 
    ParallelCoordsAxisSwapper.h 
    ParallelCoordsAxisSwapper.cpp 
    ParallelCoordsAxisSliders.h 
    ParallelCoordsAxisSliders.cpp 
    ParallelCoordsAxisBoxPlot.h 
    ParallelCoordsAxisBoxPlot.cpp 
    ParallelCoordinatesInteractors.cpp 
    ParallelCoordinatesInteractors.h 
    ParallelTools.h 
    ParallelTools.cpp 
    ParallelCoordsDataConfigWidget.cpp 
    ParallelCoordsDataConfigWidget.h 
    ParallelCoordsDrawConfigWidget.cpp 
    ParallelCoordsDrawConfigWidget.h 
)

set(parallel_UI
    ParallelCoordinatesConfigDialog.ui
    ParallelCoordsDataConfigWidget.ui
    ParallelCoordsDrawConfigWidget.ui
)

set(parallel_MOC
    ParallelCoordinatesView.h
    ParallelCoordinatesConfigDialog.h
    AxisConfigDialogs.h
    ParallelCoordsDrawConfigWidget.h
)

set(parallel_RCC
    ParallelResource.qrc )

TULIP_QT4_WRAP_UI(parallel_UI_SRCS ${parallel_UI})
TULIP_QT4_WRAP_CPP(parallel_MOC_SRCS ${parallel_MOC})
QT4_ADD_RESOURCES(parallel_RCC_SRCS ${parallel_RCC})

ADD_LIBRARY(ParallelCoordinates-${TulipVersion} SHARED ${parallel_UI_SRCS} ${parallel_MOC_SRCS} ${parallel_LIB_SRCS} ${parallel_RCC_SRCS})
TARGET_LINK_LIBRARIES(ParallelCoordinates-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(ParallelCoordinates-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(ParallelCoordinates-${TulipVersion} ${LibTulipQtName})
ADD_DEPENDENCIES(ParallelCoordinates-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

INSTALL(TARGETS ParallelCoordinates-${TulipVersion} DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/view/)
