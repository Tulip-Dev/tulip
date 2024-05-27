# - This module looks for Sphinx
# Find the Sphinx documentation generator
#
# This modules defines
#  SPHINX_EXECUTABLE
#  SPHINX_FOUND

#=============================================================================
# Copyright 2002-2009 Kitware, Inc.
# Copyright 2009-2011 Peter Colberg
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file COPYING-CMAKE-SCRIPTS for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

IF(WIN32)
STRING(REPLACE "python.exe" "Scripts/" SPHINX_PATH "${Python_EXECUTABLE}")
ELSEIF(LINUX)
GET_FILENAME_COMPONENT(PYTHON_EXECUTABLE_PATH "${Python_EXECUTABLE}" DIRECTORY)
# remove final /bin component
GET_FILENAME_COMPONENT(PYTHON_EXECUTABLE_PATH "${PYTHON_EXECUTABLE_PATH}" DIRECTORY)
SET(SPHINX_PATH "/usr/share/sphinx/scripts/python${Python_VERSION_MAJOR}/" "${PYTHON_EXECUTABLE_PATH}")
ELSE()
#STRING(REGEX MATCH "[0-9].[0-9]" PYTHON_VERSION_MAJOR_MINOR "${PYTHON_VERSION_RAW}")
SET(SPHINX_PATH "$ENV{HOME}/Library/Python/${Python_VERSION}")
ENDIF()

FIND_PROGRAM(SPHINX_EXECUTABLE NAMES sphinx-build3 sphinx-build
  HINTS ${SPHINX_PATH}
  PATHS /usr /usr/local /opt/local
  PATH_SUFFIXES bin
  DOC "Sphinx documentation generator"
)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Sphinx DEFAULT_MSG
  SPHINX_EXECUTABLE
)

MARK_AS_ADVANCED(
  SPHINX_EXECUTABLE
)

