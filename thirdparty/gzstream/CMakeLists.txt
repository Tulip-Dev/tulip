set(gzstream_STAT_SRCS gzstream.cpp gzstream.h)

INCLUDE_DIRECTORIES(.)

ADD_LIBRARY(gzstream STATIC ${gzstream_STAT_SRCS})


#EXTRA_DIST = 	COPYING.LIB \
#		index.html
