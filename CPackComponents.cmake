## -----------------------------------------------------------------------------------------------
## Package generation for the "make package" target
## This file define basic Tulip components from Tulip libraries and applications.
## It provides rules to generate packages for Windows (NSIS), Linux (DEB,RPM,Archive) and Mac OSX (Installer)
## See CPackOptions.cmake for additional generator-specific rules
## -----------------------------------------------------------------------------------------------
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
SET(CPACK_PACKAGE_DESCRIPTION_FILE ${PROJECT_SOURCE_DIR}/README)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tulip: A software for huge graphs 3D visualization.")
SET(CPACK_PACKAGE_DESCRIPTION "Tulip is an information visualization framework dedicated to the analysis and visualization of relational data. Tulip aims to provide the developer with a complete library, supporting the design of interactive information visualization applications for relational data that can be tailored to the problems he or she is addressing.\nWritten in C++ the framework enables the development of algorithms, visual encodings, interaction techniques, data models, and domain-specific visualizations. One of the goal of Tulip is to facilitates the reuse of components and allows the developers to focus on programming their application. This development pipeline makes the framework efficient for research prototyping as well as the development of end-user applications.")
SET(CPACK_PACKAGE_VENDOR "Tulip Dev Team, LaBRI <tulipdev@labri.fr>")
SET(CPACK_PACKAGE_VERSION_MAJOR ${TulipMajorVersion})
SET(CPACK_PACKAGE_VERSION_MINOR ${TulipMinorVersion})
SET(CPACK_PACKAGE_VERSION_PATCH ${TulipReleaseVersion})
SET(CPACK_PROJECT_CONFIG_FILE ${PROJECT_SOURCE_DIR}/CPackOptions.cmake)
SET(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
SET(CPACK_SOURCE_IGNORE_FILES ".svn")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "tulip-${TulipVersion}-src")
SET(CPACK_PACKAGE_NAME "tulip")
SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING.LESSER)

# Defines software component attributes (display name, description, dependencies, group)
MACRO(DEFINE_COMPONENT comp display_name desc deps group)
  SET(CPACK_COMPONENTS_ALL
  ${CPACK_COMPONENTS_ALL}
  ${comp})
  STRING(TOUPPER ${comp} CN)
  SET(CPACK_COMPONENT_${CN}_DISPLAY_NAME ${display_name})
  SET(CPACK_COMPONENT_${CN}_DESCRIPTION "${desc}.\n Tulip is a free software, you can visit http://tulip.labri.fr to get more informations, news and extra content for Tulip.")
  IF(NOT "${deps}" STREQUAL "")
    SET(CPACK_COMPONENT_${CN}_DEPENDS ${deps})
  ENDIF()
  SET(CPACK_COMPONENT_${CN}_GROUP ${group})
ENDMACRO()

SET(LIBS_GROUP_NAME "Libraries")
SET(HEADERS_GROUP_NAME "Headers")
SET(SOFTWARE_GROUP_NAME "Runtime")
SET(PLUGINS_GROUP_NAME "Plugins")
SET(EXTRAS_GROUP_NAME "Extras")

SET(CPACK_COMPONENT_GROUP_LIBRARIES_DESCRIPTION "Tulip framework dynamic libraries.")
SET(CPACK_COMPONENT_GROUP_HEADERS_DESCRIPTION "Tulip framework C++ headers.")
SET(CPACK_COMPONENT_GROUP_SOFTWARE_DESCRIPTION "Tulip softwares.")
SET(CPACK_COMPONENT_GROUP_PLUGINS_DESCRIPTION "Tulip base plugins set.")
SET(CPACK_COMPONENT_GROUP_EXTRAS_DESCRIPTION "Tulip extra files and documentation.")

# Note: dependencies for components have ben made by looking into the CMakeList to see libraries used at link stage.
# If a component is modified, one should look if additional libraries have been put into the corresponding CMakeList and make changes in the following list if needed

# library/tulip
DEFINE_COMPONENT(libtulip "Tulip framework" "Tulip core library provides a framework for huge graph manipulation." "" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_dev "Tulip framework - Development files." "Tulip core library provides a framework for huge graph manipulation." "libtulip" ${HEADERS_GROUP_NAME})

# library/tulip-ogl
DEFINE_COMPONENT(libtulip_ogl "Tulip framework - OpenGL bindings" "Tulip-ogl provides opengl bindings for 3D visualization of graphs created with the Tulip core library. " "libtulip" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_ogl_dev "Tulip framework - OpenGL bindings - Development files" "Tulip-ogl provides opengl bindings for 3D visualization of graphs created with the Tulip core library." "libtulip_ogl" ${HEADERS_GROUP_NAME})

#library/tulip-qt
DEFINE_COMPONENT(libtulip_qt "Tulip framework - Qt bindings" "Tulip-qt provides binding for the Qt4 framework. Helping user to design user interfaces to integrate Tulip 3D visualizations into a GUI." "libtulip;libtulip_ogl" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_qt_dev "Tulip framework - Qt bindings - Development files" "Tulip-qt provides binding for the Qt4 framework. Helping user to design user interfaces to integrate Tulip 3D visualizations into a GUI." "libtulip_qt" ${HEADERS_GROUP_NAME})

# plugins/*
DEFINE_COMPONENT(libtulip_plugins "Tulip - libtulip plugins" "Base plugins for the tulip library" "libtulip" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_ogl_plugins "Tulip - libtulip-ogl plugins" "Base plugins for the Tulip OGL library" "libtulip;libtulip_ogl" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_qt_plugins "Tulip - libtulip-qt plugins" "Base plugins for the Tulip Qt library" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(tulip_plugins_spreadsheetview "Tulip - Spreadsheet view" "Spreadsheet view plugin for the Tulip framework" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})

# software/pluginsmanager
DEFINE_COMPONENT(tulip_pluginsmanager "Tulip - Plugins manager" "A software to manage plugins installation from external sources." "libtulip;libtulip_ogl;libtulip_qt" ${SOFTWARE_GROUP_NAME})
DEFINE_COMPONENT(tulip_app "Tulip software" "The main Tulip software. Provides a complete interface and a set of tools to easily create, manage and visualize huge graphs in 3D scenes." "libtulip;libtulip_ogl;libtulip_qt;tulip_pluginsmanager" ${SOFTWARE_GROUP_NAME})
DEFINE_COMPONENT(tulip_textures "Tulip - Additional textures" "Additional textures for the Tulip software" "" ${EXTRAS_GROUP_NAME})

# doc/*
DEFINE_COMPONENT(tulip_doc "Tulip - Documentation" "Manuals and doxygen for the Tulip framework." "" ${EXTRAS_GROUP_NAME})

# Force grouping of components in a single package (CMake 2.9)
SET(CPACK_COMPONENTS_ALL_GROUPS_IN_ONE_PACKAGE 1)

IF(LINUX)
  # meta package (Linux only)
  DEFINE_COMPONENT(tulip "Tulip - Meta package" "Meta package containing tulip application, libraries, documentation and base plugins" "libtulip;libtulip_ogl;libtulip_qt;tulip_app;tulip_pluginsmanager;tulip_doc;tulip_textures;libtulip_plugins;libtulip_qt_plugins;libtulip_ogl_plugins;tulip_plugins_spreadsheetview" ${EXTRAS_GROUP_NAME})

  IF(I64)
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
  ELSE()
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
  ENDIF()

  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tulip Dev Team <tulipdev@labri.fr>")
  SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
  SET(CPACK_GENERATOR ";")
  # Note: With CMake 2.8, we must introduce a hack to force generation of one deb per component. See CPackOptions.cmake for additional informations.
  FOREACH(ITC ${CPACK_COMPONENTS_ALL})
    SET(CPACK_GENERATOR "${CPACK_GENERATOR}DEB;")
  ENDFOREACH()
ELSEIF(WIN32)
    SET(CPACK_GENERATOR "NSIS")
    SET(TULIP_ICO "${PROJECT_SOURCE_DIR}/software/tulip/bitmaps/logo32x32.ico")
    SET(CPACK_NSIS_MUI_ICON ${TULIP_ICO})
    SET(CPACK_NSIS_MUI_UNIICON ${TULIP_ICO})
    SET(CPACK_NSIS_DISPLAY_NAME "Tulip ${TulipVersion}")
    SET(CPACK_NSIS_INSTALLED_ICON_NAME ${TULIP_ICO})
    SET(CPACK_NSIS_HELP_LINK "http://tulip.labri.fr")
    SET(CPACK_NSIS_URL_INFO_ABOUT "http://tulip.labri.fr")
    SET(CPACK_NSIS_CONTACT "auber@labri.fr")
#CPACK_NSIS_EXTRA_INSTALL_COMMANDS 	Extra NSIS commands that will be added to the install Section. 	ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /q:a'
#CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS 	Extra NSIS commands that will be added to the uninstall Section.
ENDIF()
