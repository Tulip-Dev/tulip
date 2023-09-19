CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

PROJECT(sip)

IF(LINUX AND TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  # when building linux wheels in manylinux2014_x86_64 docker image
  # we need to build sip with a python different from the one used
  # for the current Tulip build
  SET(Python_FIND_STRATEGY LOCATION)
ENDIF()

FIND_PACKAGE(Python REQUIRED COMPONENTS Interpreter)

INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(sip_SRCS
    export.c
    extracts.c
    gencode.c
    heap.c
    lexer.c
    main.c
    parser.c
    transform.c
    type_hints.c
)

ADD_EXECUTABLE(sip ${sip_SRCS})

