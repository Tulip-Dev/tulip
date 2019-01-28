SET(SRCS
  src/main.cpp
  src/PerspectiveItemWidget.cpp
  src/PluginErrorReport.cpp
  src/PluginsCenter.cpp
  src/PluginInformationListItem.cpp
  src/TulipMainWindow.cpp
  src/TulipSplashScreen.cpp
  src/TulipWelcomePage.cpp
  src/TulipPerspectiveProcessHandler.cpp
  src/TulipPerspectiveCrashHandler.cpp
  src/FormPost.cpp
  src/PerspectiveSelectionDialog.cpp
)

QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_UI(UI_HEADERS
  designer/PerspectiveItem.ui
  designer/PluginErrorReport.ui
  designer/PluginsCenter.ui
  designer/TulipMainWindow.ui
  designer/TulipWelcomePage.ui
  designer/PluginInformationListItem.ui
  designer/TulipPerspectiveCrashHandler.ui
  designer/PerspectiveSelectionDialog.ui
)

IF(TULIP_BUILD_PYTHON_COMPONENTS)
  ADD_DEFINITIONS("-DTULIP_BUILD_PYTHON_COMPONENTS")
ENDIF(TULIP_BUILD_PYTHON_COMPONENTS)

QTX_WRAP_CPP(MOC_SRCS
  include/PerspectiveItemWidget.h
  include/PluginErrorReport.h
  include/PluginsCenter.h
  include/TulipMainWindow.h
  include/TulipWelcomePage.h
  include/PluginInformationListItem.h
  include/TulipPerspectiveProcessHandler.h
  include/TulipPerspectiveCrashHandler.h
  include/FormPost.h
  include/PerspectiveSelectionDialog.h
)

QTX_ADD_RESOURCES(RCC_SRCS resources/TulipAgent.qrc)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../crash_handling)

INCLUDE_DIRECTORIES(include ${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

IF(WIN32)

STRING(TIMESTAMP CURRENT_YEAR "%Y")

INCLUDE(generate_product_version)
generate_product_version(
   VersionFiles
   NAME "Tulip Agent"
   ICON ${PROJECT_SOURCE_DIR}/library/tulip-gui/resources/icons/tulip.ico
   VERSION_MAJOR ${TulipMajorVersion}
   VERSION_MINOR ${TulipMinorVersion}
   VERSION_PATCH ${TulipReleaseVersion}
   COMPANY_NAME "LaBRI, University of Bordeaux"
   COMPANY_COPYRIGHT "(C) Copyright ${CURRENT_YEAR} David Auber and the Tulip development Team"
   ORIGINAL_FILENAME "tulip.exe"
)

SET(SRCS ${SRCS} ${VersionFiles})

ENDIF(WIN32)

ADD_EXECUTABLE(tulip ${SRCS} ${UI_HEADERS} ${MOC_SRCS} ${RCC_SRCS})

TARGET_LINK_LIBRARIES(tulip crash_handling ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES})

INSTALL(TARGETS tulip
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)

IF(LINUX)

  SET(TULIP_LINUX_DESKTOP_REGISTRATION ON CACHE BOOL "Register Tulip application and set file associations on Linux desktops ? [ON|OFF]")

  IF(TULIP_LINUX_DESKTOP_REGISTRATION)

    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/Tulip.desktop.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Tulip-${TulipVersion}.desktop)

    INSTALL(CODE
      "EXECUTE_PROCESS(COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime/packages
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)"
    )

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-tulip-tlpx.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-tulip-tlp.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-tulip-tlpgz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-tulip-tlpz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-tulip-tlpb.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-tulip-tlpbgz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/resources/linux_desktop/application-x-tulip-tlpbz.xml
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime/packages)

    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Tulip-${TulipVersion}.desktop
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/tulip-icon.png
            RENAME tulip.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/tulip-file-icon.png
            RENAME application-x-tulip.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/tulip-icon.png
            RENAME tulip.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/tulip-file-icon.png
            RENAME application-x-tulip.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)


    INSTALL(CODE
      "EXECUTE_PROCESS(COMMAND update-desktop-database ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications
                       COMMAND update-mime-database ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime
                       OUTPUT_QUIET ERROR_QUIET)"
    )

  ENDIF(TULIP_LINUX_DESKTOP_REGISTRATION)

ENDIF(LINUX)
