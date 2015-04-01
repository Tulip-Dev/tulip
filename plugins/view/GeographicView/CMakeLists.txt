INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
    GoogleMapsView.cpp
    GoogleMapsViewConfigWidget.cpp
    GeolocalisationConfigWidget.cpp
    ProgressWidgetGraphicsProxy.cpp
    GoogleMapsViewInteractors.cpp
    AddressSelectionDialog.cpp
    GoogleMaps.cpp
    GoogleMapsGraphicsView.cpp
    GoogleMapsShowElementInfo.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    GeolocalisationConfigWidget.ui
    GoogleMapsViewConfigWidget.ui
    ProgressWidget.ui
    AddressSelectionDialog.ui
    ElementInformationsWidget.ui
)

QTX_WRAP_CPP(LIB_MOC_SRCS
    GoogleMapsView.h
    GoogleMapsGraphicsView.h
    ProgressWidgetGraphicsProxy.h
    GoogleMapsViewConfigWidget.h
    GeolocalisationConfigWidget.h
    GoogleMaps.h
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    GoogleMapViewResource.qrc
)

ADD_LIBRARY(GeographicView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(GeographicView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS GeographicView-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT GeographicView)
