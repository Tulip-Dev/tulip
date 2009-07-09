SET(gzstream_SRCS
    gzstream.cpp 
    gzstream.h
)

INCLUDE_DIRECTORIES(./)

ADD_LIBRARY(gzstream STATIC ${gzstream_SRCS})
