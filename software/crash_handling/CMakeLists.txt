SET(LIB_SRCS
    CrashHandling.cpp
    StackWalker.cpp)

IF(NOT MSVC AND NOT BSD)

  IF(LINUX OR MINGW)

    IF(MINGW)

      # ensure correct bfd header file is selected with MSYS2
      SET(CMAKE_INCLUDE_PATH ${MINGW_INCLUDE_PATH}/binutils ${CMAKE_INCLUDE_PATH})
      # ensure correct binutils libraries are selected with MSYS2
      SET(CMAKE_LIBRARY_PATH ${MINGW_LIB_PATH}/binutils ${CMAKE_LIBRARY_PATH})
      # ensure correct iberty header file is selected with MSYS2
      SET(CMAKE_INCLUDE_PATH ${MINGW_INCLUDE_PATH}/libiberty ${CMAKE_INCLUDE_PATH})

      FIND_PACKAGE(Bfd)
      FIND_PACKAGE(Iberty)

      # Check if libintl is present in the MinGW build environment
      FIND_LIBRARY(INTL_LIBRARY intl PATHS ${MINGW_LIB_PATH} ${MINGW_LIB64_PATH} NO_DEFAULT_PATH)
      # the switch from binutils 2.39 to 2.40 added the two dependencies below
      # for libbfd.a which is needed by crashandling static library
      # Check if libsframe is present in the MinGW build environment
      FIND_LIBRARY(SFRAME_LIBRARY sframe PATHS ${MINGW_LIB_PATH} ${MINGW_LIB64_PATH} NO_DEFAULT_PATH)
      IF(SFRAME_LIBRARY)
	FIND_PACKAGE(Zstd)
	MARK_AS_ADVANCED(SFRAME_LIBRARY)
      ENDIF()
    ELSE(MINGW)
      FIND_LIBRARY(INTL_LIBRARY intl)
    ENDIF(MINGW)

    MARK_AS_ADVANCED(INTL_LIBRARY)

    IF(BFD_FOUND)

      SET(LIB_SRCS ${LIB_SRCS} BfdWrapper.cpp)
      ADD_DEFINITIONS(-DHAVE_BFD)
      # workaround for latest versions of GNU binutils
      ADD_DEFINITIONS(-DPACKAGE -DPACKAGE_VERSION)
      INCLUDE_DIRECTORIES(BEFORE ${BFD_INCLUDE_DIR})

    ELSE(BFD_FOUND)

      MESSAGE("Install GNU binutils development package to get a detailed stack trace when Tulip crashes.")

    ENDIF(BFD_FOUND)

  ENDIF(LINUX OR MINGW)

ENDIF(NOT MSVC AND NOT BSD)
	
INCLUDE_DIRECTORIES(BEFORE ${TulipCoreBuildInclude} ${TulipCoreInclude})

IF(UNIX)

  SET(LIB_SRCS ${LIB_SRCS} UnixSignalInterposer.cpp)

ENDIF(UNIX)

ADD_LIBRARY(crash_handling STATIC ${LIB_SRCS})

TARGET_LINK_LIBRARIES(crash_handling ${CMAKE_DL_LIBS})

IF(BFD_FOUND)

  TARGET_LINK_LIBRARIES(crash_handling ${BFD_LIBRARY})

  # some build environments requires the need to link against the intl library
  IF(INTL_LIBRARY)

    TARGET_LINK_LIBRARIES(crash_handling ${INTL_LIBRARY})

  ENDIF(INTL_LIBRARY)

  # sframe, zlib and zstd libraries may be needed
  # since the switch from binutils 2.39 to 2.40
  IF(SFRAME_LIBRARY)
    TARGET_LINK_LIBRARIES(crash_handling ${SFRAME_LIBRARY} z zstd)
  ENDIF(SFRAME_LIBRARY)

ENDIF(BFD_FOUND)

IF(IBERTY_LIBRARY)

  TARGET_LINK_LIBRARIES(crash_handling ${IBERTY_LIBRARY})

ENDIF(IBERTY_LIBRARY)

IF(MINGW)

  TARGET_LINK_LIBRARIES(crash_handling imagehlp)

ELSEIF(MSVC)

  TARGET_LINK_LIBRARIES(crash_handling dbghelp)

ENDIF(MINGW)
