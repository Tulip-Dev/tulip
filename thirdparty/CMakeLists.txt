DISABLE_COMPILER_WARNINGS()

ADD_CORE_FILES(CMakeLists.txt
               utf8-cpp/source/utf8.h
               utf8-cpp/source/utf8/checked.h
               utf8-cpp/source/utf8/core.h
               utf8-cpp/source/utf8/unchecked.h
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
  IF(NOT QUAZIP_FOUND)
    IF (TULIP_USE_THIRDPARTY_STATIC_LIBS)
      MESSAGE(STATUS "Quazip static library located in thirdparty will be used.")
    ELSE()
      MESSAGE(STATUS "Quazip library was not found on the system, the static version located in thirdparty will be used.")
    ENDIF()
    ADD_SUBDIRECTORY(quazip)
  ENDIF(NOT QUAZIP_FOUND)
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TRAVIS_BUILD_THIRDPARTY_ONLY)
IF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
  IF(SIP_OK AND NOT SIP_FOUND)
    ADD_SUBDIRECTORY(sip-${SIP_VERSION_THIRDPARTY})
  ENDIF(SIP_OK AND NOT SIP_FOUND)
ENDIF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)

