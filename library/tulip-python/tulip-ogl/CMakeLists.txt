IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

SET(TULIPOGL_SIP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlGraphInputData.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlLayer.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlScene.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Glyph.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlSimpleEntity.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlComposite.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlSceneVisitor.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlSceneObserver.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlAbstractPolygon.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlPolygon.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlRegularPolygon.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlComplexPolygon.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlCircle.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GlRect.sip
    CACHE INTERNAL ""
)

# Generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${SIP_EXE} -b ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.sbf TULIPOGL_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z_]*\\.cpp" TULIPOGL_PYTHON_BINDINGS_SRC ${TULIPOGL_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${TULIPOGL_PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -o -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python Bindings for libtulip-ogl"
                   DEPENDS ${TULIPOGL_SIP_FILES} ${TULIP_SIP_FILES} ${STL_SIP_FILES})
           
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w ")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w ")

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${LIBXML2_INCLUDE_DIR})

ADD_LIBRARY(${LibTulipOglPythonName} SHARED ${TULIPOGL_PYTHON_BINDINGS_SRC})

SET_TARGET_PROPERTIES(${LibTulipOglPythonName} 
		      PROPERTIES OUTPUT_NAME tulipogl PREFIX "")

IF(WIN32)
SET_TARGET_PROPERTIES(${LibTulipOglPythonName} 
		      PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)		     
SET_TARGET_PROPERTIES(${LibTulipOglPythonName} 
		      PROPERTIES SUFFIX ".so") 
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${LibTulipOglPythonName} ${LibTulipName} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(${LibTulipOglPythonName} ${PYTHON_LIBRARY})

ADD_CUSTOM_TARGET(touchTulipOglPythonCMakeLists ALL
                  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE}
                  COMMENT "touching CMakeLists.txt for Tulip OpenGL Python Bindings (force configure before next build to keep source files list updated)"
                  DEPENDS ${LibTulipOglPythonName})

INSTALL(TARGETS ${LibTulipOglPythonName}
        RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT pythonbindings
        LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT pythonbindings
        ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT pythonbindings)



ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
