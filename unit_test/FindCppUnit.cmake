# Find CppUnit includes and library
#
# This module defines
#  CppUnit_INCLUDE_DIRS
#  CppUnit_LIBRARIES, the libraries to link against to use CppUnit.
#  CppUnit_LIBRARY_DIRS, the location of the libraries
#  CppUnit_FOUND, If false, do not try to use CppUnit
#
# Copyright © 2007, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.

IF(WIN32) #Windows
  SET(CppUnit_INCLUDE_SEARCH_DIRS
        ${CppUnit_LIBRARY_SEARCH_DIRS}
        ${CMAKE_INCLUDE_PATH}
        /usr/include/
        /usr/local/
        /opt/include/
  )
  SET(CppUnit_LIBRARY_SEARCH_DIRS
        ${CppUnit_LIBRARY_SEARCH_DIRS}
        ${CMAKE_LIBRARY_PATH}
        /usr/lib
        /usr/local/lib
        /opt/lib
  )
  FIND_PATH(CppUnit_INCLUDE_DIRS cppunit/Test.h ${CppUnit_INCLUDE_SEARCH_DIRS})
  IF(MSVC)
    # prefer the cppunit dll with Visual Studio
    FIND_LIBRARY(CppUnit_LIBRARIES cppunit_dll PATHS ${CppUnit_LIBRARY_SEARCH_DIRS})
  ELSE(MSVC)
    FIND_LIBRARY(CppUnit_LIBRARIES cppunit PATHS ${CppUnit_LIBRARY_SEARCH_DIRS})
  ENDIF(MSVC)
ELSE(WIN32) #Unix
  CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)
  FIND_PACKAGE(PkgConfig)
  PKG_SEARCH_MODULE(CppUnit cppunit)
  SET(CppUnit_INCLUDE_DIRS ${CppUnit_INCLUDE_DIRS})
  SET(CppUnit_LIBRARY_DIRS ${CppUnit_LIBDIR})
  SET(CppUnit_LIBRARIES ${CppUnit_LIBRARIES} CACHE STRING "")
ENDIF(WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(CppUnit_INCLUDE_DIRS)
SEPARATE_ARGUMENTS(CppUnit_LIBRARIES)

SET(CppUnit_INCLUDE_DIRS ${CppUnit_INCLUDE_DIRS})
SET(CppUnit_LIBRARIES ${CppUnit_LIBRARIES})
SET(CppUnit_LIBRARY_DIRS ${CppUnit_LIBRARY_DIRS})

MARK_AS_ADVANCED(CppUnit_INCLUDE_DIRS CppUnit_LIBRARIES CppUnit_LIBRARY_DIRS)

IF(CppUnit_INCLUDE_DIRS AND CppUnit_LIBRARIES)
  SET(CppUnit_FOUND TRUE)
ENDIF(CppUnit_INCLUDE_DIRS AND CppUnit_LIBRARIES)

IF(NOT CppUnit_FOUND)
  IF(CppUnit_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find CppUnit")
  ENDIF(CppUnit_FIND_REQUIRED)
ENDIF(NOT CppUnit_FOUND)
