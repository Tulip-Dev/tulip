INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
    GeographicView.cpp
    GeographicViewConfigWidget.cpp
    GeolocalisationConfigWidget.cpp
    ProgressWidgetGraphicsProxy.cpp
    GeographicViewInteractors.cpp
    AddressSelectionDialog.cpp
    GoogleMaps.cpp
    GeographicViewGraphicsView.cpp
    GeographicViewShowElementInfo.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    GeolocalisationConfigWidget.ui
    GeographicViewConfigWidget.ui
    ProgressWidget.ui
    AddressSelectionDialog.ui
    ElementInformationsWidget.ui
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    GeographicViewResource.qrc
)

IF(QT_HAS_WEBKIT)
  SET(QT_WEB_DEFINITIONS -DQT_HAS_WEBKIT)
ELSE(QT_HAS_WEBKIT)
  SET(QT_WEB_DEFINITIONS -DQT_HAS_WEBENGINE)
ENDIF(QT_HAS_WEBKIT)

ADD_DEFINITIONS(${QT_WEB_DEFINITIONS})

QTX_WRAP_CPP(LIB_MOC_SRCS
    GeographicView.h
    GeographicViewGraphicsView.h
    ProgressWidgetGraphicsProxy.h
    GeographicViewConfigWidget.h
    GeolocalisationConfigWidget.h
    GoogleMaps.h
    OPTIONS ${QT_WEB_DEFINITIONS}
)

ADD_LIBRARY(GeographicView-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(GeographicView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${OPENGL_gl_LIBRARY} ${QT_LIBRARIES})

INSTALL_TULIP_PLUGIN(GeographicView-${TulipVersion} ${TulipViewPluginsInstallDir})
