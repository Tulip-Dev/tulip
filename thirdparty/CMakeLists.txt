TULIP_DISABLE_COMPILER_WARNINGS()

ADD_CORE_FILES(CMakeLists.txt
               utf8-cpp/source/utf8.h
               utf8-cpp/source/utf8/checked.h
               utf8-cpp/source/utf8/core.h
               utf8-cpp/source/utf8/unchecked.h
               utf8-cpp/source/utf8/cpp11.h
               utf8-cpp/source/utf8/cpp17.h
)

ADD_SUBDIRECTORY(gzstream)
ADD_SUBDIRECTORY(OGDF)

IF(NOT QHULL_FOUND)
  IF (TULIP_USE_THIRDPARTY_STATIC_LIBS)
    MESSAGE(STATUS "Qhull static library located in thirdparty will be used.")
  ELSE()
    MESSAGE(STATUS "Qhull library was not found on the system, the static version located in thirdparty will be used.")
  ENDIF()
  ADD_SUBDIRECTORY(libqhull)
ENDIF(NOT QHULL_FOUND)
ADD_SUBDIRECTORY(yajl)

ADD_SUBDIRECTORY(antlr)
ADD_SUBDIRECTORY(xdkbibtex)

IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
  ADD_SUBDIRECTORY(ftgl)
  ADD_SUBDIRECTORY(libtess2)
  IF (NOT LIBZIP_FOUND)
    MESSAGE(STATUS "libzip static library located in third party will be used")
    ADD_SUBDIRECTORY(libzip)
  ENDIF(NOT LIBZIP_FOUND)
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
