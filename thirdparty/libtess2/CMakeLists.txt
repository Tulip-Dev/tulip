SET(LibTess2_SRCS
    Source/bucketalloc.c
    Source/dict.c
    Source/geom.c
    Source/mesh.c
    Source/priorityq.c
    Source/sweep.c
    Source/tess.c)

INCLUDE_DIRECTORIES(${Tess2Include})

ADD_LIBRARY(${Tess2Library} SHARED ${LibTess2_SRCS})

INSTALL(TARGETS ${Tess2Library}
        RUNTIME DESTINATION ${TulipBinInstallDir}
        LIBRARY DESTINATION ${TulipLibInstallDir}
        ARCHIVE DESTINATION ${TulipLibInstallDir})

IF(ACTIVATE_PYTHON_WHEELS_TARGETS)
COPY_TARGET_LIBRARY_POST_BUILD(${Tess2Library} ${TULIPOGL_PYTHON_NATIVE_FOLDER})
ENDIF(ACTIVATE_PYTHON_WHEELS_TARGETS)
