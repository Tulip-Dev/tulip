SET(LIB_SRCS
    CrashHandling.cpp
    StackWalker.cpp)

IF(NOT MSVC)

UNSET(HAVE_BFD CACHE)
IF(LINUX OR MINGW)
INCLUDE(CheckIncludeFile)
# workaround for latest versions of GNU binutils
SET(CMAKE_REQUIRED_DEFINITIONS "-DPACKAGE -DPACKAGE_VERSION")
# workaround for mingw 4.8
IF(MINGW)
GET_FILENAME_COMPONENT(BFD_INCLUDE_PATH ${CMAKE_C_COMPILER} PATH)
SET(BFD_INCLUDE_PATH "${BFD_INCLUDE_PATH}/../include")
SET(CMAKE_REQUIRED_INCLUDES ${BFD_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${BFD_INCLUDE_PATH})
ENDIF(MINGW)
CHECK_INCLUDE_FILE(bfd.h HAVE_BFD)

IF(HAVE_BFD)
SET(LIB_SRCS ${LIB_SRCS} BfdWrapper.cpp)
ADD_DEFINITIONS(-DHAVE_BFD)
# workaround for latest versions of GNU binutils
ADD_DEFINITIONS(-DPACKAGE -DPACKAGE_VERSION)
ELSE()
MESSAGE("Install GNU binutils development package to get a detailed stack trace when Tulip crashes.")	
ENDIF()
ENDIF(LINUX OR MINGW)

ENDIF(NOT MSVC)
	
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

IF(UNIX)
	SET(LIB_SRCS ${LIB_SRCS} UnixSignalInterposer)
ENDIF(UNIX)

ADD_LIBRARY(crash_handling STATIC ${LIB_SRCS})

TARGET_LINK_LIBRARIES(crash_handling ${CMAKE_DL_LIBS})

IF(HAVE_BFD)
    TARGET_LINK_LIBRARIES(crash_handling bfd)
ENDIF(HAVE_BFD)

IF(MINGW)
  TARGET_LINK_LIBRARIES(crash_handling imagehlp)

  IF(HAVE_BFD)
    TARGET_LINK_LIBRARIES(crash_handling iberty)
    # MinGW build environments may differ about the need to link against the intl library
    IF(LIBINTL_FOUND)
        TARGET_LINK_LIBRARIES(crash_handling intl)
    ENDIF(LIBINTL_FOUND)
  ENDIF(HAVE_BFD)

ELSEIF(MSVC)
  TARGET_LINK_LIBRARIES(crash_handling dbghelp)
ENDIF(MINGW)
