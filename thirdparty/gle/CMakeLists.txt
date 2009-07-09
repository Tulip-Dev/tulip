SET(gle_SRCS
    ex_angle.c
    ex_cut_round.c
    ex_raw.c
    extrude.c
    intersect.c
    qmesh.c
    rot_prince.c
    rotate.c
    round_cap.c
    segment.c
    texgen.c
    urotate.c
    view.c
    gle.h
    copy.h
    extrude.h
    intersect.h
    port.h
    rot.h
    segment.h
    tube_gc.h
    vvector.h
    config.h)


#INCLUDE_DIRECTORIES(${OpenGLIncludeDirectory})

ADD_LIBRARY(gle STATIC ${gle_SRCS})
