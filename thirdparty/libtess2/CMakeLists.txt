IF(WIN32)
  ADD_DEFINITIONS("-DDLL_TESS2")
ENDIF(WIN32)

SET(libtess2_SRCS
    Source/bucketalloc.c
    Source/dict.c
    Source/geom.c
    Source/mesh.c
    Source/priorityq.c
    Source/sweep.c
    Source/tess.c)

INCLUDE_DIRECTORIES(${Tess2Include})

ADD_LIBRARY(${Tess2Library} OBJECT ${libtess2_SRCS})
# allow the object files to be used to build shared libs
SET_TARGET_PROPERTIES(${Tess2Library} PROPERTIES POSITION_INDEPENDENT_CODE ON)
