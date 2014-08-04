INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${CppUnit_INCLUDE_DIRS})

ADD_LIBRARY(testPlugin SHARED testPlugin.cpp)
SET_TARGET_PROPERTIES(testPlugin 
   PROPERTIES
   PREFIX "")
TARGET_LINK_LIBRARIES(testPlugin ${LibTulipCoreName})

ADD_LIBRARY(testPlugin2 SHARED testPlugin2.cpp)
SET_TARGET_PROPERTIES(testPlugin2
   PROPERTIES
   PREFIX "")
TARGET_LINK_LIBRARIES(testPlugin2 ${LibTulipCoreName})

ADD_CUSTOM_TARGET(copyTestData ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/DATA ${CMAKE_CURRENT_BINARY_DIR}/DATA VERBATIM)
ADD_CUSTOM_TARGET(copyScript ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tulip_tests.sh ${CMAKE_CURRENT_BINARY_DIR}/tulip_tests.sh VERBATIM)

UNIT_TEST(ImportExportTest importexporttest.cpp tuliplibtest.cpp)
UNIT_TEST(ExistEdgeTest ExistEdgeTest.cpp tuliplibtest.cpp)
UNIT_TEST(ExtendedClusterOperationTest ExtendedClusterOperationTest.cpp tuliplibtest.cpp)
UNIT_TEST(VectorTest VectorTest.cpp tuliplibtest.cpp)
UNIT_TEST(MatrixTest MatrixTest.cpp tuliplibtest.cpp)
UNIT_TEST(TestAlgorithmTest TestAlgorithmTest.cpp tuliplibtest.cpp)
UNIT_TEST(BooleanPropertyTest BooleanPropertyTest.cpp tuliplibtest.cpp)
UNIT_TEST(GraphPropertyTest GraphPropertyTest.cpp tuliplibtest.cpp)
UNIT_TEST(SuperGraphTest SuperGraphTest.cpp tuliplibtest.cpp)
UNIT_TEST(TlpImportExportTest TlpImportExportTest.cpp tuliplibtest.cpp)
UNIT_TEST(IdManagerTest IdManagerTest.cpp tuliplibtest.cpp)
UNIT_TEST(DataSetTest DataSetTest.cpp tuliplibtest.cpp)
UNIT_TEST(ObservablePropertyTest ObservablePropertyTest.cpp tuliplibtest.cpp)
UNIT_TEST(MutableContainerTest MutableContainerTest.cpp tuliplibtest.cpp)
UNIT_TEST(ObservableGraphTest ObservableGraphTest.cpp tuliplibtest.cpp)
UNIT_TEST(PushPopTest PushPopTest.cpp tuliplibtest.cpp)
UNIT_TEST(IntegerPropertyMinMaxSubgraphTest IntegerPropertyMinMaxSubgraphTest.cpp tuliplibtest.cpp)
UNIT_TEST(IntegerPropertyMinMaxUptodateTest IntegerPropertyMinMaxUptodateTest.cpp tuliplibtest.cpp)
UNIT_TEST(TestPropertiesMinMaxAfterAddNode TestPropertiesMinMaxAfterAddNode.cpp tuliplibtest.cpp)
UNIT_TEST(IsMetaEdgeTest IsMetaEdgeTest.cpp tuliplibtest.cpp)
UNIT_TEST(StringPropertyTest stringpropertytest.cpp tuliplibtest.cpp)
UNIT_TEST(DoublePropertyTest DoublePropertyTest.cpp tuliplibtest.cpp)
UNIT_TEST(PlanarityTestTest PlanarityTestTest.cpp tuliplibtest.cpp)
UNIT_TEST(WithParameterTest withparametertest.cpp tuliplibtest.cpp)
# UNIT_TEST(FaceIteratorTest.cpp)
UNIT_TEST(PluginsTest PluginsTest.cpp tuliplibtest.cpp)
SET_TESTS_PROPERTIES(PluginsTest PROPERTIES DEPENDS copyTestData)
