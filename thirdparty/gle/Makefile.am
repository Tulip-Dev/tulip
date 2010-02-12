noinst_LTLIBRARIES = libgle.la

AM_CFLAGS = -fPIC -Wstrict-prototypes  -Wconversion
INCLUDES = @GL_INCLUDES@ 
AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@

EXTRA_DIST = \
  AUTHORS \
  COPYING \
  COPYING.src \
  INSTALL \
  NEWS \
  README \
  README.gutil CMakeLists.txt

libgle_la_SOURCES = 	\
  ex_angle.c		\
  ex_cut_round.c	\
  ex_raw.c		\
  extrude.c		\
  intersect.c		\
  qmesh.c		\
  rot_prince.c		\
  rotate.c		\
  round_cap.c		\
  segment.c		\
  texgen.c		\
  urotate.c		\
  view.c		\
  gle.h	                \
  copy.h 		\
  extrude.h 		\
  intersect.h 		\
  port.h 		\
  rot.h 		\
  segment.h 		\
  tube_gc.h 		\
  vvector.h 		\
  config.h

libgle_la_LDFLAGS = -version-info 3:7:0



