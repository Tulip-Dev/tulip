INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude}
                    ${TulipUIGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${GLEW_INCLUDE_DIRS})

QTX_SET_INCLUDES_AND_DEFINITIONS()

SET(LIB_SRCS
    GeographicView.cpp
    GeographicViewConfigWidget.cpp
    GeolocalisationConfigWidget.cpp
    GeographicViewInteractors.cpp
    AddressSelectionDialog.cpp
    GeoMapWidget.cpp
    GeographicViewGraphicsView.cpp
    GeographicViewShowElementInfo.cpp
    NominatimGeocoder.cpp)

QTX_WRAP_UI(LIB_UI_HEADERS
            GeolocalisationConfigWidget.ui
            GeographicViewConfigWidget.ui
            AddressSelectionDialog.ui)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
                  GeographicViewResource.qrc)


QTX_WRAP_CPP(LIB_MOC_SRCS
             GeographicView.h
             GeographicViewGraphicsView.h
             GeographicViewConfigWidget.h
             GeographicViewShowElementInfo.h
             GeolocalisationConfigWidget.h
             GeoMapWidget.h)

ADD_LIBRARY(GeographicView-${Tulip_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(GeographicView-${Tulip_VERSION}
    PUBLIC ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES}
                                                 INTERFACE OpenGL::GL)

TULIP_INSTALL_PLUGIN(GeographicView-${Tulip_VERSION} ${TulipViewPluginsInstallDir})
