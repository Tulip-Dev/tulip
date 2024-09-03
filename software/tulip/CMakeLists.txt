configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/main.cpp @ONLY)

SET(SRCS
  ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
  src/TulipPerspectiveMainWindow.cpp
)

QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_CPP(MOC_SRCS
  include/TulipPerspectiveMainWindow.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../crash_handling)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR} include/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

IF(WIN32)

STRING(TIMESTAMP CURRENT_YEAR "%Y")

INCLUDE(generate_product_version)
generate_product_version(
   VersionFiles
   NAME ${CMAKE_PROJECT_NAME}
   ICON ${PROJECT_SOURCE_DIR}/library/tulip-gui/resources/icons/tulip.ico
   VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
   VERSION_MINOR ${PROJECT_VERSION_MINOR}
   VERSION_PATCH ${PROJECT_VERSION_PATCH}
   COMPANY_NAME "LaBRI, University of Bordeaux"
   COMPANY_COPYRIGHT "(C) Copyright ${CURRENT_YEAR} David Auber and the Tulip development Team"
   ORIGINAL_FILENAME "tulip.exe"
)

SET(SRCS ${SRCS} ${VersionFiles})
ADD_EXECUTABLE(tulip WIN32 ${SRCS} ${MOC_SRCS})
ELSE(WIN32)
    ADD_EXECUTABLE(tulip ${SRCS} ${MOC_SRCS})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(tulip crash_handling ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES})

INSTALL(TARGETS tulip
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)

IF(WIN32)
  SET(TULIP_FIXUP_BUNDLE ON CACHE BOOL "Enable / disable the gathering and copy of Tulip DLL dependencies into the install bin folder")
  # That simple install command does the magic of gathering all required dll dependencies
  # in order to run Tulip and copy them to the bin install folder (thanks to the CMake module named BundleUtilities)
  # In the case where a dll dependency is not automatically found, you must add its path to the CMAKE_LIBRARY_PATH variable.
  IF(TULIP_FIXUP_BUNDLE)
    # copy required Qt plugins
    INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/imageformats DESTINATION ${TulipBinInstallDir})
    INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/platforms DESTINATION ${TulipBinInstallDir})
    IF(EXISTS ${QT_PLUGINS_DIR}/styles)
      INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/styles DESTINATION ${TulipBinInstallDir})
    ENDIF(EXISTS ${QT_PLUGINS_DIR}/styles)

    # then schedule a call to FIXUP_BUNDLE at the end of the CMake install process
    # this process is configured here instead in the root CMakeLists.txt as
    # CMake inserts install commands before the ones defined in sub directories
    # and we need Tulip to be fully installed before running the bundle fixing process
    SET(APP "\${CMAKE_INSTALL_PREFIX}/bin/tulip.exe")
    SET(LIB_DIRS ${QT_BINARY_DIR} ${CMAKE_LIBRARY_PATH})
    STRING(REPLACE ";" "\;" LIB_DIRS "${LIB_DIRS}")
    INSTALL(CODE
            "INCLUDE(BundleUtilities)
             FIXUP_BUNDLE(\"${APP}\" \"\" \"${LIB_DIRS}\")"
    )
    # When generating Tulip installers using MSYS2, we need to bundle OpenSSL
    # libraries in order for the Geographic view to work correctly.
    # As Qt5 does not link directly to OpenSSL for license issue and thus use
    # a dynamic loading approach, we are forced to use the following trick to copy
    # the needed dlls into the Tulip install folder.
    FIND_PROGRAM(OPENSSL openssl.exe)
    IF(MINGW AND OPENSSL)
      INSTALL(CODE
              "INCLUDE(BundleUtilities)
               FILE(COPY ${OPENSSL} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/bin/\")
               FIXUP_BUNDLE(\"${CMAKE_INSTALL_PREFIX}/bin/openssl.exe\" \"\" \"${LIB_DIRS}\")"
      )
    ENDIF(MINGW AND OPENSSL)
  ENDIF(TULIP_FIXUP_BUNDLE)
ENDIF(WIN32)

IF(LINUX)

  SET(TULIP_LINUX_DESKTOP_REGISTRATION ON CACHE BOOL "Register Tulip application and set file associations on Linux desktops ? [ON|OFF]")

  IF(TULIP_LINUX_DESKTOP_REGISTRATION)

    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/Tulip.desktop.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Tulip-${Tulip_VERSION}.desktop)

    INSTALL(CODE
      "EXECUTE_PROCESS(COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime/packages
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps
                       COMMAND mkdir -p ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)"
    )

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/application-x-tulip-tlpx.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/application-x-tulip-tlp.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/application-x-tulip-tlpgz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/application-x-tulip-tlpz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/application-x-tulip-tlpb.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/application-x-tulip-tlpbgz.xml
                  ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/application-x-tulip-tlpbz.xml
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime/packages)

    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Tulip-${Tulip_VERSION}.desktop
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/tulip-icon.png
            RENAME tulip.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/tulip-file-icon.png
            RENAME application-x-tulip.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/pixmaps)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/tulip-icon.png
            RENAME tulip.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)

    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/linux_desktop/tulip-file-icon.png
            RENAME application-x-tulip.png
            DESTINATION ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/icons)


    INSTALL(CODE
      "EXECUTE_PROCESS(COMMAND update-desktop-database ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/applications
                       COMMAND update-mime-database ${DESKTOP_CONFIG_INSTALL_PREFIX}/share/mime
                       OUTPUT_QUIET ERROR_QUIET)"
    )

  ENDIF(TULIP_LINUX_DESKTOP_REGISTRATION)

ENDIF(LINUX)
