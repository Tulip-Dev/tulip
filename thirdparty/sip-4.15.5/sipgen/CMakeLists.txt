CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(sip)

FIND_PACKAGE(PythonLibs REQUIRED)
FIND_PACKAGE(PythonInterp REQUIRED)

IF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

SET(sip_SRCS
    export.c
    extracts.c
    gencode.c
    heap.c
    lexer.c
    main.c
    parser.c
    transform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../siplib/sip.h
)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../siplib/sip.h
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../configure.py 
                                                -b ${CMAKE_CURRENT_BINARY_DIR}/../sipgen -e ${CMAKE_CURRENT_SOURCE_DIR}/../siplib
                                                -d ${CMAKE_CURRENT_BINARY_DIR}/../siplib
                     
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../siplib/sip.h.in
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ VERBATIM)

ADD_EXECUTABLE(sip ${sip_SRCS})

ENDIF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)

