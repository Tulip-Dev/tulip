######################################################################################
# File is taken and adapted from the Point Cloud Library (PCL) - www.pointclouds.org (BSD License)
# https://github.com/otherlab/pcl/blob/master/cmake/Modules/FindQhull.cmake
######################################################################################
# Find QHULL
#
# This sets the following variables:
# QHULL_FOUND - True if QHULL was found.
# QHULL_INCLUDE_DIRS - Directories containing the QHULL include files.
# QHULL_LIBRARIES - Libraries needed to use QHULL.
# QHULL_DEFINITIONS - Compiler flags for QHULL.
# If QHULL_USE_STATIC is specified then look for static libraries ONLY else 
# look for shared ones

SET(QHULL_MAJOR_VERSION 7)

# prefer static qhull on windows
IF(WIN32)
  SET(QHULL_USE_STATIC TRUE)
ENDIF(WIN32)

IF(QHULL_USE_STATIC)
  SET(QHULL_RELEASE_NAME qhullstatic)
  SET(QHULL_DEBUG_NAME qhullstatic_d)
ELSE(QHULL_USE_STATIC)
  SET(QHULL_RELEASE_NAME qhull qhull${QHULL_MAJOR_VERSION})
  SET(QHULL_DEBUG_NAME qhull_d qhull${QHULL_MAJOR_VERSION}_d qhull_d${QHULL_MAJOR_VERSION})
ENDIF(QHULL_USE_STATIC)

FIND_FILE(QHULL_HEADER
          NAMES libqhull/libqhull.h libqhull.h qhull.h
          HINTS "${QHULL_ROOT}" "$ENV{QHULL_ROOT}" "${QHULL_INCLUDE_DIR}"
          PATHS "$ENV{PROGRAMFILES}/QHull" "$ENV{PROGRAMW6432}/QHull" 
          PATH_SUFFIXES qhull src/libqhull libqhull include)

SET(QHULL_HEADER "${QHULL_HEADER}" CACHE INTERNAL "QHull header" FORCE )

IF(QHULL_HEADER)
  GET_FILENAME_COMPONENT(qhull_header ${QHULL_HEADER} NAME_WE)
  IF("${qhull_header}" STREQUAL "qhull")
    SET(HAVE_QHULL_2011 OFF)
    GET_FILENAME_COMPONENT(QHULL_INCLUDE_DIR ${QHULL_HEADER} PATH)
  ELSEIF("${qhull_header}" STREQUAL "libqhull")
    SET(HAVE_QHULL_2011 ON)
    GET_FILENAME_COMPONENT(QHULL_INCLUDE_DIR ${QHULL_HEADER} PATH)
    GET_FILENAME_COMPONENT(QHULL_INCLUDE_DIR ${QHULL_INCLUDE_DIR} PATH)
    GET_FILENAME_COMPONENT(qhull_header_dir ${QHULL_HEADER} DIRECTORY)
    # special case for MSYS2 where qhull includes are located in a 'qhull' directory instead of a 'libqhull' one
    STRING(REGEX MATCH ".*/qhull$" LIBQHULL_OTHER_INCLUDE_PREFIX "${qhull_header_dir}")
  ENDIF()
ELSE(QHULL_HEADER)
  SET(QHULL_INCLUDE_DIR "QHULL_INCLUDE_DIR-NOTFOUND")
ENDIF(QHULL_HEADER)

SET(QHULL_INCLUDE_DIR "${QHULL_INCLUDE_DIR}" CACHE PATH "QHull include dir." FORCE)

FIND_LIBRARY(QHULL_LIBRARY
             NAMES ${QHULL_RELEASE_NAME}
             HINTS "${QHULL_ROOT}" "$ENV{QHULL_ROOT}"
             PATHS "$ENV{PROGRAMFILES}/QHull" "$ENV{PROGRAMW6432}/QHull" 
             PATH_SUFFIXES project build bin lib)

FIND_LIBRARY(QHULL_LIBRARY_DEBUG
             NAMES ${QHULL_DEBUG_NAME} ${QHULL_RELEASE_NAME}
             HINTS "${QHULL_ROOT}" "$ENV{QHULL_ROOT}"
             PATHS "$ENV{PROGRAMFILES}/QHull" "$ENV{PROGRAMW6432}/QHull" 
             PATH_SUFFIXES project build bin lib)

IF(NOT QHULL_LIBRARY_DEBUG)
  SET(QHULL_LIBRARY_DEBUG ${QHULL_LIBRARY})
ENDIF(NOT QHULL_LIBRARY_DEBUG)

SET(QHULL_INCLUDE_DIRS ${QHULL_INCLUDE_DIR})
SET(QHULL_LIBRARIES optimized ${QHULL_LIBRARY} debug ${QHULL_LIBRARY_DEBUG})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Qhull DEFAULT_MSG QHULL_LIBRARY QHULL_INCLUDE_DIR)

MARK_AS_ADVANCED(QHULL_LIBRARY QHULL_LIBRARY_DEBUG QHULL_INCLUDE_DIR)
