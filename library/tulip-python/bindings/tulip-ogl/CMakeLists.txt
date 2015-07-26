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

SET(TULIP_OGL_PYTHON_BINDINGS_SRC
    sip_tulipoglpart0.cpp
    sip_tulipoglpart1.cpp
    sip_tulipoglpart2.cpp
)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_OGL_PYTHON_BINDINGS_SRC}
  COMMAND ${SIP_EXE} -o -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -c ${CMAKE_CURRENT_BINARY_DIR} -j3 ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating Python Bindings for libtulip-ogl"
  DEPENDS ${TULIP_OGL_SIP_FILES} ${TULIP_SIP_FILES} ${STL_SIP_FILES} VERBATIM)
           
DISABLE_COMPILER_WARNINGS()

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude})

ADD_LIBRARY(${LibTulipOGLPythonBindingsName} SHARED ${TULIP_OGL_PYTHON_BINDINGS_SRC})

SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TULIPOGL_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TULIPOGL_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TULIPOGL_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TULIPOGL_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TULIPOGL_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TULIPOGL_PYTHON_NATIVE_FOLDER})

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tulipogl_module/tulipogl/)

IF(NOT SYSTEM_SIP)
ADD_DEPENDENCIES(${LibTulipOGLPythonBindingsName} ${SIP_LIB})
ENDIF(NOT SYSTEM_SIP)

SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES OUTPUT_NAME tulipogl PREFIX "_")

IF(WIN32)
  SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
  SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES SUFFIX ".so")
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${LibTulipOGLPythonBindingsName} ${LibTulipCoreName} ${LibTulipOglName})
# When building Python wheel for MacOS, don't link the C extension module with the Python library
# and use dynamic lookup for retrieving its symbols.
# That way, we can produce a C extension module that can be imported through the Python interpreter
# provided by Apple with the System and the one provided by Python.org
IF(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)
  SET_TARGET_PROPERTIES(${LibTulipOGLPythonBindingsName} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ELSE(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)
  TARGET_LINK_LIBRARIES(${LibTulipOGLPythonBindingsName} ${PYTHON_LIBRARY})
ENDIF(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)

IF(NOT PYTHON_DIST_PACKAGE)
INSTALL(TARGETS ${LibTulipOGLPythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/python/tulipogl/native COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python/tulipogl/native COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python/tulipogl/native COMPONENT tulip_python)
IF(WIN32)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipBinInstallDir}/python/tulipogl COMPONENT tulip_python)
ELSE(WIN32)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipLibInstallDir}/python/tulipogl COMPONENT tulip_python)
ENDIF(WIN32)
ELSE(NOT PYTHON_DIST_PACKAGE)
INSTALL(TARGETS ${LibTulipOGLPythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/../lib/python${PYTHON_VERSION}/dist-packages/tulipogl/native COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulipogl/native COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulipogl/native COMPONENT tulip_python)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulipogl COMPONENT tulip_python)
ENDIF(NOT PYTHON_DIST_PACKAGE)

ADD_CUSTOM_COMMAND(
  TARGET ${LibTulipOGLPythonBindingsName}
  POST_BUILD
  COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -a ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating API file for Tulip OpenGL Python bindings"
  VERBATIM)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulipogl.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulipogl.api;sipAPI_tulipogl.h;tulipogl_module")

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
