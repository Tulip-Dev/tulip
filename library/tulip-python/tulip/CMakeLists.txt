IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

SET(TULIP_SIP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/AcyclicTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractProperty.sip.in
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Array.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BiconnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BooleanAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BooleanPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/BoundingBox.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ColorScale.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Color.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Coord.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DoubleAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DoublePropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/DrawingTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Edge.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ExportModule.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Graph.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphEvent.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphProperty.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphMeasure.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/ImportModule.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegerAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegerPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Iterator.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/LayoutAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/LayoutPropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/MapIterator.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Matrix.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Node.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Observable.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/OuterPlanarTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PlanarityTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginContext.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginLoader.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginProgress.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/PropertyInterface.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Reflect.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SimpleTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SizeAlgorithm.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/SizePropertySpecific.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Size.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/StringCollection.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TlpTools.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/TriconnectedTest.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/Vector.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/WithDependency.sip
    ${CMAKE_CURRENT_SOURCE_DIR}/WithParameter.sip
    CACHE INTERNAL ""
)

# First, generate tulip properties bindings based on template SIP files
# Then, generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -DWORKING_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/generatePropertiesBindings.cmake)
EXECUTE_PROCESS(COMMAND ${SIP_EXE} -b ${CMAKE_CURRENT_BINARY_DIR}/tulip.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/tulip.sbf TULIP_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z_]*\\.cpp" TULIP_PYTHON_BINDINGS_SRC ${TULIP_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python Bindings for libtulip"
                   DEPENDS ${TULIP_SIP_FILES} ${STL_SIP_FILES})
           
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${TulipInclude} ${TulipBuildInclude})

SET_SOURCE_FILES_PROPERTIES(siptuliptlpIteratorGraph.cpp
                            PROPERTIES COMPILE_DEFINITIONS ITERATOR_NO_COPY)
                            
SET_SOURCE_FILES_PROPERTIES(siptuliptlpBooleanProperty.cpp 
							siptuliptlpBooleanVectorProperty.cpp
							siptuliptlpDoubleProperty.cpp
							siptuliptlpDoubleVectorProperty.cpp
							siptuliptlpIntegerProperty.cpp
							siptuliptlpIntegerVectorProperty.cpp	
                            PROPERTIES COMPILE_DEFINITIONS PROPERTY_BASE_TYPE)                            
 
ADD_LIBRARY(${LibTulipPythonName} SHARED ${TULIP_PYTHON_BINDINGS_SRC})

SET_TARGET_PROPERTIES(${LibTulipPythonName} 
		      PROPERTIES OUTPUT_NAME tulip PREFIX "")

IF(WIN32)
SET_TARGET_PROPERTIES(${LibTulipPythonName} 
		      PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
SET_TARGET_PROPERTIES(${LibTulipPythonName} 
		      PROPERTIES SUFFIX ".so")		      
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${LibTulipPythonName} ${LibTulipName})
TARGET_LINK_LIBRARIES(${LibTulipPythonName} ${PYTHON_LIBRARY})

ADD_CUSTOM_TARGET(touchTulipPythonCMakeLists ALL
                  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE}
                  COMMENT "touching CMakeLists.txt for Tulip Python Bindings (force configure before next build to keep source files list updated)"
                  DEPENDS ${LibTulipPythonName})

INSTALL(TARGETS ${LibTulipPythonName}
        RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT pythonbindings
        LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT pythonbindings
        ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT pythonbindings)

ADD_CUSTOM_COMMAND(
				   TARGET ${LibTulipPythonName}
				   POST_BUILD
                   COMMAND ${SIP_EXE} -a ${CMAKE_CURRENT_BINARY_DIR}/tulip.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating QScintilla API file for Tulip Python bindings"
				   VERBATIM)
				   
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulip.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT pythonbindings)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tulipplugins.py DESTINATION ${TulipLibInstallDir} COMPONENT pythonbindings)

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
