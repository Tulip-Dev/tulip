SET(SRCS
src/AboutTulipPage.cpp
src/main.cpp
src/PluginErrorReport.cpp
src/PluginLoaderDispatcher.cpp
src/PluginLoaderReporter.cpp
src/PluginsCenter.cpp
src/RssParser.cpp
src/TulipAgentServiceAdaptor.cpp
src/TulipMainWindow.cpp
src/TulipSplashScreen.cpp
src/TulipWelcomePage.cpp)

QT4_WRAP_UI(UI_HEADERS
designer/AboutTulipPage.ui
designer/PluginErrorReport.ui
designer/PluginsCenter.ui
designer/TulipMainWindow.ui
designer/TulipWelcomePage.ui)

QT4_WRAP_CPP(MOC_SRCS
include/PluginErrorReport.h
include/PluginsCenter.h
include/TulipAgentServiceAdaptor.h
include/TulipMainWindow.h
include/TulipWelcomePage.h)

QT4_ADD_RESOURCES(RCC_SRCS resources/TulipAgent.qrc)

INCLUDE_DIRECTORIES(include ${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${QT_INCLUDE_DIR})

ADD_EXECUTABLE(tulip_agent ${SRCS} ${UI_HEADERS} ${MOC_SRCS} ${RCC_SRCS})

IF(LINUX)
  SET_TARGET_PROPERTIES(tulip_agent PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
ELSEIF(APPLE)
  SET_TARGET_PROPERTIES(tulip_agent PROPERTIES INSTALL_RPATH "$ORIGIN/../Frameworks")
ELSE()
  SET_TARGET_PROPERTIES(tulip_agent PROPERTIES INSTALL_RPATH "$ORIGIN")
ENDIF()

TARGET_LINK_LIBRARIES(tulip_agent ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES} ${QT_QTDBUS_LIBRARY})

INSTALL(TARGETS tulip_agent
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)
