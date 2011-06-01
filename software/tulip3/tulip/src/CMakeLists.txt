QT4_WRAP_UI(UI_HEADERS
  ../designer/Tulip.ui
  ../designer/AppStartUp.ui
  ../designer/InfoDialog.ui
  ../designer/PreferenceDialog.ui
  ../designer/ChooseControllerDialog.ui)

QT4_WRAP_CPP(MOC_SRCS
  ../include/AppStartUp.h
  ../include/InfoDialog.h
  ../include/TulipApp.h
  ../include/PreferenceDialog.h
  ../include/ChooseControllerDialog.h)

SET(SRCS
  TulipApp.cpp
  InfoDialog.cpp
  AppStartUp.cpp
  PreferenceDialog.cpp
  ChooseControllerDialog.cpp
  main.cpp)

QT4_ADD_RESOURCES(RCC_SRCS ../designer/TulipResource.qrc)

INCLUDE_DIRECTORIES(../include/ ${TulipCoreBuildInclude} ${TulipGUIBuildInclude} ${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreInclude} ${TulipGUIInclude} ${TulipOGLInclude} ${PROJECT_SOURCE_DIR}/software/plugins-manager/include/)

ADD_DEFINITIONS("-DQT_MINOR_REL=${QT_VERSION_MINOR}")

# Tulip launcher (Windows only)
IF(WIN32)
  ADD_EXECUTABLE(tulip TulipLauncher.cpp)
  SET_TARGET_PROPERTIES(tulip PROPERTIES INSTALL_RPATH ${TulipLibInstallDir})
  IF(NOT ${MSVC})
    TARGET_LINK_LIBRARIES(tulip -mwindows)
  ENDIF()
  TARGET_LINK_LIBRARIES(tulip ${QT_LIBRARIES})
ENDIF(WIN32)

# Tulip need restart executable
ADD_EXECUTABLE(tulip_need_restart TulipNeedRestart.cpp)
SET_TARGET_PROPERTIES(tulip_need_restart PROPERTIES INSTALL_RPATH ${TulipLibInstallDir})
TARGET_LINK_LIBRARIES( tulip_need_restart ${QT_LIBRARIES} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

#Tulip app
ADD_EXECUTABLE(tulip_app ${UI_HEADERS} ${MOC_SRCS} ${SRCS} ${RCC_SRCS})
SET_TARGET_PROPERTIES(tulip_app PROPERTIES INSTALL_RPATH ${TulipLibInstallDir})
IF(WIN32 AND NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio" AND NOT ${CMAKE_GENERATOR} MATCHES "NMake Makefiles")
  TARGET_LINK_LIBRARIES(tulip_app -mwindows)
ENDIF()

TARGET_LINK_LIBRARIES( tulip_app ${FREETYPE_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip_app ${GZStreamLibrary} )
TARGET_LINK_LIBRARIES( tulip_app ${QT_LIBRARIES} )
IF(QT_VERSION_MINOR LESS 7)
  TARGET_LINK_LIBRARIES( tulip_app ${QT_QTASSISTANT_LIBRARY})
  TARGET_LINK_LIBRARIES( tulip_app ${QT_QTASSISTANTCLIENT_LIBRARY} )
ENDIF(QT_VERSION_MINOR LESS 7)
TARGET_LINK_LIBRARIES( tulip_app ${QT_QTOPENGL_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip_app ${OPENGL_glu_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip_app ${OPENGL_gl_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip_app ${LIBXML2_LIBRARIES} )
TARGET_LINK_LIBRARIES( tulip_app ${JPEG_LIBRARY})
TARGET_LINK_LIBRARIES( tulip_app ${CMAKE_DL_LIBS})
TARGET_LINK_LIBRARIES( tulip_app "tulip-pluginsmanager-${TulipMMVersion}")
TARGET_LINK_LIBRARIES( tulip_app ${QT_QTXML_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip_app ${LibTulipCoreName})
TARGET_LINK_LIBRARIES( tulip_app ${LibTulipOglName})
TARGET_LINK_LIBRARIES( tulip_app ${LibTulipGUIName})

IF(WIN32)
  IF(${CMAKE_GENERATOR} MATCHES "Visual Studio") #if generating for visual studio, we assume the compiler will be vc++
    INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/\${BUILD_TYPE}/tulip.exe
            DESTINATION "${TulipBinInstallDir}"
            COMPONENT tulip_app)
    INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/\${BUILD_TYPE}/tulip_app.exe
            DESTINATION "${TulipBinInstallDir}"
            COMPONENT tulip_app)
  ELSE()
    INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tulip.exe
            DESTINATION "${TulipBinInstallDir}"
            COMPONENT tulip_app)
    INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tulip_app.exe
            DESTINATION "${TulipBinInstallDir}"
            COMPONENT tulip_app)
  ENDIF()
ELSE(WIN32)
  INSTALL(PROGRAMS
    ${CMAKE_CURRENT_SOURCE_DIR}/tulip.sh
    DESTINATION ${TulipBinInstallDir}
    RENAME tulip
    COMPONENT tulip_app)
  INSTALL(PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/tulip_app
    DESTINATION ${TulipBinInstallDir}
    COMPONENT tulip_app)
  INSTALL(PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/tulip_need_restart
    DESTINATION ${TulipBinInstallDir}
    COMPONENT tulip_app)
ENDIF(WIN32)


