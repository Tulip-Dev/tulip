ADD_CORE_FILES(.)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDLL_TULIP_PYTHON")
ENDIF(WIN32)

# Visual Studio has a limit for string constants length
# so we define the sip feature NO_LIMIT_DOC in Module.sip
# to avoid the compiler error (fatal error C1091)
IF(MSVC)
  SET(SIP_FLAGS -x NO_LIMIT_DOC)
ENDIF()

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
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml.in ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml @ONLY)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_PYTHON_BINDINGS_SRC}
    COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/generatePropertiesBindings.cmake
    COMMAND ${SIP_BUILD} --verbose --protected-is-public --concatenate 8 --pep484-pyi --api-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Python Bindings and API file for libtulip"
    DEPENDS ${TULIP_SIP_FILES} ${STL_SIP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../../include/tulip/PythonCppTypesConverter.h VERBATIM)

TULIP_DISABLE_COMPILER_WARNINGS_PYTHON()

SET(TULIP_PYTHON_BINDINGS_SRC
     ${TULIP_PYTHON_BINDINGS_SRC}
     ${CMAKE_CURRENT_SOURCE_DIR}/../../src/PythonCppTypesConverter.cpp)

Python_add_library(${LibTulipCorePythonBindingsName} MODULE WITH_SOABI ${TULIP_PYTHON_BINDINGS_SRC})
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel ${LibTulipCorePythonBindingsName})
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

TARGET_COMPILE_DEFINITIONS(${LibTulipCorePythonBindingsName} PUBLIC "protected=public")
TARGET_COMPILE_DEFINITIONS(${LibTulipCorePythonBindingsName} PUBLIC "SIP_PROTECTED_IS_PUBLIC")
TARGET_COMPILE_DEFINITIONS(${LibTulipCorePythonBindingsName} PUBLIC "DYNAMIC_ANNOTATIONS_ENABLED=1")
TARGET_INCLUDE_DIRECTORIES(${LibTulipCorePythonBindingsName}
    PUBLIC ${SIP_INCLUDE_DIR} ${Python_INCLUDE_DIRS}
    PUBLIC ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipPythonInclude})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TULIP_PYTHON_NATIVE_FOLDER})

ADD_DEPENDENCIES(${LibTulipCorePythonBindingsName} ${LibStlPythonBindingsName})

ADD_CUSTOM_TARGET(copyTulipInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${TULIP_PYTHON_FOLDER}/__init__.py
    COMMENT "Copying tulip-core module __init__.py file"
    DEPENDS ${LibTulipCorePythonBindingsName}
    VERBATIM)

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel copyTulipInitPy)

  CONFIGURE_FILE(packaging/setup.cfg.in ${TULIP_PYTHON_FOLDER}/../setup.cfg)
  CONFIGURE_FILE(packaging/setup.py.in ${TULIP_PYTHON_FOLDER}/../setup.py)
  IF(TULIP_GENERATE_TESTPYPI_WHEEL)
    SET(TULIP_PYTHON_WHEEL_VERSION ${TULIP_PYTHON_TEST_WHEEL_VERSION})
    CONFIGURE_FILE(packaging/setup.py.in ${TULIP_PYTHON_FOLDER}/../setuptest.py)
  ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)

  ADD_CUSTOM_TARGET(copyTulipPythonPackagingFiles ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/MANIFEST.in ${TULIP_PYTHON_FOLDER}/../MANIFEST.in
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/README.rst ${TULIP_PYTHON_FOLDER}/../README.rst
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/copyTulipCoreDllDependencies.cmake ${TULIP_PYTHON_FOLDER}/../copyTulipCoreDllDependencies.cmake
                    DEPENDS ${TULIP_PYTHON_FOLDER}/../setup.py ${TULIP_PYTHON_FOLDER}/../setup.cfg)
  ADD_DEPENDENCIES(wheel copyTulipPythonPackagingFiles)
  IF(TULIP_GENERATE_TESTPYPI_WHEEL)
    ADD_DEPENDENCIES(wheel copyTulipPythonPackagingFiles)
  ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)
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
ELSE(APPLE AND TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
    IF(NOT LINUX OR NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
      TARGET_LINK_LIBRARIES(${LibTulipCorePythonBindingsName} PUBLIC ${Python_LIBRARIES})
    ENDIF(NOT LINUX OR NOT TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
ENDIF(APPLE AND TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

TULIP_INSTALL_PYTHON_FILES(tulip ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py)
TULIP_INSTALL_PYTHON_FILES(tulip/native ${LibTulipCorePythonBindingsName})


IF(NOT TULIP_BUILD_CORE_ONLY)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/build/_tulip.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_CLEAN_FILES "build;tulip_module;BooleanProperty.sip;ColorProperty.sip;DoubleProperty.sip;IntegerProperty.sip;LayoutProperty.sip;SizeProperty.sip;StringProperty.sip")
