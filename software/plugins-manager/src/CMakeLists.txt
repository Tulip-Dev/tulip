FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

set(${LibTulipPluginsManagerName}_UI
     ../designer/PluginsInfoWidget.ui
     ../designer/InstallPluginDialog.ui
     ../designer/ProxyConfigurationDialog.ui
     ../designer/UpdatePluginsDialog.ui
     ../designer/NewTulipVersionAvailableDialog.ui
)

set(${LibTulipPluginsManagerName}_MOC
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
    ../include/NewTulipVersionAvailableDialog.h
)

set(${LibTulipPluginsManagerName}_LIB_SRCS 
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
    PluginInfo.cpp
    NewTulipVersionAvailableDialog.cpp
)

set(tulip_check_pl_SRCS TulipCheckPluginLoad.cpp)

TULIP_QT4_WRAP_UI_IN_INCLUDE(${LibTulipPluginsManagerName}_UI_SRCS ${${LibTulipPluginsManagerName}_UI})
TULIP_QT4_WRAP_CPP(${LibTulipPluginsManagerName}_MOC_SRCS ${${LibTulipPluginsManagerName}_MOC})

INCLUDE_DIRECTORIES(../include/ ${TulipInclude} ${TulipBuildInclude} ${PROJECT_BINARY_DIR}/software/plugins-manager/include ${TulipQtInclude})

ADD_EXECUTABLE( tulip_check_pl ${tulip_check_pl_SRCS})
TARGET_LINK_LIBRARIES(tulip_check_pl ${LibTulipName} ${CMAKE_DL_LIBS})
ADD_DEPENDENCIES(tulip_check_pl ${LibTulipName})

ADD_LIBRARY(${LibTulipPluginsManagerName} SHARED ${${LibTulipPluginsManagerName}_UI_SRCS} ${${LibTulipPluginsManagerName}_MOC_SRCS} ${${LibTulipPluginsManagerName}_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipPluginsManagerName} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipPluginsManagerName} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipPluginsManagerName} ${QT_QTXML_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipPluginsManagerName} ${QT_QTNETWORK_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipPluginsManagerName} ${LibTulipName})
TARGET_LINK_LIBRARIES(${LibTulipPluginsManagerName} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(${LibTulipPluginsManagerName} ${LibTulipQtName})
ADD_DEPENDENCIES(${LibTulipPluginsManagerName} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

IF(WIN32)
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tulip_check_pl.exe 
        DESTINATION ${TulipBinInstallDir}
        COMPONENT tulip_pluginsmanager
)
ELSE(WIN32)
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tulip_check_pl
        DESTINATION ${TulipBinInstallDir}
        COMPONENT tulip_pluginsmanager
)
ENDIF(WIN32)
INSTALL(TARGETS ${LibTulipPluginsManagerName}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_pluginsmanager
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_pluginsmanager
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_pluginsmanager)
