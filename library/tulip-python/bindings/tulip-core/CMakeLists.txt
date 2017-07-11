IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

ADD_CORE_FILES(.)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDLL_TULIP_PYTHON")
ENDIF(WIN32)

# Visual Studio has a limit for string constants length
# so we define the sip feature NO_LIMIT_DOC in Module.sip
# to avoid the compiler error (fatal error C1091)
IF(MSVC AND MSVC_VERSION LESS 1900)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/TulipFontAwesome.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TulipMaterialDesignIcons.sip
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
                   COMMAND ${SIP_EXE} -y ${CMAKE_CURRENT_BINARY_DIR}/tulip.pyi -o -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} -j8 ${SIP_FLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python Bindings for libtulip"
                   DEPENDS ${TULIP_SIP_FILES} ${STL_SIP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../../include/tulip/PythonCppTypesConverter.h VERBATIM)
           
DISABLE_COMPILER_WARNINGS()

# Force to use SIP and headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipPythonInclude})
 
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

# Ensure bindings build output folder exists
ADD_CUSTOM_TARGET(create-tulip-python-native-folder ALL
                  COMMAND ${CMAKE_COMMAND} -E make_directory ${TULIP_PYTHON_NATIVE_FOLDER})
ADD_DEPENDENCIES(${LibTulipCorePythonBindingsName} create-tulip-python-native-folder)


ADD_CUSTOM_TARGET(copyTulipInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${TULIP_PYTHON_FOLDER}/__init__.py)
IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
ADD_DEPENDENCIES(tulip-core-wheel copyTulipInitPy)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
STRING(REGEX REPLACE "[^0-9.]" "" TulipVersionClean ${TulipVersion})

IF(WIN32)
SET(WHEEL_INSTALL_PATH "\\")
ELSE(WIN32)
SET(WHEEL_INSTALL_PATH "/")
ENDIF(WIN32)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_PYTHON_FOLDER}/../setup.py ${TULIP_PYTHON_FOLDER}/../setup.cfg
  COMMAND ${CMAKE_COMMAND} -DINPUT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/packaging/
                           -DOUTPUT_DIR=${TULIP_PYTHON_FOLDER}/../
                           -DTulipVersionClean=${TulipVersionClean}
                           -DPythonWheelVersionSuffix=${PythonWheelVersionSuffix}
                           -DWHEEL_INSTALL_PATH=${WHEEL_INSTALL_PATH}
                           -P ${CMAKE_CURRENT_SOURCE_DIR}/packaging/configure_tulip_python_setup.cmake
  DEPENDS
    packaging/setup.py.in
    packaging/setup.cfg.in
  COMMENT "Generating tulip-python packaging files"
  VERBATIM)


ADD_CUSTOM_TARGET(copyTulipPythonPackagingFiles ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/MANIFEST.in ${TULIP_PYTHON_FOLDER}/../MANIFEST.in
                                                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/README.rst ${TULIP_PYTHON_FOLDER}/../README.rst
                                                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/copyTulipCoreDllDependencies.cmake ${TULIP_PYTHON_FOLDER}/../copyTulipCoreDllDependencies.cmake
                                                DEPENDS ${TULIP_PYTHON_FOLDER}/../setup.py ${TULIP_PYTHON_FOLDER}/../setup.cfg
)
ADD_DEPENDENCIES(tulip-core-wheel copyTulipPythonPackagingFiles)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

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

# On MacOS, add the paths of dependencies dylibs in install rpaths of the _tulip.so binary
# That way, the tulip module can be imported in a classical Python shell without having to
# modify the content of the DYLD_LIBRARY_PATH environment variable
IF(APPLE AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipCorePythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS};@loader_path/../../../;@loader_path/../../../../Frameworks")
ELSEIF(LINUX AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipCorePythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS}:$ORIGIN/../../../")
ENDIF(APPLE AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

TARGET_LINK_LIBRARIES(${LibTulipCorePythonBindingsName} ${LibTulipCoreName})
# When building Python wheel for MacOS, don't link the C extension module with the Python library
# and use dynamic lookup for retrieving its symbols.
# That way, we can produce a C extension module that can be imported through the Python interpreter
# provided by Apple with the System and the one provided by Python.org
IF(APPLE AND TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ELSE(APPLE AND TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  IF(NOT LINUX OR NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
    TARGET_LINK_LIBRARIES(${LibTulipCorePythonBindingsName} ${PYTHON_LIBRARY})
  ENDIF(NOT LINUX OR NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
ENDIF(APPLE AND TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

INSTALL_TULIP_PYTHON_FILES(tulip ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py)
INSTALL_TULIP_PYTHON_FILES(tulip/native ${LibTulipCorePythonBindingsName})

IF(NOT TULIP_BUILD_CORE_ONLY)
  ADD_CUSTOM_COMMAND(TARGET ${LibTulipCorePythonBindingsName}
                     POST_BUILD
                     COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -a ${CMAKE_CURRENT_BINARY_DIR}/tulip.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                     COMMENT "Generating API file for Tulip Python bindings"
                     VERBATIM)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulip.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)
ENDIF(NOT TULIP_BUILD_CORE_ONLY)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulip.api;sipAPI_tulip.h;tulip_module;BooleanProperty.sip;ColorProperty.sip;DoubleProperty.sip;IntegerProperty.sip;LayoutProperty.sip;SizeProperty.sip;StringProperty.sip")

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
