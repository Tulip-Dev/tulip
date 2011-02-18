IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

SET(TULIPOGL_SIP_FILES
    Camera.sip
    GlGraphInputData.sip
    GlLayer.sip
    GlScene.sip
    GlTools.sip
    Glyph.sip
    Module.sip
)


# Generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${SIP_EXE} -b ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.sbf TULIPOGL_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z]*\\.cpp" TULIPOGL_PYTHON_BINDINGS_SRC ${TULIPOGL_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${TULIPOGL_PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -o -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python Bindings for libtulip-ogl"
                   DEPENDS ${TULIPOGL_SIP_FILES})
           
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -DSIP_STATIC_MODULE")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -DSIP_STATIC_MODULE")

IF(UNIX)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${SIP_INCLUDE_DIR} ${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude})

ADD_LIBRARY(${LibTulipOglPythonName} STATIC ${TULIPOGL_PYTHON_BINDINGS_SRC})

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
