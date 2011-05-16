set(pluginPusher_SRCS
    PushToPluginServer.cpp
)

INCLUDE_DIRECTORIES(${TulipInclude} ${TulipQtInclude} ${TulipOGLInclude} ${TulipBuildInclude})

QT4_WRAP_CPP(pluginPusher_MOC_SRCS PushToPluginServer.h)
ADD_EXECUTABLE(pluginPusher ${pluginPusher_SRCS} ${pluginPusher_MOC_SRCS})
target_link_libraries(pluginPusher ${QT_LIBRARIES}
                      ${QT_QTXML_LIBRARY}
                      ${LibTulipName}
                      ${LibTulipOglName}
                      ${LibTulipQtName}
                      ${LibTulipPluginsManagerName} 
                      ${LIBXML2_LIBRARIES})

