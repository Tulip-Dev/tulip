IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDLL_TULIP_PYTHON")
ENDIF(WIN32)

IF(MINGW AND I64)
  ADD_DEFINITIONS(-DMS_WIN64)
ENDIF(MINGW AND I64)

# VS2013 and beyond implement C99 math functions.  Of interest to Python is the function round().
# Python conditionally provides its own implementation of round() based on whether or not HAVE_ROUND is defined, but in no case is HAVE_ROUND ever defined.
# This leads to a huge spew of warnings when compiling with VS2013, and presumably it also leads to undefined behavior.
# So define HAVE_ROUND for that compiler.
IF(MSVC12)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_ROUND")
ENDIF(MSVC12)

ADD_SUBDIRECTORY(api)
ADD_SUBDIRECTORY(bindings)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(modules)
ADD_SUBDIRECTORY(src)
