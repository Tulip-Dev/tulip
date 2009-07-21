FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

set(tulip-pluginsmanager_UI
     ../designer/PluginsInfoWidget.ui
     ../designer/InstallPluginDialog.ui
     ../designer/ProxyConfigurationDialog.ui
     ../designer/UpdatePluginsDialog.ui
)

set(tulip-pluginsmanager_MOC
    ../include/PluginsManagerMainWindow.h
    ../include/PluginsManagerDialog.h
    ../include/PluginsWidget.h
    ../include/PluginsViewWidget.h
    ../include/Server.h
    ../include/MultiServerManager.h
    ../include/HttpRequest.h
    ../include/PluginsInfoWidget.h
    ../include/UpdatePlugin.h
    ../include/InstallPluginDialog.h
    ../include/Request.h
    ../include/InstallExitDialog.h
    ../include/AuthorizationInstallDependencies.h
    ../include/ChooseServerDialog.h
    ../include/ServersOptionDialog.h
    ../include/ModifyServerDialog.h
    ../include/PluginsUpdateChecker.h
    ../include/ProxyConfigurationDialog.h
    ../include/UpdatePluginsDialog.h
)

set(tulip-pluginsmanager_LIB_SRCS 
    PluginsManagerMainWindow.cpp 
    PluginsManagerDialog.cpp 
    PluginsWidget.cpp 
    PluginsViewWidget.cpp 
    Server.cpp 
    MultiServerManager.cpp 
    SoapResponseReader.cpp 
    SoapRequestBuilder.cpp 
    HttpRequest.cpp 
    PluginsListManager.cpp 
    Request.cpp 
    PluginsInfoWidget.cpp 
    UpdatePlugin.cpp 
    InstallPluginDialog.cpp 
    InstallExitDialog.cpp 
    AuthorizationInstallDependencies.cpp 
    ChooseServerDialog.cpp 
    ServersOptionDialog.cpp 
    ModifyServerDialog.cpp 
    PluginsUpdateChecker.cpp 
    ProxyConfigurationDialog.cpp 
    UpdatePluginsDialog.cpp 
    PluginsHelp.cpp
)

TULIP_QT4_WRAP_UI_IN_INCLUDE(tulip-pluginsmanager_UI_SRCS ${tulip-pluginsmanager_UI})
TULIP_QT4_WRAP_CPP(tulip-pluginsmanager_MOC_SRCS ${tulip-pluginsmanager_MOC})

INCLUDE_DIRECTORIES(../include/ ${TulipInclude} ${TulipQtInclude})

ADD_LIBRARY(tulip-pluginsmanager SHARED ${tulip-pluginsmanager_UI_SRCS} ${tulip-pluginsmanager_MOC_SRCS} ${tulip-pluginsmanager_LIB_SRCS})
TARGET_LINK_LIBRARIES(tulip-pluginsmanager ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(tulip-pluginsmanager ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(tulip-pluginsmanager ${QT_QTXML_LIBRARY})
TARGET_LINK_LIBRARIES(tulip-pluginsmanager ${QT_QTNETWORK_LIBRARY})
TARGET_LINK_LIBRARIES(tulip-pluginsmanager tulip)
TARGET_LINK_LIBRARIES(tulip-pluginsmanager tulip-ogl)
TARGET_LINK_LIBRARIES(tulip-pluginsmanager tulip-qt4)
ADD_DEPENDENCIES(tulip-pluginsmanager tulip tulip-ogl tulip-qt4)
INSTALL_TARGETS(${TulipLibInstallDir} tulip-pluginsmanager)
