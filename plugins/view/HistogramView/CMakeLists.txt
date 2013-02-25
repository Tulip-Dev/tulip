INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
    HistogramView.cpp
    HistoStatsConfigWidget.cpp
    HistogramMetricMapping.cpp
    HistogramStatistics.cpp    
    Histogram.cpp
    HistogramViewNavigator.cpp
    HistogramInteractors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.cpp
    HistoOptionsWidget.cpp
    SizeScaleConfigDialog.cpp
    GlyphScaleConfigDialog.cpp
)

QT4_WRAP_UI(LIB_UI_HEADERS
   ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.ui
	HistoStatsConfigWidget.ui
	HistoOptionsWidget.ui
	SizeScaleConfigDialog.ui
	GlyphScaleConfigDialog.ui
)

QT4_WRAP_CPP(LIB_MOC_SRCS
      HistogramView.h
      HistogramStatistics.h
      HistoOptionsWidget.h
      SizeScaleConfigDialog.h
      GlyphScaleConfigDialog.h  
      ${CMAKE_CURRENT_SOURCE_DIR}/../utils/ViewGraphPropertiesSelectionWidget.h
)

QT4_ADD_RESOURCES(LIB_RCC_SRCS
HistogramResource.qrc
)

ADD_LIBRARY(HistogramView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(HistogramView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS HistogramView-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT HistogramView)
