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

FIND_PACKAGE(PythonInterp)

IF(PYTHONINTERP_FOUND)

EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} --version OUTPUT_VARIABLE PYTHON_VERSION_RAW ERROR_VARIABLE PYTHON_VERSION_RAW)
STRING(REPLACE "\n" "" PYTHON_VERSION_RAW "${PYTHON_VERSION_RAW}")
STRING(REGEX MATCH "[0-9]" PYTHON_VERSION_MAJOR "${PYTHON_VERSION_RAW}")

IF(WIN32)
STRING(REPLACE "python.exe" "Scripts/" SPHINX_PATH "${PYTHON_EXECUTABLE}")
ELSEIF(LINUX)
SET(SPHINX_PATH "/usr/share/sphinx/scripts/python${PYTHON_VERSION_MAJOR}/")
ELSE()
SET(SPHINX_PATH "")
ENDIF()

FIND_PROGRAM(SPHINX_EXECUTABLE NAMES sphinx-build
  HINTS ${SPHINX_PATH}
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

ENDIF(PYTHONINTERP_FOUND)
