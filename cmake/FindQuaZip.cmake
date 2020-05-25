# QUAZIP_FOUND               - QuaZip library was found
# QUAZIP_INCLUDE_DIR         - Path to QuaZip include dir
# QUAZIP_INCLUDE_DIRS        - Path to QuaZip and zlib include dir (combined from QUAZIP_INCLUDE_DIR + ZLIB_INCLUDE_DIR)
# QUAZIP_LIBRARIES           - List of QuaZip libraries
# QUAZIP_ZLIB_INCLUDE_DIR    - The include dir of zlib headers

# Unset related CMake variables in order to change the lib version without having to delete the current CMake cache
UNSET(QUAZIP_FOUND CACHE)
UNSET(QUAZIP_LIBRARIES CACHE)
UNSET(QUAZIP_INCLUDE_DIR CACHE)
UNSET(QUAZIP_INCLUDE_DIRS CACHE)
UNSET(QUAZIP_ZLIB_INCLUDE_DIR CACHE)

IF (WIN32)
  FIND_PATH(QUAZIP_LIBRARY_DIR
    WIN32_DEBUG_POSTFIX d
    NAMES libquazip.dll
    HINTS "C:/Programme/" "C:/Program Files"
    PATH_SUFFIXES QuaZip/lib
  )
  SET(QUAZIP_INCLUDE_PATH_SUFFIXES include/quazip)
  SET(QUAZIP_DLL_NAMES libquazip.dll)
  # special case when using Qt5 on MSYS2
  SET(QUAZIP_INCLUDE_PATH_SUFFIXES include/quazip5 ${QUAZIP_INCLUDE_PATH_SUFFIXES})
  SET(QUAZIP_DLL_NAMES libquazip5.dll ${QUAZIP_DLL_NAMES})
  FIND_LIBRARY(QUAZIP_LIBRARIES NAMES ${QUAZIP_DLL_NAMES} HINTS ${QUAZIP_LIBRARY_DIR})
  FIND_PATH(QUAZIP_INCLUDE_DIR NAMES quazip.h HINTS ${QUAZIP_LIBRARY_DIR}/../ PATH_SUFFIXES ${QUAZIP_INCLUDE_PATH_SUFFIXES})
  FIND_PATH(QUAZIP_ZLIB_INCLUDE_DIR NAMES zlib.h)
ELSE(WIN32)  

  # special case when using Qt5 on unix
  SET(QUAZIP_LIBRARY_NAMES quazip5 quazip-qt5)
  IF (APPLE)
    # needed by homebrew on MacOS
    SET(QUAZIP_LIBRARY_NAMES ${QUAZIP_LIBRARY_NAMES} quazip)
  ENDIF(APPLE)
  FIND_LIBRARY(QUAZIP_LIBRARIES
    NAMES ${QUAZIP_LIBRARY_NAMES}
    HINTS /usr/lib /usr/lib64 /usr/local/lib /opt/local/lib
  )

  # special case when using Qt5 on unix
  SET(QUAZIP_PATH_SUFFIXES quazip5 quazip)
  FIND_PATH(QUAZIP_INCLUDE_DIR quazip.h
  HINTS /usr/include /usr/local/include /usr/local/include/quazip
  PATH_SUFFIXES ${QUAZIP_PATH_SUFFIXES}
  )
  FIND_PATH(QUAZIP_ZLIB_INCLUDE_DIR zlib.h HINTS /usr/include /usr/local/include)
ENDIF (WIN32)

INCLUDE(FindPackageHandleStandardArgs)
SET(QUAZIP_INCLUDE_DIRS ${QUAZIP_INCLUDE_DIR} ${QUAZIP_ZLIB_INCLUDE_DIR})
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QuaZip DEFAULT_MSG QUAZIP_LIBRARIES QUAZIP_INCLUDE_DIR QUAZIP_ZLIB_INCLUDE_DIR QUAZIP_INCLUDE_DIRS)
