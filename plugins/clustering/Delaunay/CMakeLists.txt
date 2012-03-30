INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TulipInclude} ${TulipBuildInclude} poly2tri poly2tri/sweep poly2tri/common)

set(PLUGIN_NAME delaunay)
FILE(GLOB LIB_SRCS "*.cpp")

SET(LIB_SRCS ${LIB_SRCS}
  poly2tri/common/shapes.cc
  poly2tri/sweep/advancing_front.cc
  poly2tri/sweep/cdt.cc
  poly2tri/sweep/sweep.cc
  poly2tri/sweep/sweep_context.cc
)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32")
ELSE(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLINUX")
ENDIF(WIN32)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DANSI_DECLARATORS -DTRILIBRARY -DNO_TIMER")

ADD_LIBRARY(${PLUGIN_NAME}-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TulipVersion} ${LibTulipCoreName} ${QT_QTCORE_LIBRARY})

INSTALL(TARGETS ${PLUGIN_NAME}-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ${PLUGIN_NAME})
