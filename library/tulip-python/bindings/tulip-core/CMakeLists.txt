IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDLL_TULIP_PYTHON")
ENDIF(WIN32)

SET(TULIP_SIP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/AcyclicTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractProperty.sip.in
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BiconnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BooleanPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundingBox.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorScale.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Color.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ConvexHull.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Coord.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DataSet.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Delaunay.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DoublePropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DrawingTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Edge.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ExportModule.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Graph.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphEvent.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphProperty.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphMeasure.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ImportModule.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegerPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Iterator.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/LayoutPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/MapIterator.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Matrix.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Node.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/NumericProperty.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Observable.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/OuterPlanarTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PlanarityTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginContext.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginLoader.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginProgress.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyEvent.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyInterface.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SizePropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Size.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/StringCollection.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TlpTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TriconnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TulipFontAwesome.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TulipViewSettings.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Vector.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/WithDependency.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/WithParameter.sip
    CACHE INTERNAL ""
)

SET(TULIP_PYTHON_BINDINGS_SRC
    sip_tulippart0.cpp
    sip_tulippart1.cpp
    sip_tulippart2.cpp
    sip_tulippart3.cpp
    sip_tulippart4.cpp
    sip_tulippart5.cpp
    sip_tulippart6.cpp
    sip_tulippart7.cpp
)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_PYTHON_BINDINGS_SRC}
                   COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/generatePropertiesBindings.cmake
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} -j8 ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python Bindings for libtulip"
                   DEPENDS ${TULIP_SIP_FILES} ${STL_SIP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../../include/tulip/PythonCppTypesConverter.h VERBATIM)
           
DISABLE_COMPILER_WARNINGS()

# Force to use SIP and headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipPythonInclude})
 
SET(TULIP_PYTHON_BINDINGS_SRC
    ${TULIP_PYTHON_BINDINGS_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/PythonCppTypesConverter.cpp)

ADD_LIBRARY(${LibTulipCorePythonBindingsName} SHARED ${TULIP_PYTHON_BINDINGS_SRC})

SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TULIP_PYTHON_NATIVE_FOLDER})

ADD_CUSTOM_TARGET(copyTulipInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${TULIP_PYTHON_FOLDER}/__init__.py)
IF(ACTIVATE_PYTHON_WHEELS_TARGETS)
ADD_DEPENDENCIES(tulip-core-wheel copyTulipInitPy)
ENDIF(ACTIVATE_PYTHON_WHEELS_TARGETS)

IF(ACTIVATE_PYTHON_WHEELS_TARGETS)
CONFIGURE_FILE(packaging/setup.py.in ${TULIP_PYTHON_FOLDER}/../setup.py)
ADD_CUSTOM_TARGET(copyTulipPythonPackagingFiles ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/setup.cfg ${TULIP_PYTHON_FOLDER}/../setup.cfg
                                                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/Manifest.in ${TULIP_PYTHON_FOLDER}/../Manifest.in
                                                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/README.rst ${TULIP_PYTHON_FOLDER}/../README.rst
                                                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/DESCRIPTION.rst ${TULIP_PYTHON_FOLDER}/../DESCRIPTION.rst
)
ADD_DEPENDENCIES(tulip-core-wheel copyTulipPythonPackagingFiles)
ENDIF(ACTIVATE_PYTHON_WHEELS_TARGETS)

IF(NOT SYSTEM_SIP)
ADD_DEPENDENCIES(${LibTulipCorePythonBindingsName} ${SIP_LIB})
ENDIF(NOT SYSTEM_SIP)

SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName}
                      PROPERTIES OUTPUT_NAME tulip PREFIX "_")

IF(WIN32)
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName}
                      PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName}
                      PROPERTIES SUFFIX ".so")
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${LibTulipCorePythonBindingsName} ${LibTulipCoreName})
# When building Python wheel for MacOS, don't link the C extension module with the Python library
# and use dynamic lookup for retrieving its symbols.
# That way, we can produce a C extension module that can be imported through the Python interpreter
# provided by Apple with the System and the one provided by Python.org
IF(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)
  SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ELSE(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)
  TARGET_LINK_LIBRARIES(${LibTulipCorePythonBindingsName} ${PYTHON_LIBRARY})
ENDIF(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS)

IF(NOT PYTHON_DIST_PACKAGE)
INSTALL(TARGETS ${LibTulipCorePythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/python/tulip/native COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python/tulip/native COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python/tulip/native COMPONENT tulip_python)
IF(WIN32)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipBinInstallDir}/python/tulip COMPONENT tulip_python)
ELSE(WIN32)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipLibInstallDir}/python/tulip COMPONENT tulip_python)
ENDIF(WIN32)
ELSE(NOT PYTHON_DIST_PACKAGE)
INSTALL(TARGETS ${LibTulipCorePythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/../lib/python${PYTHON_VERSION}/dist-packages/tulip/native COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulip/native COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulip/native COMPONENT tulip_python)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulip COMPONENT tulip_python)
ENDIF(NOT PYTHON_DIST_PACKAGE)

ADD_CUSTOM_COMMAND(TARGET ${LibTulipCorePythonBindingsName}
                   POST_BUILD
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -a ${CMAKE_CURRENT_BINARY_DIR}/tulip.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating API file for Tulip Python bindings"
                   VERBATIM)
				   
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulip.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulip.api;sipAPI_tulip.h;tulip_module;BooleanProperty.sip;ColorProperty.sip;DoubleProperty.sip;IntegerProperty.sip;LayoutProperty.sip;SizeProperty.sip;StringProperty.sip")

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
