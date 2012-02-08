# - Try to find GLEW
# Once done this will define
#  
#  GLEW_FOUND        - system has GLEW
#  GLEW_INCLUDE_DIR  - the GLEW include directory
#  GLEW_LIBRARY_DIR  - where the libraries are
#  GLEW_LIBRARY      - Link these to use GLEW
#   

IF (GLEW_INCLUDE_DIR)
  # Already in cache, be silent
  SET(GLEW_FOUND TRUE)
  SET(GLEW_FIND_QUIETLY TRUE)

ELSE (GLEW_INCLUDE_DIR)

  IF( WIN32 )
    IF( MSVC80 )
	SET( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ 8/VC" )
    ENDIF( MSVC80 )
    IF( MSVC71 )
	SET( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ .NET\ 2003/Vc7" )
    ENDIF( MSVC71 )
    FIND_PATH( GLEW_INCLUDE_DIR glew.h wglew.h
	       PATHS c:/glew/include ${COMPILER_PATH}/PlatformSDK/Include
               PATH_SUFFIXES gl GL )
    SET( GLEW_NAMES glew32 glew)
    FIND_LIBRARY( GLEW_LIBRARY
		  NAMES ${GLEW_NAMES}
		  PATHS c:/glew/lib ${COMPILER_PATH}/PlatformSDK/Lib )
  ELSE( WIN32 )
    FIND_PATH( GLEW_INCLUDE_DIR glew.h wglew.h
	       PATHS /usr/local/include /usr/include
               PATH_SUFFIXES gl GL )
    SET( GLEW_NAMES glew GLEW )
    FIND_LIBRARY( GLEW_LIBRARY
                  NAMES ${GLEW_NAMES}
                  PATHS /usr/lib /usr/local/lib )
  ENDIF( WIN32 )

  GET_FILENAME_COMPONENT( GLEW_LIBRARY_DIR ${GLEW_LIBRARY} PATH )	
  IF(GLEW_INCLUDE_DIR)
    # Remove the GL or gl subdirectory, because you should say #include <GL/foo.h>, not #include <foo.h>
    SET(GLEW_INCLUDE_DIR ${GLEW_INCLUDE_DIR}/.. CACHE PATH "" FORCE)
  ENDIF()

  IF (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
    SET(GLEW_FOUND TRUE)
    #SET( GLEW_LIBRARY_DIR ${GLEW_LIBRARY} )
  ELSE (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
    SET( GLEW_FOUND FALSE )
    SET( GLEW_LIBRARY_DIR )
  ENDIF (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)

  MARK_AS_ADVANCED(GLEW_LIBRARY
		   GLEW_INCLUDE_DIR)

ENDIF(GLEW_INCLUDE_DIR)