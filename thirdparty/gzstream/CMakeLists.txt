ADD_CORE_FILES(.)

SET(gzstream_SRCS
    gzstream.cpp
    gzstream.h
)

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# add -fPIC to allow the produced static library to be used
# to build a shared library
IF(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

ADD_LIBRARY(${GZStreamLibrary} STATIC ${gzstream_SRCS})
TARGET_LINK_LIBRARIES(${GZStreamLibrary} ${ZLIB_LIBRARY})
