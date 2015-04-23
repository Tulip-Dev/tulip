#===========================================================================
#
#    Description: Set up CMake environment variables pointing to Tulip components one can use to compile code using Tulip
#    Filename: FindTULIP.cmake
#    Authors: Tulip team
#
#===========================================================================
#
# The FindTULIP script can be used from any CMake-based project to set up environment variables poiting to Tulip libraries and headers directories.
# The FIND_PACKAGE command provides several arguments. While the COMPONENTS argument will be ignored, you can tell FindTULIP to look for a specific version of Tulip libraries:.
#
# FIND_PACKAGE(Tulip 3.5.0 REQUIRED) # Will look for Tulip 3.5.0 or higher
# See CMake documentation for details about the FIND_PACKAGE macro.
#
# By default, this script will try to automatically detect the installation directory of Tulip libraries installed on your system.
# You can override this behavior by defining the TULIP_DIR variable. If this variable is set, FindTulip will only check into that directory for a valid Tulip installation.
#
# This script will output the following variables:
#
# TULIP_DIR (will not be modified if already set): The Tulip installation path
# TULIP_FOUND: This variable is set to true if a valid Tulip installation has been found.
# TULIP_INCLUDE_DIR: The path where Tulip headers are located.
# TULIP_LIBRARIES: Contains path to the Tulip libraries
# TULIP_LIBRARIES_DIR: Contains path to the Tulip libraries
# TULIP_PLUGINS_DIR: Path where Tulip plugins are located
# TULIP_GLYPH_PLUGINS_DIR: Path where Tulip glyph plugins are located
# TULIP_INTERACTOR_PLUGINS_DIR: Path where Tulip interaction plugins are located
# TULIP_VIEW_PLUGINS_DIR: Path where Tulip view plugins are located
# TULIP_PERSPECTIVE_PLUGINS_DIR: Path where Tulip perspective plugins are located
# TULIP_CORE_LIBRARY: The path to the TulipCore module library.
# TULIP_OGL_LIBRARY: The path to the TulipOGL module library.
# TULIP_GUI_LIBRARY: The path to the TulipGUI module library.
# TULIP_OGDF_LIBRARY: The path to the TulipOGDF module library.
# TULIP_PYTHON_LIBRARY: The path to the TulipPython module library.
# TULIP_SHARE_DIR: Installation path for resources
# TULIP_VERSION: Complete version string.
# TULIP_MAJOR_VERSION: Major version digit.
# TULIP_MINOR_VERSION: Minor version digit.
# TULIP_PATCH_VERSION: Patch version digit.
# TULIP_USE_FILE: Additional Tulip-related macro definitions

# Define version separator (different for MSVC builds)
SET(TulipVersionSeparator .)
IF(${CMAKE_GENERATOR} MATCHES "Visual Studio" OR ${CMAKE_GENERATOR} MATCHES "NMake Makefiles") #visual studio does not recognize these options
   SET(TulipVersionSeparator _)
ENDIF()

# A macro to retrieve Tulip version from the TulipRelase.h file
# This will output the following variables:
# TULIP_VERSION: Complete version string.
# TULIP_MAJOR_VERSION: Major version digit.
# TULIP_MINOR_VERSION: Minor version digit.
# TULIP_PATCH_VERSION: Patch version digit.
MACRO(RETRIEVE_VERSION)
  # Unset previous variables
  SET(TULIP_VERSION)
  SET(TULIP_MAJOR_VERSION)
  SET(TULIP_MINOR_VERSION)
  SET(TULIP_PATCH_VERSION)

  # Check for TulipRelease.h
  IF(TULIP_INCLUDE_DIR)
    FILE(STRINGS ${TULIP_INCLUDE_DIR}/tulip/TulipRelease.h
         TMPSTR
         REGEX "[0-9]*\\${TulipVersionSeparator}[0-9]*"
         NO_HEX_CONVERSION)

    STRING(REGEX MATCH "[0-9]*\\${TulipVersionSeparator}[0-9]*\\${TulipVersionSeparator}[0-9]*"
           TULIP_VERSION
           ${TMPSTR})

    STRING(REGEX REPLACE "([0-9]*)\\${TulipVersionSeparator}([0-9]*)\\${TulipVersionSeparator}([0-9]*)"
            "\\1"
           TULIP_MAJOR_VERSION
           ${TULIP_VERSION})

    STRING(REGEX REPLACE "([0-9]*)\\${TulipVersionSeparator}([0-9]*)\\${TulipVersionSeparator}([0-9]*)"
            "\\2"
           TULIP_MINOR_VERSION
           ${TULIP_VERSION})

    STRING(REGEX REPLACE "([0-9]*)\\${TulipVersionSeparator}([0-9]*)\\${TulipVersionSeparator}([0-9]*)"
            "\\3"
           TULIP_PATCH_VERSION
           ${TULIP_VERSION})
  ENDIF(TULIP_INCLUDE_DIR)
ENDMACRO(RETRIEVE_VERSION)


# Check if the given version corresponds to the one asked when calling FIND_PACKAGE
# This will exit with a fatal error if provided version is considered as not compatible with requested version
MACRO(CHECK_VERSION)
  IF(TULIP_FIND_VERSION)
    IF(NOT ${TULIP_VERSION_MAJOR} EQUAL ${TULIP_FIND_VERSION_MAJOR})
      MESSAGE(FATAL_ERROR "Invalid Tulip version string given: ${TULIP_FIND_VERSION}, detected version is ${TULIP_VERSION}")
    ELSEIF(${TULIP_VERSION_MINOR} LESS ${TULIP_FIND_VERSION_MINOR})
      MESSAGE(FATAL_ERROR "Invalid Tulip version string given: ${TULIP_FIND_VERSION}, detected version is ${TULIP_VERSION}")
    ENDIF()
  ENDIF(TULIP_FIND_VERSION)
ENDMACRO(CHECK_VERSION)

# CMAKE_FIND_ROOT_PATH_MODE_* are set to ONLY in the CMake platform file for emscripten
# which prevents FIND_* commands to work correctly, so reset them to BOTH
IF(EMSCRIPTEN)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
ENDIF(EMSCRIPTEN)

# First step: set up the TULIP_DIR variable
IF(TULIP_DIR) # TULIP_DIR is already set
  IF(NOT TULIP_INCLUDE_DIR)
    FIND_PATH(TULIP_INCLUDE_DIR tulip/TulipRelease.h
              PATHS ${TULIP_DIR}/include
              NO_DEFAULT_PATH)          
  ENDIF(NOT TULIP_INCLUDE_DIR)
  RETRIEVE_VERSION()
  CHECK_VERSION()
ELSE(TULIP_DIR)

  IF(NOT TULIP_INCLUDE_DIR)
    FIND_PATH(TULIP_INCLUDE_DIR tulip/TulipRelease.h)
    IF(NOT TULIP_INCLUDE_DIR) # Last resort: use CMAKE_MODULE_PATH/../../include/
      FOREACH(p ${CMAKE_MODULE_PATH})
        FIND_PATH(TULIP_INCLUDE_DIR tulip/TulipRelease.h PATHS "${p}/../../include")
        IF(TULIP_INCLUDE_DIR)
          BREAK()
        ENDIF(TULIP_INCLUDE_DIR)
      ENDFOREACH()
    ENDIF(NOT TULIP_INCLUDE_DIR)
  ENDIF(NOT TULIP_INCLUDE_DIR)

  IF(NOT TULIP_INCLUDE_DIR)
    MESSAGE(FATAL_ERROR "Auto-detecting your Tulip headers installation directory failed. Please try to provide the TULIP_DIR CMake variable poiting to your Tulip installation root directory")
  ELSE()
    SET(TULIP_DIR ${TULIP_INCLUDE_DIR}/../)
  ENDIF(NOT TULIP_INCLUDE_DIR)

  RETRIEVE_VERSION()
  CHECK_VERSION()
ENDIF(TULIP_DIR)

MARK_AS_ADVANCED(TULIP_DIR)
MARK_AS_ADVANCED(TULIP_INCLUDE_DIR)
IF(WIN32)
  SET(TULIP_LIBRARIES_DIR ${TULIP_DIR}/bin)
ELSE()
  SET(TULIP_LIBRARIES_DIR ${TULIP_DIR}/lib)
ENDIF()
MARK_AS_ADVANCED(TULIP_LIBRARIES_DIR)
SET(TULIP_PLUGINS_DIR ${TULIP_DIR}/lib/tulip)
SET(TULIP_SHARE_DIR ${TULIP_DIR}/share/tulip)
MARK_AS_ADVANCED(TULIP_PLUGINS_DIR)
SET(TULIP_GLYPH_PLUGINS_DIR ${TULIP_PLUGINS_DIR}/glyph)
MARK_AS_ADVANCED(TULIP_GLYPH_PLUGINS_DIR)
SET(TULIP_INTERACTOR_PLUGINS_DIR ${TULIP_PLUGINS_DIR}/interactor)
MARK_AS_ADVANCED(TULIP_INTERACTOR_PLUGINS_DIR)
SET(TULIP_VIEW_PLUGINS_DIR ${TULIP_PLUGINS_DIR}/view)
MARK_AS_ADVANCED(TULIP_VIEW_PLUGINS_DIR)
SET(TULIP_PERSPECTIVE_PLUGINS_DIR ${TULIP_PLUGINS_DIR}/perspective)
MARK_AS_ADVANCED(TULIP_PERSPECTIVE_PLUGINS_DIR)
SET(TULIP_FOUND true)

IF(WIN32 AND NOT MSVC)
  SET(TULIP_LIBRARIES_DIR "${TULIP_DIR}/bin")
ELSE()
  SET(TULIP_LIBRARIES_DIR ${TULIP_DIR}/lib)
ENDIF()

FIND_LIBRARY(TULIP_CORE_LIBRARY "tulip-core-${TULIP_MAJOR_VERSION}${TulipVersionSeparator}${TULIP_MINOR_VERSION}"
             PATHS ${TULIP_LIBRARIES_DIR}
             NO_DEFAULT_PATH)

# There is no dynamic linking with emscripten, add dependencies libraries in the TULIP_CORE_LIBRARY variable
IF(EMSCRIPTEN)
FIND_LIBRARY(GZSTREAM_TULIP_LIBRARY "gzstream-tulip-${TULIP_MAJOR_VERSION}${TulipVersionSeparator}${TULIP_MINOR_VERSION}"
             PATHS ${TULIP_LIBRARIES_DIR}
             NO_DEFAULT_PATH)
FIND_LIBRARY(YAJL_TULIP_LIBRARY "yajl-tulip-${TULIP_MAJOR_VERSION}${TulipVersionSeparator}${TULIP_MINOR_VERSION}"
             PATHS ${TULIP_LIBRARIES_DIR}
             NO_DEFAULT_PATH)
SET(TULIP_CORE_LIBRARY ${TULIP_CORE_LIBRARY} ${GZSTREAM_TULIP_LIBRARY} ${YAJL_TULIP_LIBRARY})
ENDIF(EMSCRIPTEN)
             
FIND_LIBRARY(TULIP_OGL_LIBRARY "tulip-ogl-${TULIP_MAJOR_VERSION}${TulipVersionSeparator}${TULIP_MINOR_VERSION}"
             PATHS ${TULIP_LIBRARIES_DIR}
             NO_DEFAULT_PATH)

FIND_LIBRARY(TULIP_GUI_LIBRARY "tulip-gui-${TULIP_MAJOR_VERSION}${TulipVersionSeparator}${TULIP_MINOR_VERSION}"
             PATHS ${TULIP_LIBRARIES_DIR}
             NO_DEFAULT_PATH)

FIND_LIBRARY(TULIP_OGDF_LIBRARY "tulip-ogdf-${TULIP_MAJOR_VERSION}${TulipVersionSeparator}${TULIP_MINOR_VERSION}"
             PATHS ${TULIP_LIBRARIES_DIR}
             NO_DEFAULT_PATH)

# There is no dynamic linking with emscripten, add dependency library in the TULIP_OGDF_LIBRARY variable
IF(EMSCRIPTEN)
FIND_LIBRARY(OGDF_TULIP_LIBRARY "OGDF-tulip-${TULIP_MAJOR_VERSION}${TulipVersionSeparator}${TULIP_MINOR_VERSION}"
             PATHS ${TULIP_LIBRARIES_DIR}
             NO_DEFAULT_PATH)
SET(TULIP_OGDF_LIBRARY ${TULIP_OGDF_LIBRARY} ${OGDF_TULIP_LIBRARY})
ENDIF(EMSCRIPTEN)
             
FIND_LIBRARY(TULIP_PYTHON_LIBRARY "tulip-python-${TULIP_MAJOR_VERSION}${TulipVersionSeparator}${TULIP_MINOR_VERSION}"
             PATHS ${TULIP_LIBRARIES_DIR}
             NO_DEFAULT_PATH)

IF((TULIP_OGL_LIBRARY AND NOT TULIP_CORE_LIBRARY) OR (TULIP_GUI_LIBRARY AND NOT TULIP_OGL_LIBRARY) OR (TULIP_OGDF_LIBRARY AND NOT TULIP_CORE_LIBRARY) OR (TULIP_PYTHON_LIBRARY AND NOT TULIP_CORE_LIBRARY))
  MESSAGE("Dependency check between Tulip libraries failed. Detected libraries are:")
  MESSAGE("TulipCore: ${TULIP_CORE_LIBRARY}")
  MESSAGE("TulipOGL: ${TULIP_OGL_LIBRARY} (depends on TulipCore)")
  MESSAGE("TulipGUI: ${TULIP_GUI_LIBRARY} (depends on TulipOGL)")
  MESSAGE("TulipOGDF: ${TULIP_OGDF_LIBRARY} (depends on TulipCore)")
  MESSAGE("TulipPython: ${TULIP_PYTHON_LIBRARY} (depends on TulipCore)")
  MESSAGE(FATAL_ERROR "Please check that dependencies for detected libraries are met.")
ENDIF()

SET(TULIP_LIBRARIES)
IF(TULIP_CORE_LIBRARY)
  MARK_AS_ADVANCED(TULIP_CORE_LIBRARY)
  SET(TULIP_LIBRARIES ${TULIP_LIBRARIES} ${TULIP_CORE_LIBRARY})
ENDIF(TULIP_CORE_LIBRARY)

IF(TULIP_OGL_LIBRARY)
  MARK_AS_ADVANCED(TULIP_OGL_LIBRARY)
  SET(TULIP_LIBRARIES ${TULIP_LIBRARIES} ${TULIP_OGL_LIBRARY})
ENDIF(TULIP_OGL_LIBRARY)

IF(TULIP_GUI_LIBRARY)
  MARK_AS_ADVANCED(TULIP_GUI_LIBRARY)
  SET(TULIP_LIBRARIES ${TULIP_LIBRARIES} ${TULIP_GUI_LIBRARY})
ENDIF(TULIP_GUI_LIBRARY)

IF(TULIP_OGDF_LIBRARY)
  MARK_AS_ADVANCED(TULIP_OGDF_LIBRARY)
  SET(TULIP_LIBRARIES ${TULIP_LIBRARIES} ${TULIP_OGDF_LIBRARY})
ENDIF(TULIP_OGDF_LIBRARY)

IF(TULIP_PYTHON_LIBRARY)
  MARK_AS_ADVANCED(TULIP_PYTHON_LIBRARY)
ENDIF(TULIP_PYTHON_LIBRARY)

SET(TULIP_USE_FILE "${TULIP_DIR}/share/tulip/TulipUseFile.cmake")
