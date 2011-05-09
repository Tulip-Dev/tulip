CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

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
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

ADD_EXECUTABLE(sip ${sip_SRCS})


