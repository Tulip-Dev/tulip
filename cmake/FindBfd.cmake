# - Find Bfd
# This module finds libbfd.
#
# It sets the following variables:
#  BFD_INCLUDE_DIR
#  BFD_LIBRARY

FIND_PATH(BFD_INCLUDE_DIR NAMES bfd.h)
FIND_LIBRARY(BFD_LIBRARY NAMES bfd)

IF(BFD_INCLUDE_DIR AND BFD_LIBRARY)

  #MESSAGE(STATUS "Found bfd header file: ${BFD_INCLUDE_DIR}/bfd.h")
  #MESSAGE(STATUS "Found bfd library: ${BFD_LIBRARY}")

  SET(BFD_FOUND TRUE)

ENDIF(BFD_INCLUDE_DIR AND BFD_LIBRARY)
