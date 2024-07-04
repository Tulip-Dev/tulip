SET(LIB_SRCS
    CrashHandling.cpp
    StackWalker.cpp)

IF(LINUX OR MINGW)

  FIND_PACKAGE(Bfd)
  IF(BFD_FOUND)
    SET(LIB_SRCS ${LIB_SRCS} BfdWrapper.cpp)
  ELSE(BFD_FOUND)
    MESSAGE(WARNING "Install GNU binutils development package to get a detailed stack trace when Tulip crashes.")
  ENDIF(BFD_FOUND)

  FIND_PACKAGE(Iberty)

  IF(MINGW)
    # Check if libintl is present in the MinGW build environment
    FIND_PACKAGE(Intl)
    # the switch from binutils 2.39 to 2.40 added the two dependencies below
    # for libbfd.a which is needed by crashandling static library
    # Check if libsframe is present in the MinGW build environment
    # FIND_LIBRARY(SFRAME_LIBRARY sframe PATHS ${MINGW_LIB_PATH} ${MINGW_LIB64_PATH} REQUIRED)
    FIND_LIBRARY(SFRAME_LIBRARY sframe REQUIRED)
    FIND_PACKAGE(Zstd)
    MARK_AS_ADVANCED(SFRAME_LIBRARY)
  ENDIF(MINGW)

ENDIF(LINUX OR MINGW)

IF(UNIX)
  SET(LIB_SRCS ${LIB_SRCS} UnixSignalInterposer.cpp)
ENDIF(UNIX)

ADD_LIBRARY(crash_handling STATIC ${LIB_SRCS})

IF(BFD_FOUND)
  target_COMPILE_DEFINITIONS(crash_handling PUBLIC HAVE_BFD)
  # workaround for latest versions of GNU binutils
  target_COMPILE_DEFINITIONS(crash_handling PUBLIC PACKAGE)
  target_COMPILE_DEFINITIONS(crash_handling PUBLIC PACKAGE_VERSION)
  target_include_directories(crash_handling BEFORE PUBLIC ${BFD_INCLUDE_DIR})
ENDIF(BFD_FOUND)

target_INCLUDE_DIRECTORIES(crash_handling BEFORE PUBLIC ${TulipCoreBuildInclude} ${TulipCoreInclude})

TARGET_LINK_LIBRARIES(crash_handling ${CMAKE_DL_LIBS})

IF(BFD_FOUND)

  TARGET_LINK_LIBRARIES(crash_handling ${BFD_LIBRARY})

  # some build environments requires the need to link against the intl library
  IF(Intl_FOUND)
    TARGET_LINK_LIBRARIES(crash_handling ${Intl_LIBRARIES})
  ENDIF(Intl_FOUND)

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
