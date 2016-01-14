SET(LIB_SRCS
    CrashHandling.cpp
    StackWalker.cpp)

IF(NOT MSVC)

  IF(LINUX OR MINGW)

    IF(MINGW)

      # ensure correct bfd header file is selected with MSYS2
      SET(CMAKE_INCLUDE_PATH ${MINGW_INCLUDE_PATH}/binutils ${CMAKE_INCLUDE_PATH})
      # ensure correct binutils libraries are selected with MSYS2
      SET(CMAKE_LIBRARY_PATH ${MINGW_LIB_PATH}/binutils ${CMAKE_LIBRARY_PATH})
      # ensure correct iberty header file is selected with MSYS2
      SET(CMAKE_INCLUDE_PATH ${MINGW_INCLUDE_PATH}/libiberty ${CMAKE_INCLUDE_PATH})

    ENDIF(MINGW)

    FIND_PACKAGE(Bfd)
    FIND_PACKAGE(Iberty)

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

ENDIF(NOT MSVC)
	
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

IF(UNIX)

  SET(LIB_SRCS ${LIB_SRCS} UnixSignalInterposer)

ENDIF(UNIX)

ADD_LIBRARY(crash_handling STATIC ${LIB_SRCS})

TARGET_LINK_LIBRARIES(crash_handling ${CMAKE_DL_LIBS})

IF(BFD_FOUND)

  TARGET_LINK_LIBRARIES(crash_handling ${BFD_LIBRARY})

ENDIF(BFD_FOUND)

IF(IBERTY_LIBRARY)

  TARGET_LINK_LIBRARIES(crash_handling ${IBERTY_LIBRARY})

ENDIF(IBERTY_LIBRARY)

IF(MINGW)

  TARGET_LINK_LIBRARIES(crash_handling imagehlp)

  IF(BFD_FOUND)

    # MinGW build environments may differ about the need to link against the intl library
    IF(INTL_LIBRARY)

      TARGET_LINK_LIBRARIES(crash_handling ${INTL_LIBRARY})

    ENDIF(INTL_LIBRARY)

  ENDIF(BFD_FOUND)

ELSEIF(MSVC)

  TARGET_LINK_LIBRARIES(crash_handling dbghelp)

ENDIF(MINGW)
