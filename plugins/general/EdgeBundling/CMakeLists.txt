INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})
SET(LIB_SRCS 
    BendsTools.cpp
    Dijkstra.cpp
    EdgeBundling.cpp
    OctreeBundle.cpp
    QuadTree.cpp
    SphereUtils.cpp)

FIND_PACKAGE(OpenMP QUIET)
IF(OPENMP_FOUND)
   SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS}")
   SET(OPENMP_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
   IF(WIN32)
          IF(MSVC)
                SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /openmp")
                SET(OPENMP_CXX_FLAGS "/openmp")
          ELSE()
                SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp ${CMAKE_THREAD_LIBS_INIT}")
                SET(OPENMP_LIBRARIES "-lgomp -lpthread")
          ENDIF()
   ENDIF()
ELSE(OPENMP_FOUND)
  IF(WIN32)
   STRING(COMPARE NOTEQUAL "${OpenMP_C_FLAGS}" "" OMP_CFLAGS)
   IF(OMP_CFLAGS)
      ## HACK: do not know why OpenMP_CXX_FLAGS
      ## is not set when running on window ?
      SET(CMAKE_CXX_FLAGS_RELEASES "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_C_FLAGS}")
      SET(OPENMP_CXX_FLAGS "${OpenMP_C_FLAGS}")
      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_C_FLAGS}")
      SET(OPENMP_LINKER_FLAGS "${OpenMP_C_FLAGS}")
      SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp -lpthread")
      SET(OPENMP_LIBRARIES "-lgomp -lpthread")
      SET(OPENMP_FOUND TRUE)
   ELSE(OMP_CFLAGS)
      MESSAGE("OpenMP not found: multithreaded code will not be used")
   ENDIF(OMP_CFLAGS)
  ELSE(WIN32)
    MESSAGE("OpenMP not found: multithreaded code will not be used")
  ENDIF(WIN32)
ENDIF(OPENMP_FOUND)

ADD_LIBRARY(EdgeBundling-${TulipVersion} SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(EdgeBundling-${TulipVersion} ${LibTulipCoreName})

INSTALL(TARGETS EdgeBundling-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT EdgeBundling)
