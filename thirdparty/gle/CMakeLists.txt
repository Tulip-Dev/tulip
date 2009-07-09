set(gle_STAT_SRCS
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


INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

ADD_LIBRARY(gle STATIC ${gle_STAT_SRCS})


#EXTRA_DIST = \
#  AUTHORS \
#  COPYING \
#  COPYING.src \
#  INSTALL \
#  NEWS \
#  README \
#  README.gutil
#
#libgle_la_SOURCES = 	\
#  ex_angle.c		\
#  ex_cut_round.c	\
#  ex_raw.c		\
#  extrude.c		\
#  intersect.c		\
#  qmesh.c		\
#  rot_prince.c		\
#  rotate.c		\
#  round_cap.c		\
#  segment.c		\
#  texgen.c		\
#  urotate.c		\
#  view.c		\
#  gle.h	                \
#  copy.h 		\
#  extrude.h 		\
#  intersect.h 		\
#  port.h 		\
#  rot.h 		\
#  segment.h 		\
#  tube_gc.h 		\
#  vvector.h 		\
#  config.h
#
#libgle_la_LDFLAGS = -version-info 3:7:0
#
#
#
