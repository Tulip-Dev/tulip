IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

SET(TULIP_SIP_FILES
    AcyclicTest.sip
    Algorithm.sip
    Array.sip
    BiconnectedTest.sip
    BooleanAlgorithm.sip
    BooleanProperty.sip
    BoundingBox.sip
    ColorAlgorithm.sip
    ColorProperty.sip
    ColorScale.sip
    Color.sip
    ConnectedTest.sip
    Coord.sip
    DoubleAlgorithm.sip
    DoubleProperty.sip
    Edge.sip
    ExportModule.sip
    GraphProperty.sip
    Graph.sip
    ImportModule.sip
    IntegerAlgorithm.sip
    IntegerProperty.sip
    Iterator.sip
    LayoutAlgorithm.sip
    LayoutProperty.sip
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
    SizeProperty.sip
    Size.sip
    StringCollection.sip
    StringProperty.sip
    TlpTools.sip
    TreeTest.sip
    TriconnectedTest.sip
    Vector.sip
    WithDependency.sip
    WithParameter.sip
)

# Generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${SIP_EXE} -b ${CMAKE_CURRENT_BINARY_DIR}/tulip.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/tulip.sbf TULIP_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z_]*\\.cpp" TULIP_PYTHON_BINDINGS_SRC ${TULIP_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_PYTHON_BINDINGS_SRC}
                   COMMAND ${SIP_EXE} -o -I ${CMAKE_CURRENT_BINARY_DIR} -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating Python Bindings for libtulip"
                   DEPENDS ${TULIP_SIP_FILES})
           
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -DSIP_STATIC_MODULE")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -DSIP_STATIC_MODULE")

IF(UNIX)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${TulipInclude} ${TulipBuildInclude})

SET_SOURCE_FILES_PROPERTIES(siptuliptlpIteratorGraph.cpp
                            PROPERTIES COMPILE_DEFINITIONS ITERATOR_NO_COPY)
                       
ADD_LIBRARY(${LibTulipPythonName} STATIC ${TULIP_PYTHON_BINDINGS_SRC})

ADD_CUSTOM_TARGET(touchTulipPythonCMakeLists ALL
                  COMMAND cmake -E touch ${CMAKE_CURRENT_LIST_FILE}
                  COMMENT "touching CMakeLists.txt for Tulip Python Bindings (force configure before next build to keep source files list updated)"
                  DEPENDS ${LibTulipPythonName})

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
