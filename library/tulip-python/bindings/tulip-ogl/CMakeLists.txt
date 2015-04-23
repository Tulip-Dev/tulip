IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

SET(TULIP_OGL_SIP_FILES
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
  ${CMAKE_CURRENT_SOURCE_DIR}/GlGraphRenderingParameters.sip
  ${CMAKE_CURRENT_SOURCE_DIR}/GlGraphComposite.sip
  ${CMAKE_CURRENT_SOURCE_DIR}/ParametricCurves.sip
  CACHE INTERNAL ""
)

# Generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -b ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.sbf TULIPOGL_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z_]*\\.cpp" TULIPOGL_PYTHON_BINDINGS_SRC ${TULIPOGL_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${TULIPOGL_PYTHON_BINDINGS_SRC}
  COMMAND ${SIP_EXE} -o -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating Python Bindings for libtulip-ogl"
  DEPENDS ${TULIP_OGL_SIP_FILES} ${TULIP_SIP_FILES} ${STL_SIP_FILES} VERBATIM)
           
DISABLE_COMPILER_WARNINGS()

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude})

ADD_LIBRARY(${LibTulipOGLPythonBindingsName} SHARED ${TULIPOGL_PYTHON_BINDINGS_SRC})

IF(NOT SYSTEM_SIP)
ADD_DEPENDENCIES(${LibTulipOGLPythonBindingsName} ${SIP_LIB})
ENDIF(NOT SYSTEM_SIP)

SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES OUTPUT_NAME tulipogl PREFIX "")

IF(WIN32)
  SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
  SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES SUFFIX ".so")
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${LibTulipOGLPythonBindingsName} ${LibTulipCoreName} ${LibTulipOglName} ${PYTHON_LIBRARY})

ADD_CUSTOM_TARGET(touchTulipOglPythonCMakeLists ALL
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE}
  COMMENT "touching CMakeLists.txt for Tulip OpenGL Python Bindings (force configure before next build to keep source files list updated)"
  DEPENDS ${LibTulipOGLPythonBindingsName} VERBATIM)

IF(NOT UBUNTU_PPA_BUILD)
INSTALL(TARGETS ${LibTulipOGLPythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/python COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python COMPONENT tulip_python)
ELSE()
INSTALL(TARGETS ${LibTulipOGLPythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/../lib/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python)
ENDIF()

ADD_CUSTOM_COMMAND(
  TARGET ${LibTulipOGLPythonBindingsName}
  POST_BUILD
  COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -a ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating QScintilla API file for Tulip OpenGL Python bindings"
  VERBATIM)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulipogl.sbf;tulipogl.api;sipAPItulipogl.h")

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
