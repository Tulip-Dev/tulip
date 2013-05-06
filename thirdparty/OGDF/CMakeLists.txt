include_directories(${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB_RECURSE OGDF_SOURCES "src/*.cpp")

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOGDF_DLL -DOGDF_INSTALL")
ENDIF(WIN32)

MACRO(CAR var)
  SET(${var} ${ARGV1})
ENDMACRO(CAR)

MACRO(CDR var junk)
  SET(${var} ${ARGN})
ENDMACRO(CDR)

MACRO(LIST_INDEX var index)
  SET(list . ${ARGN})
  FOREACH(i RANGE 1 ${index})
    CDR(list ${list})
  ENDFOREACH(i)
  CAR(${var} ${list})
ENDMACRO(LIST_INDEX)

# On Ubuntu 13.04, there is a bug when compiling OGDF with GCC 4.7.3 (at least on 64bits system)
# Turning on the optimizations makes Tulip crash when it try to load libOGDF.so
# So turn it off for that particular version of GCC
IF(LINUX AND (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ARG1}" MATCHES ".*[g][+][+].*"))
    EXECUTE_PROCESS(COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} --version
                    OUTPUT_VARIABLE GCXX_V)

    STRING(REGEX MATCHALL "[0-9]*\\.[0-9]*\\.[0-9]*" GCXX_V ${GCXX_V})
    LIST_INDEX(GCXX_VERSION 1 ${GCXX_V})

    IF(GCXX_VERSION VERSION_EQUAL 4.7.3)
        SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0")
    ENDIF()
ENDIF()

ADD_LIBRARY(${OGDFLibrary} SHARED ${OGDF_SOURCES} )
IF(WIN32)
  TARGET_LINK_LIBRARIES(${OGDFLibrary} psapi)
ENDIF(WIN32)

INSTALL(TARGETS ${OGDFLibrary}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT ogdf
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT ogdf
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT ogdf)
