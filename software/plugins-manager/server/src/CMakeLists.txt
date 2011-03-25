FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

set(serverGenerator_SRCS
    PluginGenerate.cpp
    ServerGenerator.cpp
)

set(pluginInstaller_SRCS
    PluginGenerate.cpp
    PluginInstaller.cpp
)

set(pluginPusher_SRCS
    PluginGenerate.cpp
    PushToPluginServer.cpp
)

CONFIGURE_FILE("generate_plugin_dirs.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/generate_plugin_dirs.sh" @ONLY)
CONFIGURE_FILE("copy_plugin_libs.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/copy_plugin_libs.sh" @ONLY)
CONFIGURE_FILE("generate_plugin_doc.sh" "${CMAKE_CURRENT_BINARY_DIR}/generate_plugin_doc.sh" COPYONLY)
CONFIGURE_FILE("plugin_doc.doxygen" "${CMAKE_CURRENT_BINARY_DIR}/plugin_doc.doxygen" COPYONLY)


INCLUDE_DIRECTORIES(../include/ ${TulipBuildInclude} ${TulipQtBuildInclude} ${LIBXML2_INCLUDE_DIR} ${PROJECT_BINARY_DIR}/library/tulip-qt/include/tulip ${PROJECT_BINARY_DIR}/software/tulip/include ${TulipInclude} ${TulipQtInclude} ${TulipOGLInclude} ${TulipPluginsManagerInclude})

ADD_EXECUTABLE(serverGenerator ${serverGenerator_SRCS})
IF(QT_VERSION_MINOR LESS 7)
TARGET_LINK_LIBRARIES(serverGenerator
                      ${FREETYPE_LIBRARY}
                      ${GZStreamLibrary}
                      ${QT_LIBRARIES}
                      ${QT_QTASSISTANT_LIBRARY}
                      ${QT_QTASSISTANTCLIENT_LIBRARY}
                      ${QT_QTXML_LIBRARY}
                      ${QT_QTOPENGL_LIBRARY}
                      ${OPENGL_glu_LIBRARY}
                      ${OPENGL_gl_LIBRARY}
                      ${LIBXML2_LIBRARIES}
                      ${JPEG_LIBRARY}
                      ${LibTulipName}
                      ${LibTulipOglName}
                      ${LibTulipQtName}
                      ${LibTulipPluginsManagerName} )
ELSE(QT_VERSION_MINOR LESS 7)                     
TARGET_LINK_LIBRARIES(serverGenerator
                      ${FREETYPE_LIBRARY}
                      ${GZStreamLibrary}
                      ${QT_LIBRARIES}
                      ${QT_QTXML_LIBRARY}
                      ${QT_QTOPENGL_LIBRARY}
                      ${OPENGL_glu_LIBRARY}
                      ${OPENGL_gl_LIBRARY}
                      ${LIBXML2_LIBRARIES}
                      ${JPEG_LIBRARY}
                      ${LibTulipName}
                      ${LibTulipOglName}
                      ${LibTulipQtName}
                      ${LibTulipPluginsManagerName} ) 
ENDIF(QT_VERSION_MINOR LESS 7)
                      
ADD_DEPENDENCIES(serverGenerator ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName} ${LibTulipPluginsManagerName})

ADD_EXECUTABLE(pluginInstaller ${pluginInstaller_SRCS})
IF(QT_VERSION_MINOR LESS 7)
TARGET_LINK_LIBRARIES(pluginInstaller
                      ${FREETYPE_LIBRARY}
                      ${GZStreamLibrary}
                      ${QT_LIBRARIES}
                      ${QT_QTASSISTANT_LIBRARY}
                      ${QT_QTASSISTANTCLIENT_LIBRARY}
                      ${QT_QTXML_LIBRARY}
                      ${QT_QTOPENGL_LIBRARY}
                      ${OPENGL_glu_LIBRARY}
                      ${OPENGL_gl_LIBRARY}
                      ${LIBXML2_LIBRARIES}
                      ${JPEG_LIBRARY}
                      ${LibTulipName}
                      ${LibTulipOglName}
                      ${LibTulipQtName}
                      ${LibTulipPluginsManagerName} )
ELSE(QT_VERSION_MINOR LESS 7)
TARGET_LINK_LIBRARIES(pluginInstaller
                      ${FREETYPE_LIBRARY}
                      ${GZStreamLibrary}
                      ${QT_LIBRARIES}
                      ${QT_QTXML_LIBRARY}
                      ${QT_QTOPENGL_LIBRARY}
                      ${OPENGL_glu_LIBRARY}
                      ${OPENGL_gl_LIBRARY}
                      ${LIBXML2_LIBRARIES}
                      ${JPEG_LIBRARY}
                      ${LibTulipName}
                      ${LibTulipOglName}
                      ${LibTulipQtName}
                      ${LibTulipPluginsManagerName} )
ENDIF(QT_VERSION_MINOR LESS 7)                      	
                      
ADD_DEPENDENCIES(pluginInstaller ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName} ${LibTulipPluginsManagerName})

IF(QT_VERSION_MINOR GREATER 6)
QT4_WRAP_CPP(pluginPusher_MOC_SRCS PushToPluginServer.h)
ADD_EXECUTABLE(pluginPusher ${pluginPusher_SRCS} ${pluginPusher_MOC_SRCS})
target_link_libraries(pluginPusher ${QT_LIBRARIES} 
                      ${LibTulipName}
                      ${LibTulipOglName}
                      ${LibTulipQtName}
                      ${LibTulipPluginsManagerName} 
                      ${LIBXML2_LIBRARIES})
ENDIF(QT_VERSION_MINOR GREATER 6)                      	
