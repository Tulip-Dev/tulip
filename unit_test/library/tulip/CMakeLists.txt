INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

MACRO(UNIT_TEST file)
  STRING(REPLACE ".cpp" "" TEST_NAME ${file})
  ADD_EXECUTABLE(${TEST_NAME} ${file} tuliplibtest.cpp )
  TARGET_LINK_LIBRARIES(${TEST_NAME} ${LibTulipCoreName} cppunit)
  ADD_TEST(NAME ${TEST_NAME} COMMAND ${TEST_NAME} ${TEST_NAME})
ENDMACRO(UNIT_TEST)

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

ENABLE_TESTING()

# UNIT_TEST(importexporttest.cpp)
UNIT_TEST(ExtendedClusterOperationTest.cpp)
UNIT_TEST(VectorTest.cpp)
UNIT_TEST(MatrixTest.cpp)
UNIT_TEST(TestAlgorithmTest.cpp)
UNIT_TEST(BooleanPropertyTest.cpp)
UNIT_TEST(GraphPropertyTest.cpp)
UNIT_TEST(SuperGraphTest.cpp)
UNIT_TEST(TlpImportExportTest.cpp)
UNIT_TEST(IdManagerTest.cpp)
UNIT_TEST(DataSetTest.cpp)
UNIT_TEST(ObservablePropertyTest.cpp)
UNIT_TEST(MutableContainerTest.cpp)
UNIT_TEST(ObservableGraphTest.cpp)
UNIT_TEST(PushPopTest.cpp)
UNIT_TEST(IntegerPropertyMinMaxSubgraphTest.cpp)
UNIT_TEST(IntegerPropertyMinMaxUptodateTest.cpp)
UNIT_TEST(TestPropertiesMinMaxAfterAddNode.cpp)
UNIT_TEST(IsMetaEdgeTest.cpp)
UNIT_TEST(stringpropertytest.cpp)
UNIT_TEST(DoublePropertyTest.cpp)
UNIT_TEST(PlanarityTestTest.cpp)
UNIT_TEST(withparametertest.cpp)
# UNIT_TEST(FaceIteratorTest.cpp)
UNIT_TEST(PluginsTest.cpp)
SET_TESTS_PROPERTIES(PluginsTest PROPERTIES DEPENDS copyTestData)
# ADD_DEPENDENCIES(TulipLibraryTestSuite copyTestData)
