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
ADD_SUBDIRECTORY(antlr)
ADD_SUBDIRECTORY(xdkbibtex)

IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
  ADD_SUBDIRECTORY(ftgl)
  ADD_SUBDIRECTORY(libtess2)
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
