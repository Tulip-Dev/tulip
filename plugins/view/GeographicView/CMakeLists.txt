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

QT4_WRAP_UI(LIB_UI_HEADERS
    GeolocalisationConfigWidget.ui
    GoogleMapsViewConfigWidget.ui
    ProgressWidget.ui
    AddressSelectionDialog.ui
    ElementInformationsWidget.ui
)

QT4_WRAP_CPP(LIB_MOC_SRCS
    GoogleMapsView.h
    ProgressWidgetGraphicsProxy.h
    GoogleMapsViewConfigWidget.h
    GeolocalisationConfigWidget.h
    GoogleMaps.h
    GoogleMapsShowElementInfo.h
)

QT4_ADD_RESOURCES(LIB_RCC_SRCS
    GoogleMapViewResource.qrc
)

ADD_LIBRARY(GeographicView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(GeographicView-${TulipVersion} ${TULIP_LIBRARIES} ${QT_LIBRARIES} ${LIBXML2_LIBRARIES} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY})

INSTALL(TARGETS GeographicView-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT GeographicView)
