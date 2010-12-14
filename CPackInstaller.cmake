# This file include definitions for default Tulip packaging options
# if CPACK_CUSTOM_INSTALLER is set, none of this options will be set and user will have to configure its own installer from scratch
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
SET(CPACK_PACKAGE_DESCRIPTION_FILE ${PROJECT_SOURCE_DIR}/README)
SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING.LESSER)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tulip: A software for huge graphs 3D visualization.")
SET(CPACK_PACKAGE_DESCRIPTION "Tulip is an information visualization framework dedicated to the analysis and visualization of relational data. Tulip aims to provide the developer with a complete library, supporting the design of interactive information visualization applications for relational data that can be tailored to the problems he or she is addressing.\nWritten in C++ the framework enables the development of algorithms, visual encodings, interaction techniques, data models, and domain-specific visualizations. One of the goal of Tulip is to facilitates the reuse of components and allows the developers to focus on programming their application. This development pipeline makes the framework efficient for research prototyping as well as the development of end-user applications.")
SET(CPACK_PACKAGE_VENDOR "Tulip Dev Team, LaBRI <tulipdev@labri.fr>")

SET(CPACK_PACKAGE_VERSION_MAJOR ${TulipMajorVersion})
SET(CPACK_PACKAGE_VERSION_MINOR ${TulipMinorVersion})
SET(CPACK_PACKAGE_VERSION_PATCH ${TulipReleaseVersion})

# Source package
SET(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
SET(CPACK_SOURCE_IGNORE_FILES ".svn")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "tulip-${TulipVersion}-src")
SET(CPACK_PACKAGE_NAME "tulip")

# Force grouping of components in a single package (CMake 2.9)
SET(CPACK_COMPONENTS_ALL_GROUPS_IN_ONE_PACKAGE 1)
SET(CPACK_PROJECT_CONFIG_FILE ${PROJECT_SOURCE_DIR}/CPackOptions.cmake)

# Defines plateform-specific generators to be used
IF(LINUX)
  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tulip Dev Team <tulipdev@labri.fr>")
  SET(CPACK_DEBIAN_PACKAGE_SECTION "science")

  IF(I64)
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
  ELSE()
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
  ENDIF()

  SET(CPACK_GENERATOR ";")
  # Note: With CMake 2.8, we must introduce a hack to force generation of one deb per component. See CPackOptions.cmake for additional informations.
  FOREACH(ITC ${CPACK_COMPONENTS_ALL})
    SET(CPACK_GENERATOR "${CPACK_GENERATOR}DEB;")
  ENDFOREACH()
ELSEIF(WIN32)
  SET(CPACK_GENERATOR "NSIS")
  CMAKE_POLICY(SET CMP0010 OLD) # Disable some warnings
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
    CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Tulip ${TulipVersion}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\tulip.exe\\\" \\\"\\\" \\\"$INSTDIR\\\\lib\\\\tlp\\\\bitmaps\\\\logo32x32.ico\\\"
    CreateShortCut \\\"$DESKTOP\\\\Tulip ${TulipVersion}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\tulip.exe\\\" \\\"\\\" \\\"$INSTDIR\\\\lib\\\\tlp\\\\bitmaps\\\\logo32x32.ico\\\"
  ")

  SET(CPACK_COMPONENT_GROUP_LIBRARIES_BOLD_TITLE TRUE)
  SET(CPACK_COMPONENT_GROUP_LIBRARIES_EXPANDED TRUE)
  SET(CPACK_COMPONENT_GROUP_RUNTIME_EXPANDED TRUE)

  SET(CPACK_ALL_INSTALL_TYPES Full Framework)

  SET(CPACK_COMPONENT_LIBTULIP_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_OGL_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_OGL_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_QT_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBTULIP_QT_DEV_INSTALL_TYPES Full Framework)

  SET(CPACK_COMPONENT_LIBTULIP_PLUGINS_INSTALL_TYPES Full)
  SET(CPACK_COMPONENT_LIBTULIP_OGL_PLUGINS_INSTALL_TYPES Full)
  SET(CPACK_COMPONENT_LIBTULIP_QT_PLUGINS_INSTALL_TYPES Full)
  SET(CPACK_COMPONENT_TULIP_PLUGINS_SPREADSHEETVIEW_INSTALL_TYPES Full)

  SET(CPACK_COMPONENT_TULIP_PLUGINSMANAGER_INSTALL_TYPES Full)
  SET(CPACK_COMPONENT_TULIP_APP_INSTALL_TYPES Full)

  SET(CPACK_COMPONENT_TULIP_TEXTURES_INSTALL_TYPES Full)

  SET(CPACK_COMPONENT_TULIP_DOC_INSTALL_TYPES Full Framework)
ENDIF()
