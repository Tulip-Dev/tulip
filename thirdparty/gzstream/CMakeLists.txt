ADD_CORE_FILES(.)

SET(gzstream_SRCS
    gzstream.cpp
    gzstream.h
)

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(${GZStreamLibrary} STATIC ${gzstream_SRCS})
# allow the static library to be used to build shared libs
SET_TARGET_PROPERTIES(${GZStreamLibrary} PROPERTIES POSITION_INDEPENDENT_CODE ON)
TARGET_LINK_LIBRARIES(${GZStreamLibrary} ${ZLIB_LIBRARY})
