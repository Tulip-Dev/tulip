cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(TULIP REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
SET(QT_USE_QTOPENGL true)
SET(QT_USE_QTSVG true)
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${TULIP_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/library/tulip-qt/include)

STRING(REGEX REPLACE ".*/" "" PLUGIN_NAME ${CMAKE_CURRENT_SOURCE_DIR})
STRING(TOLOWER ${PLUGIN_NAME} PLUGIN_NAME) 

SET(LIB_SRCS
library/tulip-qt/src/AbstractGraphicsView.cpp
library/tulip-qt/src/AnimatedGraphicsObject.cpp
library/tulip-qt/src/EmptyView.cpp
library/tulip-qt/src/FramedGraphicsProxyWidget.cpp
library/tulip-qt/src/GlMainWidgetGraphicsItem.cpp
library/tulip-qt/src/PushButtonItem.cpp
library/tulip-qt/src/ToolbarItem.cpp
library/tulip-qt/src/QtAnimationsManager.cpp
)

QT4_WRAP_CPP(LIB_MOC_SRCS
library/tulip-qt/include/tulip/AbstractGraphicsView.h
library/tulip-qt/include/tulip/AnimatedGraphicsObject.h
library/tulip-qt/include/tulip/GlMainWidgetGraphicsItem.h
library/tulip-qt/include/tulip/PushButtonItem.h
library/tulip-qt/include/tulip/ToolbarItem.h
library/tulip-qt/include/tulip/QtAnimationsManager.h
)


ADD_LIBRARY(${PLUGIN_NAME}-${TULIP_VERSION} SHARED ${LIB_SRCS} ${LIB_MOC_SRCS})

TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TULIP_VERSION}
${TULIP_CORE_LIBRARY}
${TULIP_OGL_LIBRARY}
${TULIP_GUI_LIBRARY}
${QT_QTOPENGL_LIBRARY}
${QT_LIBRARIES}
${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})

INSTALL(TARGETS ${PLUGIN_NAME}-${TULIP_VERSION} DESTINATION ${TULIP_PLUGINS_DIR} COMPONENT tulip_graphicsview)
