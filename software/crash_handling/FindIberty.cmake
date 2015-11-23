# - Find Iberty
# This module finds libiberty.
#
# It sets the following variables:
#  IBERTY_LIBRARY

FIND_PATH(IBERTY_INCLUDE_DIR NAMES libiberty.h)
FIND_LIBRARY(IBERTY_LIBRARY NAMES iberty)

IF(IBERTY_INCLUDE_DIR AND IBERTY_LIBRARY)

  #MESSAGE(STATUS "Found iberty header file: ${IBERTY_INCLUDE_DIR}/libiberty.h")
  #MESSAGE(STATUS "Found iberty library: ${IBERTY_LIBRARY}")

  SET(IBERTY_FOUND TRUE)

ENDIF(IBERTY_INCLUDE_DIR AND IBERTY_LIBRARY)
