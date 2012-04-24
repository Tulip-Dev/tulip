INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

SET(TULIP_LIBRARY_TESTS_SRCS
  tuliplibtest.cpp
  importexporttest.cpp
#   ExtendedClusterOperationTest.cpp
#   VectorTest.cpp
#   MatrixTest.cpp
#   TestAlgorithmTest.cpp
#   BooleanPropertyTest.cpp
#   GraphPropertyTest.cpp
#   SuperGraphTest.cpp
#   TlpImportExportTest.cpp
#   PluginsTest.cpp
#   IdManagerTest.cpp
#   DataSetTest.cpp
#   ObservablePropertyTest.cpp
#   MutableContainerTest.cpp
#   ObservableGraphTest.cpp
#   PushPopTest.cpp
#   IntegerPropertyMinMaxSubgraphTest.cpp
#   IntegerPropertyMinMaxUpdateTest.cpp
#   TestPropertiesMinMaxAfterAddNode.cpp
#   IsMetaEdgeTest.cpp
#   stringpropertytest.cpp
#   DoublePropertyTest.cpp
#   PlanarityTestTest.cpp
#   withparametertest.cpp
#   FaceIteratorTest.cpp
  )

ADD_EXECUTABLE(tulip_tests ${TULIP_LIBRARY_TESTS_SRCS})
TARGET_LINK_LIBRARIES(tulip_tests ${LibTulipCoreName} cppunit)

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

ADD_CUSTOM_TARGET(copyTestData ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/DATA ${CMAKE_CURRENT_BINARY_DIR}/DATA )
ADD_CUSTOM_TARGET(copyTestData2 ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TlpImportExportTest.cpp ${CMAKE_CURRENT_BINARY_DIR}/TlpImportExportTest.cpp )

ENABLE_TESTING()
ADD_TEST(TulipLibraryTestSuite tulip_tests)
SET_TESTS_PROPERTIES(TulipLibraryTestSuite PROPERTIES DEPENDS copyTestData)
# ADD_DEPENDENCIES(TulipLibraryTestSuite copyTestData)
