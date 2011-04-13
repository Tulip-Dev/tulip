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

IF(${CMAKE_GENERATOR} MATCHES "Visual Studio" OR ${CMAKE_GENERATOR} MATCHES "NMake Makefiles" OR ${CMAKE_GENERATOR} MATCHES "NMake Makefiles JOM" )
	SET(MSVC 1)
ELSE()
	SET(MSVC 0)
endif()

IF(NOT ${MSVC}) #visual studio does not recognize these options
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wunused")
ENDIF()

IF(WIN32)
	IF(NOT ${MSVC}) #visual studio does not recognize these options 
		
		# Dynamic ling against libstdc++ on win32/MinGW 
		# The second test is for the case where ccache is used (CMAKE_CXX_COMPILER_ARG1 contains the path to the g++ compiler)
		IF (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ARG1}" MATCHES ".*[g][+][+].*")
			EXECUTE_PROCESS(COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
							OUTPUT_VARIABLE GCXX_VERSION)
							
			IF(GCXX_VERSION VERSION_GREATER 4.0)
		
				#SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,-subsystem,console")
				#SET(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,-subsystem,console")
				#SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,-subsystem,console")

				SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DLL")
				SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shared-libgcc -Wl,--allow-multiple-definition")
				SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --shared-libgcc  -Wl,--allow-multiple-definition")
				SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --shared-libgcc  -Wl,--allow-multiple-definition")
	  
			ENDIF()
			IF(GCXX_VERSION VERSION_GREATER 4.4 OR GCXX_VERSION VERSION_EQUAL 4.4)
				IF(GCXX_VERSION VERSION_LESS 4.5)
					SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lstdc++_s")
				ENDIF()
			ENDIF()	
			IF(GCXX_VERSION VERSION_GREATER 4.5 OR GCXX_VERSION VERSION_EQUAL 4.5)
				SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lstdc++")
			ENDIF()
	ENDIF()
 ENDIF()
ENDIF()

## -----------------------------------------------------------------------------------------------
## Tulip version numbers
## TulipVersion : Ma.Mi.Re
## TulipMMVersion : Ma.Mi
## -----------------------------------------------------------------------------------------------
SET(TulipMajorVersion 3)
SET(TulipMinorVersion 6)
SET(TulipReleaseVersion 0)
SET(TulipVersionSeparator .)
IF(${MSVC}) #specific options for Visual Studio
	IF(${CMAKE_GENERATOR} MATCHES "Visual Studio 9") #visual studio 2008 needs boost
		find_package(BOOST REQUIRED)
		include_directories(${Boost_INCLUDE_DIRS})
		include_directories(${Boost_INCLUDE_DIRS}/boost/tr1)
	ENDIF()
   SET(TulipVersionSeparator _)
ENDIF()
SET(TulipVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}${TulipVersionSeparator}${TulipReleaseVersion}")
SET(TulipMMVersion "${TulipMajorVersion}${TulipVersionSeparator}${TulipMinorVersion}")

## -----------------------------------------------------------------------------------------------
## Lib names
## -----------------------------------------------------------------------------------------------
SET(LibTulipName "tulip-${TulipMMVersion}")
SET(LibTulipOglName "tulip-ogl-${TulipMMVersion}")
SET(LibTulipQtName "tulip-qt4-${TulipMMVersion}")
SET(LibTulipOGDFName "tulip-ogdf-${TulipMMVersion}")
SET(LibTulipPluginsManagerName "tulip-pluginsmanager-${TulipMMVersion}")
SET(LibStlPythonName "stlpython")
SET(LibTulipPythonName "tulippython")
SET(LibTulipOglPythonName "tulipoglpython")

## -----------------------------------------------------------------------------------------------
## Find need packages
## -----------------------------------------------------------------------------------------------
FIND_PACKAGE(Freetype REQUIRED)
FIND_PACKAGE(Qt4 4.7.0 REQUIRED)
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
   IF(WIN32)
	  IF(${MSVC})
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
	  ELSE()
		SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp -lpthread")
	  ENDIF()
   ENDIF()
ELSE(OPENMP_FOUND)
  IF(WIN32)
   STRING(COMPARE NOTEQUAL "${OpenMP_C_FLAGS}" "" OMP_CFLAGS)
   IF(OMP_CFLAGS)
      ## HACK: do not know why OpenMP_CXX_FLAGS
      ## is not set when running on window ?
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")
      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_C_FLAGS}")
      SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp -lpthread")
   ELSE(OMP_CFLAGS)
      MESSAGE("OpenMP not found: multithreaded code will not be used")
   ENDIF(OMP_CFLAGS)
  ELSE(WIN32)
    MESSAGE("OpenMP not found: multithreaded code will not be used")
  ENDIF(WIN32)
ENDIF(OPENMP_FOUND)

## -----------------------------------------------------------------------------------------------
## Python (not required)
## -----------------------------------------------------------------------------------------------

# Try to find Python in first place

# If Python is present, compile SIP before launching the whole Tulip build  
# because we need it later in the configure process 

FIND_PACKAGE(PythonLibs)
IF(PYTHONLIBS_FOUND)
  SET(SIP_LIB siplib)
  TRY_COMPILE(SIP_OK ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.11.2/sipgen ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-4.11.2/sipgen sip CMAKE_FLAGS -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES})
  IF(SIP_OK)
    IF(WIN32 AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.11.2/sipgen/Debug/sipgen.exe")
      SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.11.2/sipgen/Debug/sipgen.exe)
    ELSE()
      SET(SIP_EXE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/sip-4.11.2/sipgen/sipgen)
    ENDIF()
    SET(SIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sip-4.11.2/siplib/)
  ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)


## -----------------------------------------------------------------------------------------------
## Include file paths
## -----------------------------------------------------------------------------------------------
SET(GZStreamInclude ${PROJECT_SOURCE_DIR}/thirdparty/gzstream/)
SET(FTGLInclude ${PROJECT_SOURCE_DIR}/thirdparty/ftgl/)
SET(qscintilla2Include ${PROJECT_SOURCE_DIR}/thirdparty/QScintilla-gpl-2.4.6/Qt4/)
SET(OGDFInclude ${PROJECT_SOURCE_DIR}/thirdparty/OGDF)
SET(OGDFLibrary OGDF)
SET(TulipInclude ${PROJECT_SOURCE_DIR}/library/tulip/include/)
SET(TulipBuildInclude ${PROJECT_BINARY_DIR}/library/tulip/include/)
SET(TulipOGLInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogl/include/)
SET(TulipOGDFInclude ${PROJECT_SOURCE_DIR}/library/tulip-ogdf/)
SET(TulipQtInclude ${PROJECT_SOURCE_DIR}/library/tulip-qt/include/)
SET(TulipQtBuildInclude ${PROJECT_BINARY_DIR}/library/tulip-qt/include/)
SET(TulipPluginsManagerInclude ${PROJECT_SOURCE_DIR}/software/plugins-manager/include/)

## -----------------------------------------------------------------------------------------------
## Install paths
## -----------------------------------------------------------------------------------------------
IF(APPLE)
SET(TulipAppleContentsDir Tulip-${TulipVersion}.app/Contents/)
SET(TulipBinInstallDir ${TulipAppleContentsDir}MacOS/)
SET(TulipLibInstallDir ${TulipAppleContentsDir}Frameworks/)
SET(TulipIncludeInstallDir ${TulipAppleContentsDir}include/)
SET(TulipShareInstallDir ${TulipAppleContentsDir}share/)
SET(TulipPluginsInstallDir ${TulipAppleContentsDir}lib/tlp/)
SET(TulipBitmapInstallDir ${TulipAppleContentsDir}lib/tlp/bitmaps/)
SET(TulipAppleResourcesDir ${TulipAppleContentsDir}Resources/)
ELSE()
SET(TulipLibInstallDir lib/)
SET(TulipPluginsInstallDir ${TulipLibInstallDir}/tlp/)
SET(TulipBinInstallDir bin/)
SET(TulipIncludeInstallDir include/)
SET(TulipShareInstallDir share/)
SET(TulipBitmapInstallDir lib/tlp/bitmaps/)
ENDIF()

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
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

## -----------------------------------------------------------------------------------------------
## Enable output of rendering time
## Warning : You must have OpenMP
## -----------------------------------------------------------------------------------------------
SET(ENABLE_RENDERING_TIME_DISPLAY OFF CACHE BOOL "Do you want to disable the rendering time display ? (Warning : You must have OpenMP) [OFF|ON]")

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
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32 -DQT_ASSISTANT='\"assistant\"'")
   IF(${MSVC})
	  #tells VS to use multiple threads to compile
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	  #makes VS define M_PI
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
	  #prevents VS to define min and max macros (name clash with std::min and std::max)
	  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")
	  
	  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
	  #SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /NODEFAULTLIB:MSVCRT")
	  #SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /MACHINE:X86")
	  
	  #allows for link time optimization
	  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
	  #link time optimization
	  #SET(CMAKE_SHARED_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /LTCG")
   ENDIF()
ENDIF(WIN32)
IF(APPLE)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__APPLE__ -DQT_ASSISTANT='\"Assistant\"'")
ENDIF(APPLE)
IF(LINUX)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_ASSISTANT='\"assistant\"'")
ENDIF(LINUX)

## -----------------------------------------------------------------------------------------------
## Create library/tulip/include/tulip/TulipRelease.h base on TulipRelease.h.in
## -----------------------------------------------------------------------------------------------
SET(VERSION "${TulipVersion}")
SET(MM_VERSION "${TulipMMVersion}")
CONFIGURE_FILE("library/tulip/include/tulip/TulipRelease.h.in" "library/tulip/include/tulip/TulipRelease.h")

## -----------------------------------------------------------------------------------------------
## Include macro definitions
## -----------------------------------------------------------------------------------------------
INCLUDE(CMakeMacros.cmake)

## -----------------------------------------------------------------------------------------------
## Sub dir (no comment)
## -----------------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(thirdparty)
ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(software)
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(textures)
ADD_SUBDIRECTORY(externalplugins)

ADD_SUBDIRECTORY(docs)

## -----------------------------------------------------------------------------------------------
## Set var to create tulip-config script
## -----------------------------------------------------------------------------------------------
SET(prefix "${CMAKE_INSTALL_PREFIX}")
SET(exec_prefix "\${prefix}")
SET(includedir "\${prefix}/${TulipIncludeInstallDir}")
SET(libdir "\${prefix}/${TulipLibInstallDir}")
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
## create pluginstest.cpp (does not exists in delivered source package)
## -----------------------------------------------------------------------------------------------
IF(EXISTS "unit_test/plugins/pluginstest.cpp.in")
CONFIGURE_FILE("unit_test/plugins/pluginstest.cpp.in"
"unit_test/plugins/pluginstest.cpp" @ONLY)
ENDIF()

## -----------------------------------------------------------------------------------------------
## Installation of files
## -----------------------------------------------------------------------------------------------
INSTALL(PROGRAMS ${PROJECT_BINARY_DIR}/tulip-config
        DESTINATION ${TulipBinInstallDir})
INSTALL(FILES FindTULIP3.cmake 
        DESTINATION ${TulipShareInstallDir})
INSTALL(FILES FindGLEW.cmake 
        DESTINATION ${TulipShareInstallDir})
INSTALL(FILES FindXML2.cmake 
        DESTINATION ${TulipShareInstallDir})
## -----------------------------------------------------------------------------------------------
## Script for linux Tulip bundle building
IF(LINUX)
CONFIGURE_FILE("make_tulip_linux_bundle.sh.in" "make_tulip_linux_bundle.sh" @ONLY)
ENDIF(LINUX)
## -----------------------------------------------------------------------------------------------
## CPack packaging directives
## -----------------------------------------------------------------------------------------------

# You can create your own installer by creating a setting the CPACK_CUSTOM_INSTALLER to true.
# If this variable is not set, directives in the CPackInstaller.cmake file will be used to set the project licence, maintainer etc.
# When making your own installer, please be sure to set at least all the variables present in the CPackInstaller.cmake file.
# Consult CPack documentation for additional information.

# Include external plugins configuration file.
# Note: this is done before setting group names variables. User can set the CPACK_CUSTOM_INSTALLER to true and setup its own installer in this file.

SET(LIBS_GROUP_NAME "Libraries")
SET(HEADERS_GROUP_NAME "Headers")
SET(SOFTWARE_GROUP_NAME "Runtime")
SET(PLUGINS_GROUP_NAME "Plugins")
SET(EXTRAS_GROUP_NAME "Extras")
SET(THIRDPARTY_GROUP_NAME "Third party")

# Defines specific software components from tulip.
INCLUDE(CPackComponents.cmake)

IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/externalplugins/ExternalPlugins.cmake)   
  INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/externalplugins/ExternalPlugins.cmake)
ENDIF()


IF(NOT CPACK_CUSTOM_INSTALLER)
  INCLUDE(CPackInstaller.cmake)
ENDIF()
INCLUDE(CPack)
