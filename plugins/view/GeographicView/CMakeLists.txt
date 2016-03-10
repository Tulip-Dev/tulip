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

  SET(QT_WEB_INCLUDES ${Qt5WebKit_INCLUDE_DIRS} ${Qt5WebKitWidgets_INCLUDE_DIRS})
  SET(QT_WEB_DEFINITIONS ${Qt5WebKit_DEFINITIONS} ${Qt5WebKitWidgets_DEFINITIONS} -DQT_HAS_WEBKIT)
  SET(QT_WEB_LIBRARIES ${Qt5WebKit_LIBRARIES} ${Qt5WebKitWidgets_LIBRARIES})

ELSE(QT_HAS_WEBKIT)

  SET(QT_WEB_INCLUDES ${Qt5WebEngineWidgets_INCLUDE_DIRS} ${Qt5WebChannel_INCLUDE_DIRS})
  SET(QT_WEB_DEFINITIONS ${Qt5WebEngineWidgets_DEFINITIONS} ${Qt5WebChannel_DEFINITIONS} -DQT_HAS_WEBENGINE)
  SET(QT_WEB_LIBRARIES ${Qt5WebEngineWidgets_LIBRARIES} ${Qt5WebChannel_LIBRARIES})

ENDIF(QT_HAS_WEBKIT)

INCLUDE_DIRECTORIES(${QT_WEB_INCLUDES})
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
TARGET_LINK_LIBRARIES(GeographicView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${OPENGL_gl_LIBRARY} ${QT_WEB_LIBRARIES})

INSTALL_TULIP_PLUGIN(GeographicView-${TulipVersion} ${TulipViewPluginsInstallDir})
