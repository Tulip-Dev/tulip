ADD_CORE_FILES(.)

SET(QHULL_LIB_SRCS
    libqhull.h
    geom.h
    io.h
    mem.h
    merge.h
    poly.h
    qhull_a.h
    qset.h
    random.h
    stat.h
    user.h
    global.c
    stat.c
    geom2.c
    poly2.c
    merge.c
    libqhull.c
    geom.c
    poly.c
    qset.c
    mem.c
    random.c
    usermem.c
    userprintf.c
    io.c
    user.c
    rboxlib.c
    userprintf_rbox.c
)

IF(UNIX)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
ENDIF(UNIX)

IF(MINGW AND I64)
        ADD_DEFINITIONS(-D_WIN64)
ENDIF(MINGW AND I64)

ADD_LIBRARY(qhull STATIC ${QHULL_LIB_SRCS})

IF(UNIX)
    TARGET_LINK_LIBRARIES(qhull m)
ENDIF(UNIX)
