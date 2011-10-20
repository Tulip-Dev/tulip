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
	
INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude})

ADD_LIBRARY(crash_handling STATIC ${LIB_SRCS})

IF(LINUX)
  IF(HAVE_BFD)
    TARGET_LINK_LIBRARIES(crash_handling bfd)
  ENDIF(HAVE_BFD)

ELSEIF(MINGW)
  TARGET_LINK_LIBRARIES(crash_handling imagehlp)

  IF(HAVE_BFD)
    TARGET_LINK_LIBRARIES(crash_handling bfd intl iberty)
  ENDIF(HAVE_BFD)

ELSEIF(MSVC)
  TARGET_LINK_LIBRARIES(crash_handling dbghelp)
ENDIF(LINUX)
