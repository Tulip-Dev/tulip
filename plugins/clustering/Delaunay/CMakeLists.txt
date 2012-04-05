INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TulipInclude} ${TulipBuildInclude} libqhull)

SET(QHULL_LIB_SRCS
  libqhull/global.c
  libqhull/stat.c
  libqhull/geom2.c
  libqhull/poly2.c
  libqhull/merge.c
  libqhull/libqhull.c
  libqhull/geom.c
  libqhull/poly.c
  libqhull/qset.c
  libqhull/mem.c
  libqhull/random.c
  libqhull/usermem.c
  libqhull/userprintf.c
  libqhull/io.c
  libqhull/user.c
  libqhull/rboxlib.c
  libqhull/userprintf_rbox.c
  libqhull/libqhull.h
  libqhull/geom.h
  libqhull/io.h
  libqhull/mem.h
  libqhull/merge.h
  libqhull/poly.h
  libqhull/qhull_a.h
  libqhull/qset.h
  libqhull/random.h
  libqhull/stat.h
  libqhull/user.h
)

IF(UNIX)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
ENDIF(UNIX)

ADD_LIBRARY(qhull STATIC ${QHULL_LIB_SRCS})

IF(UNIX)
    TARGET_LINK_LIBRARIES(qhull m)
ENDIF(UNIX)

ADD_LIBRARY(Delaunay-${TulipVersion} SHARED Delaunay.cpp)
TARGET_LINK_LIBRARIES(Delaunay-${TulipVersion} ${LibTulipName} qhull)

ADD_LIBRARY(Voronoi-${TulipVersion} SHARED Voronoi.cpp)
TARGET_LINK_LIBRARIES(Voronoi-${TulipVersion} ${LibTulipName})

INSTALL(TARGETS Delaunay-${TulipVersion} DESTINATION ${TulipPluginsInstallDir})
INSTALL(TARGETS Voronoi-${TulipVersion} DESTINATION ${TulipPluginsInstallDir})
