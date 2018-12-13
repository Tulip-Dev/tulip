IF(WIN32)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDLL_TESS2")
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

ADD_LIBRARY(${Tess2Library} SHARED ${libtess2_SRCS})

INSTALL(TARGETS ${Tess2Library}
        RUNTIME DESTINATION ${TulipBinInstallDir}
        LIBRARY DESTINATION ${TulipLibInstallDir}
        ARCHIVE DESTINATION ${TulipLibInstallDir})
