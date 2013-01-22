INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipGUIInclude} ${TulipOGLInclude} ${TulipCoreBuildInclude} ${QuazipInclude} ${QT_INCLUDE_DIR})

QT4_WRAP_CPP(pluginPusher_MOC_SRCS WebDavManager.h)
ADD_EXECUTABLE(pushPlugins PluginPusher.cpp WebDavManager.cpp ${pluginPusher_MOC_SRCS})
target_link_libraries(pushPlugins ${QT_LIBRARIES}
                      ${LibTulipGUIName}
                      ${QT_QTXML_LIBRARY})


QT4_ADD_RESOURCES(RCC_SRCS pluginpackager.qrc)
ADD_EXECUTABLE(packagePlugins PluginPackager.cpp ${RCC_SRCS})

# needs to link against all Tulip libs to be able to load all tulip plugins
target_link_libraries(packagePlugins ${QT_LIBRARIES}
                      ${QT_QTXML_LIBRARY}
                      ${LibTulipCoreName}
                      ${LibTulipGUIName}
                      ${LibTulipOglName}
                      ${LibTulipOGDFName}
                      ${OGDFLibrary}
                      quazip)

IF(NOT WIN32)
  target_link_libraries(packagePlugins pthread)
ENDIF()

IF(TULIP_GENERATE_PLUGINSERVER_TOOLS)
  INSTALL(TARGETS packagePlugins DESTINATION ${TulipBinInstallDir} COMPONENT plugin_server)
  INSTALL(TARGETS pushPlugins DESTINATION ${TulipBinInstallDir} COMPONENT plugin_server)
ENDIF()
