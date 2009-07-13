SET(gzstream_SRCS
    gzstream.cpp 
    gzstream.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(gzstream STATIC ${gzstream_SRCS})
TARGET_LINK_LIBRARIES(gzstream z)
