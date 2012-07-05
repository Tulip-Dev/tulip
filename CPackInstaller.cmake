# This file include definitions for default Tulip packaging options
# if CPACK_CUSTOM_INSTALLER is set, none of this options will be set and user will have to configure its own installer from scratch
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 1)
SET(CPACK_PACKAGE_DESCRIPTION_FILE ${PROJECT_SOURCE_DIR}/README)
SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING.LESSER)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tulip, a software for huge graphs visualization")
SET(CPACK_PACKAGE_DESCRIPTION "Tulip is an information visualization framework dedicated to the analysis and visualization of relational data. Tulip aims to provide the developer with a complete library, supporting the design of interactive information visualization applications for relational data that can be tailored to the problems he or she is addressing.\nWritten in C++ the framework enables the development of algorithms, visual encodings, interaction techniques, data models, and domain-specific visualizations. One of the goal of Tulip is to facilitates the reuse of components and allows the developers to focus on programming their application. This development pipeline makes the framework efficient for research prototyping as well as the development of end-user applications.")
SET(CPACK_PACKAGE_VENDOR "http://www.tulip-software.org")
SET(CPACK_PACKAGE_CONTACT "Tulip Dev Team <tulipdev@labri.fr>")

SET(CPACK_PACKAGE_VERSION_MAJOR ${TulipMajorVersion})
SET(CPACK_PACKAGE_VERSION_MINOR ${TulipMinorVersion})
SET(CPACK_PACKAGE_VERSION_PATCH ${TulipReleaseVersion})

# Source package
SET(CPACK_SOURCE_GENERATOR "TGZ")
# Files excluded of the source package
SET(CPACK_SOURCE_IGNORE_FILES ".svn" "~$" "^${PROJECT_SOURCE_DIR}/unit_test/" "${CMAKE_BINARY_DIR}" "^${PROJECT_SOURCE_DIR}/docs/doxygen/xml/" "^${PROJECT_SOURCE_DIR}/profile_test/")
# name of the source package (+ extentions .tar.gz or .zip)
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "tulip-${TulipVersion}-src")
SET(CPACK_PACKAGE_NAME "tulip")

# Force grouping of components in a single package (CMake 2.9)
SET(CPACK_COMPONENTS_ALL_GROUPS_IN_ONE_PACKAGE 1)
SET(CPACK_PROJECT_CONFIG_FILE ${PROJECT_SOURCE_DIR}/CPackOptions.cmake)

IF(LINUX)
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tulip Dev Team <tulipdev@labri.fr>")
SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
ENDIF()

# Defines plateform-specific generators to be used
IF(WIN32)
  SET(CPACK_GENERATOR "NSIS")
  SET(CPACK_NSIS_DISPLAY_NAME "Tulip ${TulipVersion}")
  SET(CPACK_NSIS_HELP_LINK "http://tulip.labri.fr")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http://tulip.labri.fr")
  SET(CPACK_NSIS_CONTACT "auber@labri.fr")

  SET(TULIP_ICO "${PROJECT_SOURCE_DIR}/software/tulip/bitmaps/logo32x32.ico")
  SET(CPACK_NSIS_MUI_ICON ${TULIP_ICO})
  SET(CPACK_NSIS_MUI_UNIICON ${TULIP_ICO})
  SET(CPACK_NSIS_INSTALLED_ICON_NAME ${TULIP_ICO})
  SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\software\\\\tulip\\\\bitmaps\\\\headerlogo.bmp")
  
  SET(CPACK_NSIS_CREATE_ICONS_EXTRA "
    CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Tulip ${TulipVersion}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\tulip.exe\\\" \\\"\\\" \\\"$INSTDIR\\\\share\\\\tulip\\\\bitmaps\\\\logo32x32.ico\\\"
    CreateShortCut \\\"$DESKTOP\\\\Tulip ${TulipVersion}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\tulip.exe\\\" \\\"\\\" \\\"$INSTDIR\\\\share\\\\tulip\\\\bitmaps\\\\logo32x32.ico\\\"
  ")
  SET(CPACK_NSIS_DELETE_ICONS_EXTRA "
    Delete \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Tulip ${TulipVersion}.lnk\\\"
    Delete \\\"$DESKTOP\\\\Tulip ${TulipVersion}.lnk\\\"
  ")

  IF(MSVC)
  #install msvc redist
  SET(CMAKE_MSVC_ARCH "x86")
  #If not set try to find automatically the package
  IF(NOT MSVC_REDIST)
        FIND_PROGRAM(MSVC_REDIST NAMES
                vcredist_${CMAKE_MSVC_ARCH}/vcredist_${CMAKE_MSVC_ARCH}.exe
                PATHS $ENV{WindowsSdkDir}/Bootstrapper/Packages
                )
        ENDIF()

        IF(MSVC_REDIST)
            #If the redistribuable package is found integrate it to installer.
                INSTALL(PROGRAMS ${MSVC_REDIST} COMPONENT libtulip DESTINATION ${TulipBinInstallDir})
                SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\vcredist_x86.exe\\\" /passive'")
                message(STATUS "MSVC_REDIST: ${MSVC_REDIST}")
        ELSE()
        MESSAGE(STATUS "vcredist_${CMAKE_MSVC_ARCH} not found cannot integrate it to installer")
        ENDIF()
  ENDIF()

  SET(CPACK_COMPONENT_GROUP_LIBRARIES_BOLD_TITLE TRUE)
  SET(CPACK_COMPONENT_GROUP_LIBRARIES_EXPANDED TRUE)
  SET(CPACK_COMPONENT_GROUP_RUNTIME_EXPANDED TRUE)

  SET(CPACK_ALL_INSTALL_TYPES Full Framework)
  
  SET(CPACK_COMPONENT_FTGL_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_QSCINTILLA2_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_GZSTREAM_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_SIP_INSTALL_TYPES Full Framework)

  SET(CPACK_COMPONENT_LIBTULIP_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_OGL_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_OGL_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_QT_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_QT_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_OGDF_INSTALL_TYPES Full)
  SET(CPACK_COMPONENT_TULIP_PYTHON_INSTALL_TYPES Full Framework)

  SET(CPACK_COMPONENT_TULIP_PLUGINS_INSTALL_TYPES Full)
  
  SET(CPACK_COMPONENT_TULIP_INSTALL_TYPES Full)

  
  SET(CPACK_COMPONENT_TULIP_DOC_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_PYTHON_DOC_INSTALL_TYPES Full Framework)

ELSEIF(APPLE)
    SET(CPACK_GENERATOR "DragNDrop")
    SET(CPACK_PACKAGE_FILENAME "Tulip-${TulipMajorVersion}-${TulipMinorVersion}-${TulipReleaseVersion}")
    SET(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/software/tulip/bitmaps/logo32x32.ico")
    CONFIGURE_FILE("tulip_mac_info.plist.in" "Info.plist" @ONLY)
    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" DESTINATION ${TulipAppleContentsDir} COMPONENT tulip)
    INSTALL(FILES tulip_mac.icns DESTINATION ${TulipAppleResourcesDir} RENAME tulip.icns COMPONENT tulip)
ENDIF()
