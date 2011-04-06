IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

SET(TULIP_SIP_FILES
    AcyclicTest.sip
    AbstractPropertyPointerType.sip.in
    AbstractPropertyBaseType.sip.in
    Algorithm.sip
    Array.sip
    BiconnectedTest.sip
    BooleanAlgorithm.sip
    BooleanPropertySpecific.sip
    BoundingBox.sip
    ColorAlgorithm.sip
    ColorScale.sip
    Color.sip
    ConnectedTest.sip
    Coord.sip
    DoubleAlgorithm.sip
    DoublePropertySpecific.sip
    DrawingTools.sip
    Edge.sip
    ExportModule.sip
    Graph.sip
    GraphProperty.sip
    GraphMeasure.sip
    ImportModule.sip
    IntegerAlgorithm.sip
    IntegerPropertySpecific.sip
    Iterator.sip
    LayoutAlgorithm.sip
    LayoutPropertySpecific.sip
    MapIterator.sip
    Module.sip
    Node.sip
    OuterPlanarTest.sip
    PlanarityTest.sip
    PluginContext.sip
    PluginLoader.sip
    PluginProgress.sip
    Plugin.sip
    PropertyAlgorithm.sip
    PropertyInterface.sip
    Reflect.sip
    SimpleTest.sip
    SizeAlgorithm.sip
    SizePropertySpecific.sip
    Size.sip
    StringCollection.sip
    TlpTools.sip
    TreeTest.sip
    TriconnectedTest.sip
    Vector.sip
    WithDependency.sip
    WithParameter.sip
)

# First, generate tulip properties bindings based on template SIP files
# Then, generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -DWORKING_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/generatePropertiesBindings.cmake
				COMMAND ${SIP_EXE} -b ${CMAKE_CURRENT_BINARY_DIR}/tulip.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/tulip.sbf TULIP_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z_]*\\.cpp" TULIP_PYTHON_BINDINGS_SRC ${TULIP_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python Bindings for libtulip"
                   DEPENDS ${TULIP_SIP_FILES})
           
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${TulipInclude} ${TulipBuildInclude})

SET_SOURCE_FILES_PROPERTIES(siptuliptlpIteratorGraph.cpp
                            PROPERTIES COMPILE_DEFINITIONS ITERATOR_NO_COPY)
 
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




ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
