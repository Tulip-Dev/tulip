QT4_WRAP_UI(UI_HEADERS
  ../designer/PluginsInfoWidget.ui
  ../designer/InstallPluginDialog.ui
  ../designer/ProxyConfigurationDialog.ui
  ../designer/UpdatePluginsDialog.ui
  ../designer/NewTulipVersionAvailableDialog.ui
)

QT4_WRAP_CPP(MOC_SRCS
  ../include/PluginsManager.h
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

SET(SRCS
  PluginsManager.cpp
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

INCLUDE_DIRECTORIES(../include/ ${TulipInclude} ${TulipBuildInclude} ${CMAKE_CURRENT_BINARY_DIR} ${TulipQtInclude})

# tulip_check_pl
ADD_EXECUTABLE(tulip_check_pl TulipCheckPluginLoad.cpp)
TARGET_LINK_LIBRARIES(tulip_check_pl ${LibTulipName} ${CMAKE_DL_LIBS})
TARGET_LINK_LIBRARIES(tulip_check_pl ${LibTulipQtName} )
TARGET_LINK_LIBRARIES(tulip_check_pl ${LibTulipPluginsManagerName})

# pluginsmanager library
ADD_LIBRARY(${LibTulipPluginsManagerName} STATIC ${SRCS} ${UI_HEADERS} ${MOC_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipPluginsManagerName} ${QT_QTNETWORK_LIBRARY})

IF(${MSVC})
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/\${BUILD_TYPE}/tulip_check_pl.exe DESTINATION "${TulipBinInstallDir}" COMPONENT tulip_pluginsmanager)
ELSE()
  INSTALL(TARGETS tulip_check_pl DESTINATION ${TulipBinInstallDir} COMPONENT tulip_pluginsmanager)
ENDIF()
