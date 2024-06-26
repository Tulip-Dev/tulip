ADD_CORE_FILES(.)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/tulip.sip
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
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyProxy.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SizePropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Size.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/StringCollection.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TlpTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TriconnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TulipViewSettings.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Vector.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/WithDependency.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/WithParameter.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/../stl/stl.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/../stl/std_vector.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/../stl/std_string.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/../stl/std_set.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/../stl/std_pair.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/../stl/std_ostream.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/../stl/std_map.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/../stl/std_list.sip

    CACHE INTERNAL ""
)

SET(TULIP_PYTHON_BINDINGS_SRC
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart0.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart1.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart2.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart3.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart4.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart5.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart6.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart7.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart8.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart9.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart10.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip/sip_tulippart11.cpp
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml.in ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml @ONLY)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_PYTHON_BINDINGS_SRC}
    COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/generatePropertiesBindings.cmake
    COMMAND ${SIP_BUILD} --verbose --protected-is-public --concatenate 12 --api-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Python Bindings and API file for libtulip"
    DEPENDS ${TULIP_SIP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../../include/tulip/PythonCppTypesConverter.h VERBATIM)

TULIP_DISABLE_COMPILER_WARNINGS_PYTHON()

SET(TULIP_PYTHON_BINDINGS_SRC
     ${TULIP_PYTHON_BINDINGS_SRC}
     ${CMAKE_CURRENT_SOURCE_DIR}/../../src/PythonCppTypesConverter.cpp)

Python_add_library(${LibTulipCorePythonBindingsName} MODULE WITH_SOABI ${TULIP_PYTHON_BINDINGS_SRC})
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel ${LibTulipCorePythonBindingsName})
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

IF(WIN32)
  TARGET_COMPILE_DEFINITIONS(${LibTulipCorePythonBindingsName} PUBLIC "DLL_TULIP_PYTHON")
ENDIF(WIN32)
TARGET_COMPILE_DEFINITIONS(${LibTulipCorePythonBindingsName} PUBLIC "protected=public")
TARGET_COMPILE_DEFINITIONS(${LibTulipCorePythonBindingsName} PUBLIC "SIP_PROTECTED_IS_PUBLIC")
TARGET_COMPILE_DEFINITIONS(${LibTulipCorePythonBindingsName} PUBLIC "DYNAMIC_ANNOTATIONS_ENABLED=1")
TARGET_INCLUDE_DIRECTORIES(${LibTulipCorePythonBindingsName}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/build/ ${Python_INCLUDE_DIRS}
    PUBLIC ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipPythonInclude})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TULIP_PYTHON_NATIVE_FOLDER})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in ${TULIP_PYTHON_FOLDER}/__init__.py @ONLY)

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  CONFIGURE_FILE(packaging/pyproject.toml.in ${TULIP_PYTHON_FOLDER}/../pyproject.toml)
  FILE(COPY packaging/setup.py DESTINATION ${TULIP_PYTHON_FOLDER}/../)
  FILE(COPY packaging/README.rst DESTINATION ${TULIP_PYTHON_FOLDER}/../)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName}
                      PROPERTIES OUTPUT_NAME tulip PREFIX "_")

# On MacOS, add the paths of dependencies dylibs in install rpaths of the _tulip.so binary
# That way, the tulip module can be imported in a classical Python shell without having to
# modify the content of the DYLD_LIBRARY_PATH environment variable

FILE(RELATIVE_PATH TulipLibsInstallRelPath ${TulipPythonModulesInstallDir}/tulip/native ${CMAKE_INSTALL_PREFIX}/${TulipLibInstallDir})

IF(APPLE AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipCorePythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS};@loader_path/${TulipLibsInstallRelPath};@loader_path/../../../../Frameworks")
ELSEIF(LINUX AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipCorePythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS}:$ORIGIN/${TulipLibsInstallRelPath}")
ENDIF(APPLE AND NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

TARGET_LINK_LIBRARIES(${LibTulipCorePythonBindingsName} PUBLIC ${LibTulipCoreName})
# When building Python wheel for MacOS, don't link the C extension module with the Python library
# and use dynamic lookup for retrieving its symbols.
# That way, we can produce a C extension module that can be imported through the Python interpreter
# provided by Apple with the System and the one provided by Python.org
IF(APPLE AND TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ENDIF(APPLE AND TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

TULIP_INSTALL_PYTHON_FILES(tulip ${TULIP_PYTHON_FOLDER}/__init__.py)
TULIP_INSTALL_PYTHON_FILES(tulip/native ${LibTulipCorePythonBindingsName})


IF(NOT TULIP_BUILD_CORE_ONLY)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/build/tulip.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_CLEAN_FILES "build;tulip_module;BooleanProperty.sip;ColorProperty.sip;DoubleProperty.sip;IntegerProperty.sip;LayoutProperty.sip;SizeProperty.sip;StringProperty.sip")
