FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

set(tulip_UI
    ../designer/Tulip.ui
    ../designer/AppStartUp.ui
    ../designer/InfoDialog.ui)

set(tulip_MOC
    ../include/AppStartUp.h
    ../include/InfoDialog.h
    ../include/viewGl.h)

set(tulip_SRCS 
    viewGl.cpp
    Application.cpp
    geometrie2D.cpp
    InfoDialog.cpp
    AppStartUp.cpp
    main.cpp)

SET(tulip_RCC
    ../designer/TulipResource.qrc
)

TULIP_QT4_WRAP_UI_IN_INCLUDE(tulip_UI_SRCS ${tulip_UI})
TULIP_QT4_WRAP_CPP(tulip_MOC_SRCS ${tulip_MOC})
QT4_ADD_RESOURCES(tulip_RCC_SRCS ${tulip_RCC})

INCLUDE_DIRECTORIES(../include/ ${TulipInclude} ${TulipQtInclude} ${TulipOGLInclude} ${TulipPluginsManagerInclude})

ADD_EXECUTABLE( tulip-exe ${tulip_UI_SRCS} ${tulip_MOC_SRCS} ${tulip_SRCS} ${tulip_RCC_SRCS})
SET_TARGET_PROPERTIES(tulip-exe PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}")
TARGET_LINK_LIBRARIES( tulip-exe ${FREETYPE_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${GZStreamLibrary} )
TARGET_LINK_LIBRARIES( tulip-exe ${QT_LIBRARIES} )
TARGET_LINK_LIBRARIES( tulip-exe ${QT_QTASSISTANT_LIBRARY})	
TARGET_LINK_LIBRARIES( tulip-exe ${QT_QTASSISTANTCLIENT_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${QT_QTXML_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${QT_QTOPENGL_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${OPENGL_glu_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${OPENGL_gl_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${LIBXML2_LIBRARIES} )
TARGET_LINK_LIBRARIES( tulip-exe ${JPEG_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe tulip )
TARGET_LINK_LIBRARIES( tulip-exe tulip-ogl )
TARGET_LINK_LIBRARIES( tulip-exe tulip-qt4 )
TARGET_LINK_LIBRARIES( tulip-exe tulip-pluginsmanager )
ADD_DEPENDENCIES(tulip-exe tulip tulip-ogl tulip-qt4 tulip-pluginsmanager)

INSTALL(PROGRAMS tulip-exe 
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipBinInstallDir}" 
	RENAME tulip
)


