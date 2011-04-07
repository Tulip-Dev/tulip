cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(TULIP3 REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
SET(QT_USE_QTOPENGL true)
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${TULIP_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/library/tulip-qt/include)

STRING(REGEX REPLACE ".*/" "" PLUGIN_NAME ${CMAKE_CURRENT_SOURCE_DIR})
STRING(TOLOWER ${PLUGIN_NAME} PLUGIN_NAME) 

SET(LIB_SRCS
library/tulip-qt/src/AbstractGraphicsView.cpp
library/tulip-qt/src/AnimatedGraphicsObject.cpp
library/tulip-qt/src/EmptyView.cpp
library/tulip-qt/src/GlMainWidgetGraphicsItem.cpp
library/tulip-qt/src/GraphicsItemsGraphMapper.cpp
library/tulip-qt/src/GraphicsLayoutItemGraphMapper.cpp
library/tulip-qt/src/HighlightGraphicsEffect.cpp
library/tulip-qt/src/InteractorToolbarItem.cpp
library/tulip-qt/src/MirrorGraphicsEffect.cpp
library/tulip-qt/src/PushButtonItem.cpp
library/tulip-qt/src/QtAnimationsManager.cpp
library/tulip-qt/src/ToolbarItem.cpp
library/tulip-qt/src/TulipGraphicsLayout.cpp
)

SET(LIB_UI
)

SET(LIB_MOC
library/tulip-qt/include/tulip/AbstractGraphicsView.h
library/tulip-qt/include/tulip/AnimatedGraphicsObject.h
library/tulip-qt/include/tulip/GlMainWidgetGraphicsItem.h
library/tulip-qt/include/tulip/InteractorToolbarItem.h
library/tulip-qt/include/tulip/PushButtonItem.h
library/tulip-qt/include/tulip/QtAnimationsManager.h
library/tulip-qt/include/tulip/ToolbarItem.h
)

SET(LIB_RCC
library/tulip-qt/designer/tulipgv.qrc
)

TULIP_QT4_WRAP_UI(LIB_UI_SRCS ${LIB_UI})
TULIP_QT4_WRAP_CPP(LIB_MOC_SRCS ${LIB_MOC})
QT4_ADD_RESOURCES(LIB_RCC_SRCS ${LIB_RCC})

ADD_LIBRARY(${PLUGIN_NAME}-${TULIP_PLUGIN_VERSION} SHARED ${LIB_SRCS} ${LIB_UI_SRCS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})

TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TULIP_PLUGIN_VERSION}
${TULIP_LIBRARY}
${TULIP_LIBRARY}
${TULIP_OGL_LIBRARY}
${TULIP_QT4_LIBRARY}
${QT_QTOPENGL_LIBRARY}
${QT_LIBRARIES}
${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})

INSTALL(TARGETS ${PLUGIN_NAME}-${TULIP_PLUGIN_VERSION} DESTINATION ${TULIP_PLUGINS_PATH} COMPONENT tulip_graphicsview)
