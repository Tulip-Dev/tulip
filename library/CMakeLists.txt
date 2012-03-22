ADD_SUBDIRECTORY(tulip-core)
ADD_SUBDIRECTORY(tulip-ogl)
ADD_SUBDIRECTORY(tulip-gui)
ADD_SUBDIRECTORY(tulip-ogdf)
#ADD_SUBDIRECTORY(tulip3-compat)
ADD_SUBDIRECTORY(tulip-python)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

include(FindPythonInterp)
include(FindPythonLibs  )

include_directories(${PYTHON_INCLUDE_DIR} tulip-core/include ${TulipCoreBuildInclude})

add_library(TulipSwigPython SHARED SwigHeader_wrap.cxx)
target_link_libraries(TulipSwigPython ${LibTulipCoreName})
