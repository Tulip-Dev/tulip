INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipGUIInclude} ${TulipOGLInclude} ${TulipCoreBuildInclude} ${QuazipInclude})

IF(USE_QT5)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
ENDIF(USE_QT5)

QTX_WRAP_CPP(pluginPusher_MOC_SRCS WebDavManager.h)
ADD_EXECUTABLE(pushPlugins PluginPusher.cpp WebDavManager.cpp ${pluginPusher_MOC_SRCS})
TARGET_LINK_LIBRARIES(pushPlugins ${LibTulipGUIName} ${QT_LIBRARIES})


QTX_ADD_RESOURCES(RCC_SRCS pluginpackager.qrc)
ADD_EXECUTABLE(packagePlugins PluginPackager.cpp ${RCC_SRCS})

# needs to link against all Tulip libs to be able to load all tulip plugins
TARGET_LINK_LIBRARIES(packagePlugins
                      ${LibTulipCoreName}
                      ${LibTulipGUIName}
                      ${LibTulipOglName}
                      ${LibTulipOGDFName}
                      ${OGDFLibrary}
                      ${QT_LIBRARIES}
                      ${QuazipLibrary})

IF(NOT WIN32)
  TARGET_LINK_LIBRARIES(packagePlugins pthread)
ENDIF()

IF(TULIP_GENERATE_PLUGINSERVER_TOOLS)
  INSTALL(TARGETS packagePlugins DESTINATION ${TulipBinInstallDir} COMPONENT plugin_server)
  INSTALL(TARGETS pushPlugins DESTINATION ${TulipBinInstallDir} COMPONENT plugin_server)
ENDIF()
