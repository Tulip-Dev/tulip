FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

set(tulip_UI
    ../designer/Tulip.ui
    ../designer/AppStartUp.ui
    ../designer/InfoDialog.ui
    ../designer/PreferenceDialog.ui)

set(tulip_MOC
    ../include/AppStartUp.h
    ../include/InfoDialog.h
    ../include/TulipApp.h
    ../include/PreferenceDialog.h)

set(tulip_SRCS 
    TulipApp.cpp
    Application.cpp
    geometrie2D.cpp
    InfoDialog.cpp
    AppStartUp.cpp
    PreferenceDialog.cpp
    main.cpp)

SET(tulip_RCC
    ../designer/TulipResource.qrc
)

TULIP_QT4_WRAP_UI_IN_INCLUDE(tulip_UI_SRCS ${tulip_UI})
TULIP_QT4_WRAP_CPP(tulip_MOC_SRCS ${tulip_MOC})
QT4_ADD_RESOURCES(tulip_RCC_SRCS ${tulip_RCC})

INCLUDE_DIRECTORIES(../include/ ${TulipBuildInclude} ${TulipQtBuildInclude} ${PROJECT_BINARY_DIR}/library/tulip-qt/include/tulip ${PROJECT_BINARY_DIR}/software/tulip/include ${TulipInclude} ${TulipQtInclude} ${TulipOGLInclude} ${TulipPluginsManagerInclude})

ADD_EXECUTABLE( tulip ${tulip_UI_SRCS} ${tulip_MOC_SRCS} ${tulip_SRCS} ${tulip_RCC_SRCS})
SET_TARGET_PROPERTIES(tulip PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}")
TARGET_LINK_LIBRARIES( tulip ${FREETYPE_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip ${GZStreamLibrary} )
TARGET_LINK_LIBRARIES( tulip ${QT_LIBRARIES} )
TARGET_LINK_LIBRARIES( tulip ${QT_QTASSISTANT_LIBRARY})	
TARGET_LINK_LIBRARIES( tulip ${QT_QTASSISTANTCLIENT_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip ${QT_QTXML_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip ${QT_QTOPENGL_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip ${OPENGL_glu_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip ${OPENGL_gl_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip ${LIBXML2_LIBRARIES} )
TARGET_LINK_LIBRARIES( tulip ${JPEG_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip ${LibTulipName})
TARGET_LINK_LIBRARIES( tulip ${LibTulipOglName})
TARGET_LINK_LIBRARIES( tulip ${LibTulipQtName})
TARGET_LINK_LIBRARIES( tulip ${LibTulipPluginsManagerName})
ADD_DEPENDENCIES(tulip ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName} ${LibTulipPluginsManagerName})

IF(WIN32)
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tulip.exe 
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipBinInstallDir}" 
)
ELSE(WIN32)
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tulip
        DESTINATION "${CMAKE_INSTALL_PREFIX}${TulipBinInstallDir}" 
)
ENDIF(WIN32)


