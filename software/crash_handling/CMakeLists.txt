SET(LIB_SRCS
    CrashHandling.cpp
    StackWalker.cpp)

IF(NOT MSVC)	
INCLUDE (CheckIncludeFiles)
UNSET(HAVE_BFD CACHE)
CHECK_INCLUDE_FILES(bfd.h HAVE_BFD)

IF(HAVE_BFD)
SET(LIB_SRCS ${LIB_SRCS} BfdWrapper.cpp)
ADD_DEFINITIONS(-DHAVE_BFD)
ELSE()
MESSAGE("Install GNU binutils development package to get a detailed stack trace when Tulip crashes.")	
ENDIF()

ENDIF()
	
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/google-breakpad/src)
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

ADD_LIBRARY(crash_handling STATIC ${LIB_SRCS})
