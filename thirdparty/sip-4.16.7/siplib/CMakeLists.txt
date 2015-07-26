IF (PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)   					

IF(MINGW AND I64)
  ADD_DEFINITIONS(-DMS_WIN64)
ENDIF(MINGW AND I64)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

SET(siplib_SRCS
    apiversions.c
    array.c
    bool.cpp
    descriptors.c
    objmap.c
    qtlib.c
    siplib.c
    threads.c
    voidptr.c
    sip.h
)

DISABLE_COMPILER_WARNINGS()

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/sip.h ${CMAKE_CURRENT_SOURCE_DIR}/siplib.c
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../configure.py --sip-module ${SIP_LIB}
                                                -b ${CMAKE_CURRENT_BINARY_DIR}/../sipgen
                                                -e ${CMAKE_CURRENT_SOURCE_DIR}/../siplib
                                                -d ${CMAKE_CURRENT_BINARY_DIR}/../siplib

                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sip.h.in ${CMAKE_CURRENT_SOURCE_DIR}/siplib.c.in
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ VERBATIM)

ADD_LIBRARY(${SIP_LIB} SHARED ${siplib_SRCS})

SET_TARGET_PROPERTIES(${SIP_LIB} 
                      PROPERTIES OUTPUT_NAME ${SIP_LIB} PREFIX "")

IF(WIN32)
SET_TARGET_PROPERTIES(${SIP_LIB} 
		      PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
SET_TARGET_PROPERTIES(${SIP_LIB} 
		      PROPERTIES SUFFIX ".so")		      
ENDIF(WIN32)

# When building Python wheel for MacOS, don't link the C extension module with the Python library
# and use dynamic lookup for retrieving its symbols.
# That way, we can produce a C extension module that can be imported through the Python interpreter
# provided by Apple with the System and the one provided by Python.org
IF(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ELSE(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)
TARGET_LINK_LIBRARIES(${SIP_LIB} ${PYTHON_LIBRARY})
ENDIF(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)

INSTALL(TARGETS ${SIP_LIB}
        RUNTIME DESTINATION ${TulipBinInstallDir}/python COMPONENT ${SIP_LIB}
        LIBRARY DESTINATION ${TulipLibInstallDir}/python COMPONENT ${SIP_LIB}
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python COMPONENT ${SIP_LIB})

IF(ACTIVATE_PYTHON_WHEELS_TARGETS)
COPY_TARGET_LIBRARY_POST_BUILD(${SIP_LIB} ${TULIP_PYTHON_NATIVE_FOLDER})
ENDIF(ACTIVATE_PYTHON_WHEELS_TARGETS)

ENDIF (PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)   					
