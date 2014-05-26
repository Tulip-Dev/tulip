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
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../configure.py
                                                -b ${CMAKE_CURRENT_BINARY_DIR}/../sipgen
                                                -e ${CMAKE_CURRENT_SOURCE_DIR}/../siplib
                                                -d ${CMAKE_CURRENT_BINARY_DIR}/../siplib
                     
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sip.h.in ${CMAKE_CURRENT_SOURCE_DIR}/siplib.c.in
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ VERBATIM)

ADD_LIBRARY(${SIP_LIB} SHARED ${siplib_SRCS})

SET_TARGET_PROPERTIES(${SIP_LIB} 
		      PROPERTIES OUTPUT_NAME sip PREFIX "")

IF(WIN32)
SET_TARGET_PROPERTIES(${SIP_LIB} 
		      PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
SET_TARGET_PROPERTIES(${SIP_LIB} 
		      PROPERTIES SUFFIX ".so")		      
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${SIP_LIB} ${PYTHON_LIBRARY})

INSTALL(TARGETS ${SIP_LIB}
        RUNTIME DESTINATION ${TulipBinInstallDir}/python COMPONENT ${SIP_LIB}
        LIBRARY DESTINATION ${TulipLibInstallDir}/python COMPONENT ${SIP_LIB}
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python COMPONENT ${SIP_LIB})
	
ENDIF (PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)   					
