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

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio") #visual studio does not recognize these options
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
ENDIF(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio")

## -----------------------------------------------------------------------------------------------
## Tulip version numbers
## TulipVersion : Ma.Mi.Re
## TulipMMVersion : Ma.Mi
## -----------------------------------------------------------------------------------------------
SET(TulipMajorVersion 3)
SET(TulipMinorVersion 5)
SET(TulipReleaseVersion 0)
SET(TulipVersionSeparator .)
IF(${CMAKE_GENERATOR} MATCHES "Visual Studio") #visual studio does not recognize these options
   SET(TulipVersionSeparator _)
ENDIF(${CMAKE_GENERATOR} MATCHES "Visual Studio")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERSION_SEPARATOR=${TulipVersionSeparator}")
SET(TulipVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}${TulipVersionSeparator}${TulipReleaseVersion}")
SET(TulipMMVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}")

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
## Enable process to make generation of plugins informations for server (for developpers)
## -----------------------------------------------------------------------------------------------
SET(TULIP_SERVER_GENERATION OFF CACHE BOOL "Enable process to make generation of plugins informations for server (for developpers)")

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
   IF(${CMAKE_GENERATOR} MATCHES "Visual Studio") #if generating for visual studio, we assume the compiler will be vc++
      #recommended for now, vc++ spits out too many warnings, and renders the whole output unreadable
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0 /MP")
	  #makes VS define M_PI
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
	  #prevents VS to define min and max macros (name clash with std::min and std::max)
	  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")
	  #forces ftgl static compilation (prevents link errors with tulip-ogl, functions always defined without dllimport
	  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFTGL_LIBRARY_STATIC")
	  SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /NODEFAULTLIB:MSVCRT")
   ENDIF(${CMAKE_GENERATOR} MATCHES "Visual Studio")
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
ADD_SUBDIRECTORY(textures)
ADD_SUBDIRECTORY(externalplugins)

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
## CPack packaging directives
## -----------------------------------------------------------------------------------------------

# A macro that defines a software component (see COMPONENT directive for the INSTALL command in CMake documentation)
# This defines the required CPack variables to set the component name, group, description and dependencies.
MACRO(DEFINE_COMPONENT comp display_name desc deps group)
  SET(CPACK_COMPONENTS_ALL
  ${CPACK_COMPONENTS_ALL}
  ${comp})
  STRING(TOUPPER ${comp} CN)
  SET(CPACK_COMPONENT_${CN}_DISPLAY_NAME ${display_name})
  SET(CPACK_COMPONENT_${CN}_DESCRIPTION "${desc}.")
  IF(NOT "${deps}" STREQUAL "")
    SET(CPACK_COMPONENT_${CN}_DEPENDS ${deps})
  ENDIF()
  SET(CPACK_COMPONENT_${CN}_GROUP ${group})
ENDMACRO()

# You can create your own installer by creating a setting the CPACK_CUSTOM_INSTALLER to true.
# If this variable is not set, directives in the CPackInstaller.cmake file will be used to set the project licence, maintainer etc.
# When making your own installer, please be sure to set at least all the variables present in the CPackInstaller.cmake file.
# Consult CPack documentation for additional information.
IF(NOT CPACK_CUSTOM_INSTALLER)
  INCLUDE(CPackInstaller.cmake)
  SET(LIBS_GROUP_NAME "Libraries")
  SET(HEADERS_GROUP_NAME "Headers")
  SET(SOFTWARE_GROUP_NAME "Runtime")
  SET(PLUGINS_GROUP_NAME "Plugins")
  SET(EXTRAS_GROUP_NAME "Extras")
ELSE()
  # In case a custom installer is set, all the base tulip software components will be in the same group.
  SET(LIBS_GROUP_NAME "Tulip software")
  SET(HEADERS_GROUP_NAME "Tulip software")
  SET(SOFTWARE_GROUP_NAME "Tulip software")
  SET(PLUGINS_GROUP_NAME "Tulip software")
  SET(EXTRAS_GROUP_NAME "Tulip software")
ENDIF()

# Defines specific software components from tulip.
INCLUDE(CPackComponents.cmake)

# Force grouping of components in a single package (CMake 2.9)
SET(CPACK_COMPONENTS_ALL_GROUPS_IN_ONE_PACKAGE 1)

# Defines plateform-specific generators to be used
IF(LINUX)
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
ENDIF()

INCLUDE(CPack)
