IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/Reflect.sip
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

# First, generate tulip properties bindings based on template SIP files
# Then, generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -DWORKING_DIR=${CMAKE_CURRENT_BINARY_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/generatePropertiesBindings.cmake WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
EXECUTE_PROCESS(COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -b ${CMAKE_CURRENT_BINARY_DIR}/tulip.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/tulip.sbf TULIP_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z_]*\\.cpp" TULIP_PYTHON_BINDINGS_SRC ${TULIP_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python Bindings for libtulip"
                   DEPENDS ${TULIP_SIP_FILES} ${STL_SIP_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../../include/tulip/PythonCppTypesConverter.h VERBATIM)
           
DISABLE_COMPILER_WARNINGS()

INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipPythonInclude})

SET_SOURCE_FILES_PROPERTIES(siptuliptlpIteratorGraph.cpp
                            siptulipIteratorObservable.cpp
                            PROPERTIES COMPILE_DEFINITIONS ITERATOR_NO_COPY)
                            
SET_SOURCE_FILES_PROPERTIES(siptuliptlpBooleanProperty.cpp
                            siptuliptlpBooleanVectorProperty.cpp
                            siptuliptlpDoubleProperty.cpp
                            siptuliptlpDoubleVectorProperty.cpp
                            siptuliptlpIntegerProperty.cpp
                            siptuliptlpIntegerVectorProperty.cpp
                            PROPERTIES COMPILE_DEFINITIONS PROPERTY_BASE_TYPE)
 
SET(TULIP_PYTHON_BINDINGS_SRC
    ${TULIP_PYTHON_BINDINGS_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/PythonCppTypesConverter.cpp)

ADD_LIBRARY(${LibTulipCorePythonBindingsName} SHARED ${TULIP_PYTHON_BINDINGS_SRC})

IF(NOT SYSTEM_SIP)
ADD_DEPENDENCIES(${LibTulipCorePythonBindingsName} ${SIP_LIB})
ENDIF(NOT SYSTEM_SIP)

SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName}
                      PROPERTIES OUTPUT_NAME tulip PREFIX "")

IF(WIN32)
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName}
                      PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
SET_TARGET_PROPERTIES(${LibTulipCorePythonBindingsName}
                      PROPERTIES SUFFIX ".so")
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${LibTulipCorePythonBindingsName} ${LibTulipCoreName})
TARGET_LINK_LIBRARIES(${LibTulipCorePythonBindingsName} ${PYTHON_LIBRARY})

ADD_CUSTOM_TARGET(touchTulipPythonCMakeLists ALL
                  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE}
                  COMMENT "touching CMakeLists.txt for Tulip Python Bindings (force configure before next build to keep source files list updated)"
                  DEPENDS ${LibTulipCorePythonBindingsName} VERBATIM)

IF(NOT UBUNTU_PPA_BUILD)
INSTALL(TARGETS ${LibTulipCorePythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/python COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python COMPONENT tulip_python)
ELSE()
INSTALL(TARGETS ${LibTulipCorePythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/../lib/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python)
ENDIF()

ADD_CUSTOM_COMMAND(TARGET ${LibTulipCorePythonBindingsName}
                   POST_BUILD
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -a ${CMAKE_CURRENT_BINARY_DIR}/tulip.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating QScintilla API file for Tulip Python bindings"
                   VERBATIM)
				   
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulip.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulip.sbf;tulip.api;sipAPItulip.h;BooleanProperty.sip;ColorProperty.sip;DoubleProperty.sip;IntegerProperty.sip;LayoutProperty.sip;SizeProperty.sip;StringProperty.sip")

#IF(NOT UBUNTU_PPA_BUILD)
#INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tulipplugins.py DESTINATION ${TulipLibInstallDir}/python COMPONENT tulip_python)
#ELSE()
#INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tulipplugins.py DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python)
#ENDIF()

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
