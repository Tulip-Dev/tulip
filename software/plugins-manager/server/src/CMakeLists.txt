set(pluginPusher_SRCS
    PushToPluginServer.cpp
)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipGUIInclude} ${TulipOGLInclude} ${TulipCoreBuildInclude})

QT4_WRAP_CPP(pluginPusher_MOC_SRCS PushToPluginServer.h)
ADD_EXECUTABLE(pluginPusher ${pluginPusher_SRCS} ${pluginPusher_MOC_SRCS})
target_link_libraries(pluginPusher ${QT_LIBRARIES}
                      ${QT_QTXML_LIBRARY}
                      ${LibTulipCoreName}
                      ${LibTulipOglName}
                      ${LibTulipGUIName}
                      "tulip-pluginsmanager-${TulipMMVersion}"
                      ${LIBXML2_LIBRARIES})

