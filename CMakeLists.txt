## -----------------------------------------------------------------------------------------------
## ***********************************************************************************************
## -----------------------------------------------------------------------------------------------
##
## Options for cmake (use -D):
## CMAKE_INSTALL_PREFIX=directory_where_tulip_will_be_installed
## NO_DEPRECATE=1 disable display of deprecated warning
##
## -----------------------------------------------------------------------------------------------
## ***********************************************************************************************
## -----------------------------------------------------------------------------------------------

PROJECT(tulip)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

## -----------------------------------------------------------------------------------------------
## Tulip version numbers
## TulipVersion : Ma.Mi.Re
## TulipMMVersion : Ma.Mi
## -----------------------------------------------------------------------------------------------
SET(TulipMajorVersion 3)
SET(TulipMinorVersion 5)
SET(TulipReleaseVersion 0)
SET(TulipVersion "${TulipMajorVersion}.${TulipMinorVersion}.${TulipReleaseVersion}")
SET(TulipMMVersion "${TulipMajorVersion}.${TulipMinorVersion}")

## -----------------------------------------------------------------------------------------------
## Lib names
## -----------------------------------------------------------------------------------------------
SET(LibTulipName "tulip-${TulipMMVersion}")
SET(LibTulipOglName "tulip-ogl-${TulipMMVersion}")
SET(LibTulipQtName "tulip-qt4-${TulipMMVersion}")
SET(LibTulipPluginsManagerName "tulip-pluginsmanager-${TulipMMVersion}")

## -----------------------------------------------------------------------------------------------
## Find need packages
## -----------------------------------------------------------------------------------------------
FIND_PACKAGE(Freetype REQUIRED)
FIND_PACKAGE(Qt4  REQUIRED)
INCLUDE(${QT_USE_FILE})
IF(DEFINED CMAKE_VERSION)
  SET(HAVE_QT4_GET_MOC_FLAGS TRUE)
ELSE(DEFINED CMAKE_VERSION)
  MACRO (QT4_GET_MOC_FLAGS _moc_flags)
     SET(${_moc_flags})
     GET_DIRECTORY_PROPERTY(_inc_DIRS INCLUDE_DIRECTORIES)

     FOREACH(_current ${_inc_DIRS})
        SET(${_moc_flags} ${${_moc_flags}} "-I${_current}")
     ENDFOREACH(_current ${_inc_DIRS})

     GET_DIRECTORY_PROPERTY(_defines COMPILE_DEFINITIONS)
     FOREACH(_current ${_defines})
        SET(${_moc_flags} ${${_moc_flags}} "-D${_current}")
     ENDFOREACH(_current ${_defines})

     IF(Q_WS_WIN)
       SET(${_moc_flags} ${${_moc_flags}} -DWIN32)
     ENDIF(Q_WS_WIN)

  ENDMACRO(QT4_GET_MOC_FLAGS)
ENDIF(DEFINED CMAKE_VERSION)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)
IF(JPEG_FOUND)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_LIBJPEG")
ENDIF(JPEG_FOUND)
FIND_PACKAGE(PNG REQUIRED)
IF(PNG_FOUND)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_LIBPNG")
ENDIF(PNG_FOUND)

INCLUDE(FindGLEW.cmake)
IF(NOT GLEW_FOUND)
   message(FATAL_ERROR "Cannot find the GLEW library (required)")
ENDIF(NOT GLEW_FOUND)
INCLUDE(FindXML2.cmake)
IF(NOT LIBXML2_FOUND)
   message(FATAL_ERROR "Cannot find the XML2 library (required)")
ENDIF(NOT LIBXML2_FOUND)

## -----------------------------------------------------------------------------------------------
## OpenMP
## -----------------------------------------------------------------------------------------------

FIND_PACKAGE(OpenMP QUIET)
IF(OPENMP_FOUND)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ELSE(OPENMP_FOUND)
IF(WIN32)
   STRING(COMPARE NOTEQUAL "${OpenMP_C_FLAGS}" "" OMP_CFLAGS)
   IF(OMP_CFLAGS)
      ## HACK: do not know why OpenMP_CXX_FLAGS
      ## is not set when running on window ?
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")
      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_C_FLAGS}")
   ELSE(OMP_CFLAGS)
      MESSAGE("OpenMP not found: multithreaded code will not be used")
   ENDIF(OMP_CFLAGS)
ELSE(WIN32)
MESSAGE("OpenMP not found: multithreaded code will not be used")
ENDIF(WIN32)
ENDIF(OPENMP_FOUND)

## -----------------------------------------------------------------------------------------------
## Include file paths
## -----------------------------------------------------------------------------------------------
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
SET(TulipInclude ${PROJECT_SOURCE_DIR}/library/tulip/include/)
SET(TulipBuildInclude ${PROJECT_BINARY_DIR}/library/tulip/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipQtInclude ${PROJECT_SOURCE_DIR}/library/tulip-qt/include/)
SET(TulipQtBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-qt/include/)
SET(TulipPluginsManagerInclude ${PROJECT_SOURCE_DIR}/software/plugins-manager/include/)

## -----------------------------------------------------------------------------------------------
## Install paths
## -----------------------------------------------------------------------------------------------
SET(TulipLibInstallDir lib/)
SET(TulipBinInstallDir bin/)
SET(TulipIncludeInstallDir include/)
SET(TulipShareInstallDir share/)
SET(TulipBitmapInstallDir lib/tlp/bitmaps/)

## -----------------------------------------------------------------------------------------------
## Set UseTulip file name
## -----------------------------------------------------------------------------------------------
SET(TulipUseFile UseTulip.cmake)

## -----------------------------------------------------------------------------------------------
## Disable deprecated compilation output
## -----------------------------------------------------------------------------------------------
if (DEFINED NO_DEPRECATED)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
endif (DEFINED NO_DEPRECATED)

## -----------------------------------------------------------------------------------------------
## Disable process to make MainController plugin (for developpers)
## -----------------------------------------------------------------------------------------------
SET(TULIP_USE_MAIN_CONTROLLER ON CACHE BOOL "Link Main controller plugin")
IF(NOT TULIP_USE_MAIN_CONTROLLER)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITHOUT_MAIN_CONTROLLER")
ENDIF(NOT TULIP_USE_MAIN_CONTROLLER)

## -----------------------------------------------------------------------------------------------
## Disable process to make MainController plugin (for developpers)
## -----------------------------------------------------------------------------------------------
SET(TULIP_USE_QT_PICTURE_OUTPUT ON CACHE BOOL "Use Qt picture output")
IF(NOT TULIP_USE_QT_PICTURE_OUTPUT)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITHOUT_QT_PICTURE_OUTPUT")
ENDIF(NOT TULIP_USE_QT_PICTURE_OUTPUT)


## -----------------------------------------------------------------------------------------------
## Define NDEBUG when we are in release mode
## -----------------------------------------------------------------------------------------------
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -funroll-loops -pipe")

## -----------------------------------------------------------------------------------------------
## Check 64 bits
## -----------------------------------------------------------------------------------------------
EXEC_PROGRAM(uname ARGS "-m" OUTPUT_VARIABLE UNAME_OUT)
STRING(COMPARE EQUAL "${UNAME_OUT}" "x86_64" I64)
IF(NOT I64)
  STRING(COMPARE EQUAL "${UNAME_OUT}" "ia64" I64)
ENDIF(NOT I64)
IF(NOT I64)
  STRING(COMPARE EQUAL "${UNAME_OUT}" "Power Macintosh" MACPPC)
ENDIF(NOT I64)
IF(I64)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DI64")
ENDIF(I64)
IF(MACPPC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMACPPC")
ENDIF(MACPPC)

## -----------------------------------------------------------------------------------------------
## Windows, Apple and Linux
## -----------------------------------------------------------------------------------------------
EXEC_PROGRAM(uname OUTPUT_VARIABLE UNAME_OUT)
STRING(COMPARE EQUAL "${UNAME_OUT}" "Linux" LINUX)
IF(NOT LINUX)
  STRING(COMPARE EQUAL "${UNAME_OUT}" "GNU/kFreeBSD" LINUX)
ENDIF(NOT LINUX)
IF(LINUX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
ENDIF(LINUX)

IF(WIN32)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32 -DDLL_EXPORT -DQT_ASSISTANT='\"assistant_adp\"'")
   SET(QT_ASSISTANT assistant_adp)
ENDIF(WIN32)
IF(APPLE)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__APPLE__ -DQT_ASSISTANT='\"Assistant_adp\"'")
ENDIF(APPLE)
IF(LINUX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_ASSISTANT='\"assistant_adp\"'")
ENDIF(LINUX)

## -----------------------------------------------------------------------------------------------
## Create library/tulip/include/tulip/TulipRelease.h base on TulipRelease.h.in
## -----------------------------------------------------------------------------------------------
SET(VERSION "${TulipVersion}")
CONFIGURE_FILE("library/tulip/include/tulip/TulipRelease.h.in" "library/tulip/include/tulip/TulipRelease.h")

## -----------------------------------------------------------------------------------------------
## Sub dir (no comment)
## -----------------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(software)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(externalplugins)
ADD_SUBDIRECTORY(textures)
FIND_PACKAGE(Doxygen QUIET)
if (DOXYGEN_FOUND)
  OPTION(GENERATE_DOC "Do you want to generate the documentation ? [OFF|ON]")
  if (GENERATE_DOC)
    ADD_SUBDIRECTORY(docs)
    SET(TULIP_LIB_DESC "Tulip Graph Library")
    SET(TULIP_LIB_NAME "tulip")
    CONFIGURE_FILE("docs/doxygen/tulip-lib.doxygen.in" "docs/doxygen/tulip-lib.doxygen")
    SET(TULIP_LIB_DESC "Tulip Open GL Library")
    SET(TULIP_LIB_NAME "tulip-ogl")
    CONFIGURE_FILE("docs/doxygen/tulip-lib.doxygen.in" "docs/doxygen/tulip-ogl-lib.doxygen")
    SET(TULIP_LIB_DESC "Tulip Qt Library")
    SET(TULIP_LIB_NAME "tulip-qt")
    CONFIGURE_FILE("docs/doxygen/tulip-lib.doxygen.in" "docs/doxygen/tulip-qt-lib.doxygen")
    EXEC_PROGRAM(date ARGS "+%Y" OUTPUT_VARIABLE THIS_YEAR)
    CONFIGURE_FILE("docs/doxygen/tulip.html.in" "docs/doxygen/tulip.html")
    CONFIGURE_FILE("docs/doxygen/header.html.in" "docs/doxygen/header.html")
    CONFIGURE_FILE("docs/doxygen/footer.html.in" "docs/doxygen/footer.html")
    CONFIGURE_FILE("docs/doxygen/header-allPlugins.html.in" "docs/doxygen/header-allPlugins.html")
    CONFIGURE_FILE("docs/doxygen/tulip-plugins.doxygen.in" "docs/doxygen/tulip-plugins.doxygen")    
  endif(GENERATE_DOC)
else(DOXYGEN_FOUND)
MESSAGE("Doxygen not found: unable to generate documentation")
endif(DOXYGEN_FOUND)

## -----------------------------------------------------------------------------------------------
## Set var to create tulip-config script
## -----------------------------------------------------------------------------------------------
SET(prefix "${CMAKE_INSTALL_PREFIX}")
SET(exec_prefix "\${prefix}")
SET(includedir "\${prefix}${TulipIncludeInstallDir}")
SET(libdir "\${prefix}${TulipLibInstallDir}")
SET(libversion "${TulipMMVersion}")
SET(VERSION "${TulipVersion}")
SET(WIN_VERSION "${TulipMajorVersion}.${TulipMinorVersion}")
SET(QT_CPPFLAGS "-DQT_MINOR_REL=${QT_VERSION_MINOR}")

IF("${OPENGL_INCLUDE_DIR}" STRGREATER "")
SET(GL_INCLUDES "-I${OPENGL_INCLUDE_DIR}")
ENDIF("${OPENGL_INCLUDE_DIR}" STRGREATER "")

IF(WIN32)
SET(GL_LDFLAGS "-l${OPENGL_gl_LIBRARY} -l${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY}")
ELSE(WIN32)
SET(GL_LDFLAGS "${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY}")
ENDIF(WIN32)

SET(QT_LDFLAGS "${QT_QTCORE_LIBRARY_RELEASE} ${QT_QTGUI_LIBRARY_RELEASE} ${QT_QTOPENGL_LIBRARY_RELEASE} ${QT_QTXML_LIBRARY_RELEASE} ${QT_QTNETWORK_LIBRARY_RELEASE} ${QT_QTWEBKIT_LIBRARY_RELEASE}")
SET(QT_INCLUDES "-I${QT_INCLUDE_DIR}")

CONFIGURE_FILE("tulip-config.in" "tulip-config" @ONLY)

## -----------------------------------------------------------------------------------------------
## create pluginstest.cpp
## -----------------------------------------------------------------------------------------------
CONFIGURE_FILE("unit_test/plugins/pluginstest.cpp.in" "unit_test/plugins/pluginstest.cpp" @ONLY)

## -----------------------------------------------------------------------------------------------
## Installation of files
## -----------------------------------------------------------------------------------------------
INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config
        DESTINATION ${TulipBinInstallDir})
INSTALL(FILES FindTULIP3.cmake 
        DESTINATION ${TulipShareInstallDir})

## -----------------------------------------------------------------------------------------------
## Package generation for the "make package" target
## -----------------------------------------------------------------------------------------------
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
SET(CPACK_PACKAGE_DESCRIPTION_FILE ${PROJECT_SOURCE_DIR}/README)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tulip: A software for huge graphs 3D visualization.")
SET(CPACK_PACKAGE_DESCRIPTION "Tulip is an information visualization framework dedicated to the analysis and visualization of relational data. Tulip aims to provide the developer with a complete library, supporting the design of interactive information visualization applications for relational data that can be tailored to the problems he or she is addressing.\nWritten in C++ the framework enables the development of algorithms, visual encodings, interaction techniques, data models, and domain-specific visualizations. One of the goal of Tulip is to facilitates the reuse of components and allows the developers to focus on programming their application. This development pipeline makes the framework efficient for research prototyping as well as the development of end-user applications.")
SET(CPACK_PACKAGE_VENDOR "David Auber, LaBRI")
SET(CPACK_PACKAGE_VERSION_MAJOR ${TulipMajorVersion})
SET(CPACK_PACKAGE_VERSION_MINOR ${TulipMinorVersion})
SET(CPACK_PACKAGE_VERSION_PATCH ${TulipReleaseVersion})
SET(CPACK_PROJECT_CONFIG_FILE ${PROJECT_SOURCE_DIR}/CPackOptions.cmake)
SET(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
SET(CPACK_SOURCE_IGNORE_FILES ".svn")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "tulip-${TulipVersion}-src")
SET(CPACK_PACKAGE_NAME "tulip")
SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING.LESSER)

SET(CPACK_COMPONENTS_ALL
tulip_doc # Doxygen documentations, handbooks
libtulip # libtulip.so
libtulip_dev # tulip headers
libtulip_ogl # libtulip-ogl.so
libtulip_ogl_dev # tulip-ogl headers
libtulip_qt # libtulip-qt.so
libtulip_qt_dev # libtulip-qt headers
tulip_plugins_clustering # Clustering base plugins
tulip_plugins_colors # Colors base plugins
tulip_plugins_export # Export base plugins
tulip_plugins_glyphs # Glyphs base plugins
tulip_plugins_interactors # Glyphs base plugins
tulip_plugins_import # Import base plugins
tulip_plugins_layout # Layout base plugins
tulip_plugins_metric # Metric base plugins
tulip_plugins_selection # Selection base plugins
tulip_plugins_sizes # Sizes base plugins
tulip_plugins_spreadsheetview # Spreadsheet view plugin
tulip_pluginsmanager # Plugins manager software
#tlprender # TLP renderer (not included in normal install)
tulip_app # Tulip software
tulip_textures # Additional textures
tulip # Meta package containing tulip_app, libraries and plugins
)

# Defines software component attributes (display name, description, dependencies, group)
MACRO(DEF_COMP comp display_name desc deps group)
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
DEF_COMP(libtulip "Tulip framework" "Tulip core library provides a framework for huge graph manipulation." "" ${LIBS_GROUP_NAME})
DEF_COMP(libtulip_dev "Tulip framework - Development files." "Tulip core library provides a framework for huge graph manipulation." "libtulip" ${HEADERS_GROUP_NAME})

# library/tulip-ogl
DEF_COMP(libtulip_ogl "Tulip framework - OpenGL bindings" "Tulip-ogl provides opengl bindings for 3D visualization of graphs created with the Tulip core library. " "libtulip" ${LIBS_GROUP_NAME})
DEF_COMP(libtulip_ogl_dev "Tulip framework - OpenGL bindings - Development files" "Tulip-ogl provides opengl bindings for 3D visualization of graphs created with the Tulip core library." "libtulip_ogl" ${HEADERS_GROUP_NAME})

#library/tulip-qt
DEF_COMP(libtulip_qt "Tulip framework - Qt bindings" "Tulip-qt provides binding for the Qt4 framework. Helping user to design user interfaces to integrate Tulip 3D visualizations into a GUI." "libtulip;libtulip_ogl" ${LIBS_GROUP_NAME})
DEF_COMP(libtulip_qt_dev "Tulip framework - Qt bindings - Development files" "Tulip-qt provides binding for the Qt4 framework. Helping user to design user interfaces to integrate Tulip 3D visualizations into a GUI." "libtulip_qt" ${HEADERS_GROUP_NAME})

# plugins/*
DEF_COMP(tulip_plugins_clustering "Tulip - Clustering plugins" "Graph clustering plugins for the Tulip framework" "libtulip" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_colors "Tulip - Colors plugins" "Colors plugins for the Tulip framework" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_export "Tulip - Export plugins" "Export plugins for the Tulip framework" "libtulip" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_glyphs "Tulip - Glyphs plugins" "Glyphs plugins for the Tulip framework" "libtulip;libtulip_ogl" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_interactors "Tulip - Interactors plugins" "Glyphs plugins for the Tulip framework" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_import "Tulip - Import plugins" "Import plugins for the Tulip framework" "libtulip" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_layout "Tulip - Layout plugins" "Graph layout plugins for the Tulip framework" "libtulip" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_metric "Tulip - Metric plugins" "Metric plugins for the Tulip framework" "libtulip" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_selection "Tulip - Selection plugins" "Filter plugins for the Tulip framework" "libtulip" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_sizes "Tulip - Sizes plugins" "Sizes plugins for the Tulip framework" "libtulip;libtulip_ogl" ${PLUGINS_GROUP_NAME})
DEF_COMP(tulip_plugins_spreadsheetview "Tulip - Spreadsheet view" "Spreadsheet view plugin for the Tulip framework" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})

# software/pluginsmanager
DEF_COMP(tulip_pluginsmanager "Tulip - Plugins manager" "A software to manage plugins installation from external sources." "libtulip;libtulip_ogl;libtulip_qt" ${SOFTWARE_GROUP_NAME})
DEF_COMP(tulip_app "Tulip software" "The main Tulip software. Provides a complete interface and a set of tools to easily create, manage and visualize huge graphs in 3D scenes." "libtulip;libtulip_ogl;libtulip_qt;tulip_pluginsmanager" ${SOFTWARE_GROUP_NAME})
DEF_COMP(tulip_textures "Tulip - Additional textures" "Additional textures for the Tulip software" "" ${EXTRAS_GROUP_NAME})

# doc/*
DEF_COMP(tulip_doc "Tulip - Documentation" "Manuals and doxygen for the Tulip framework." "" ${EXTRAS_GROUP_NAME})

# meta package
DEF_COMP(tulip "Tulip - Meta package" "Meta package containing tulip application, libraries, documentation and base plugins" "libtulip;libtulip_ogl;libtulip_qt;tulip_app;tulip_pluginsmanager;tulip_doc;tulip_textures;tulip_plugins_clustering;tulip_plugins_colors;tulip_plugins_export;tulip_plugins_glyphs;tulip_plugins_interactors;tulip_plugins_import;tulip_plugins_layout;tulip_plugins_metric;tulip_plugins_selection;tulip_plugins_sizes;tulip_plugins_spreadsheetview" ${EXTRAS_GROUP_NAME})

# Force grouping of components in a single package (CMake 2.9)
SET(CPACK_COMPONENTS_ALL_GROUPS_IN_ONE_PACKAGE 1)

IF(LINUX)
  IF(I64)
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
  ELSE()
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
  ENDIF()

  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "auber@labri.fr")
  SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
  SET(CPACK_GENERATOR "STGZ;TGZ;")
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
ENDIF(LINUX)

INCLUDE(CPack)
