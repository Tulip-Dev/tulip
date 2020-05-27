ADD_CORE_FILES(.)

SET(QHULL_LIB_SRCS
    global_r.c
    stat_r.c
    geom2_r.c
    poly2_r.c
    merge_r.c
    libqhull_r.c
    geom_r.c
    poly_r.c
    qset_r.c
    mem_r.c
    random_r.c
    usermem_r.c
    userprintf_r.c
    io_r.c
    user_r.c
    rboxlib_r.c
    userprintf_rbox_r.c
)

IF(MINGW AND X86_64)
    ADD_DEFINITIONS(-D_WIN64)
ENDIF(MINGW AND X86_64)

ADD_LIBRARY(${QhullLibrary} STATIC ${QHULL_LIB_SRCS})
# allow the static library to be used to build shared libs
SET_TARGET_PROPERTIES(${QhullLibrary} PROPERTIES POSITION_INDEPENDENT_CODE ON)

IF(UNIX)
    TARGET_LINK_LIBRARIES(${QhullLibrary} m)
ENDIF(UNIX)
