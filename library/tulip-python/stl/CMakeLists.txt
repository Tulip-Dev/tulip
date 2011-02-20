IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

SET(STL_SIP_FILES
    list.sip
    map.sip
    Module.sip
    pair.sip
    set.sip
    stl.sip
    string.sip
    vector.sip
)

# Generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${SIP_EXE} -b ${CMAKE_CURRENT_BINARY_DIR}/stl.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/stl.sbf STL_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z]*\\.cpp" STL_PYTHON_BINDINGS_SRC ${STL_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${STL_PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -o -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip 
                   COMMENT "Generating Python Bindings for stl"
                   DEPENDS ${STL_SIP_FILES})
           
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -DSIP_STATIC_MODULE")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -DSIP_STATIC_MODULE")

IF(UNIX)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

# Force to use SIP and headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

ADD_LIBRARY(${LibStlPythonName} STATIC ${STL_PYTHON_BINDINGS_SRC})


ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
