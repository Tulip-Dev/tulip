SET(LIB_SRCS
    CrashHandling.cpp
    StackWalker.cpp)

IF(NOT MSVC)

UNSET(HAVE_BFD CACHE)
IF(LINUX OR MINGW)	
INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES(bfd.h HAVE_BFD)

IF(HAVE_BFD)
SET(LIB_SRCS ${LIB_SRCS} BfdWrapper.cpp)
ADD_DEFINITIONS(-DHAVE_BFD)
ELSE()
MESSAGE("Install GNU binutils development package to get a detailed stack trace when Tulip crashes.")	
ENDIF()
ENDIF(LINUX OR MINGW)

ENDIF(NOT MSVC)
	
INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude})

IF(UNIX)
	SET(LIB_SRCS ${LIB_SRCS} UnixSignalInterposer)
ENDIF(UNIX)

ADD_LIBRARY(crash_handling STATIC ${LIB_SRCS})

IF(UNIX)
	TARGET_LINK_LIBRARIES(crash_handling dl)
ENDIF(UNIX)

IF(HAVE_BFD)
    TARGET_LINK_LIBRARIES(crash_handling bfd)
ENDIF(HAVE_BFD)

IF(MINGW)
  TARGET_LINK_LIBRARIES(crash_handling imagehlp)

  IF(HAVE_BFD)
    TARGET_LINK_LIBRARIES(crash_handling iberty)
	# don't need to link against libintl when using MinGW 4.4
	IF(GCXX_VERSION VERSION_GREATER 4.5 OR GCXX_VERSION VERSION_EQUAL 4.5)
		TARGET_LINK_LIBRARIES(crash_handling intl)
	ENDIF()	
  ENDIF(HAVE_BFD)

ELSEIF(MSVC)
  TARGET_LINK_LIBRARIES(crash_handling dbghelp)
ENDIF(MINGW)
