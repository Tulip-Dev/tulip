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
  MESSAGE(STATUS "Qhull library was not found on the system, the version located in thirdparty will be compiled and used instead.")
  ADD_SUBDIRECTORY(libqhull)
ENDIF(NOT QHULL_FOUND)
IF(NOT YAJL_FOUND)
  MESSAGE(STATUS "Yajl library was not found on the system, the version located in thirdparty will be compiled and used instead.")
  ADD_SUBDIRECTORY(yajl)
ENDIF(NOT YAJL_FOUND)

ADD_SUBDIRECTORY(antlr)
ADD_SUBDIRECTORY(xdkbibtex)

IF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_SUBDIRECTORY(ftgl)
  ADD_SUBDIRECTORY(libtess2)
  IF(NOT QUAZIP_FOUND)
    MESSAGE(STATUS "Quazip library was not found on the system, the version located in thirdparty will be compiled and used instead.")
    ADD_SUBDIRECTORY(quazip)
  ENDIF(NOT QUAZIP_FOUND)
ENDIF(NOT TULIP_BUILD_CORE_ONLY AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
IF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
  IF(SIP_OK AND NOT SIP_FOUND)
    ADD_SUBDIRECTORY(sip-${SIP_VERSION_THIRDPARTY})
  ENDIF(SIP_OK AND NOT SIP_FOUND)
ENDIF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)

