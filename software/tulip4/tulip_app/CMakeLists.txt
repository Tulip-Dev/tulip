SET(SRCS
src/AboutTulipPage.cpp
src/main.cpp
src/PluginErrorReport.cpp
#src/PluginManager.cpp
src/PluginsCenter.cpp
src/RssParser.cpp
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
include/TulipMainWindow.h
include/TulipWelcomePage.h)

QT4_ADD_RESOURCES(RCC_SRCS resources/TulipApp.qrc)

INCLUDE_DIRECTORIES(include ${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${QT_INCLUDES})

ADD_EXECUTABLE(tulip_app4 ${SRCS} ${UI_HEADERS} ${MOC_SRCS} ${RCC_SRCS})

TARGET_LINK_LIBRARIES(tulip_app4 ${LibTulipGUIName} ${QT_LIBRARIES})

INSTALL(TARGETS tulip_app4
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)
