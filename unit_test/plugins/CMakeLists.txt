INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude})

SET(prefix "${CMAKE_INSTALL_PREFIX}")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pluginstest.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/pluginstest.cpp" @ONLY)

SET(TULIP_PLUGINS_TESTS_SRCS
  BasicPluginsTest.cpp
  pluginstest.cpp)

ADD_EXECUTABLE(tulip_plugins_tests  ${TULIP_PLUGINS_TESTS_SRCS})
TARGET_LINK_LIBRARIES(tulip_plugins_tests ${LibTulipName} cppunit)

# ADD_CUSTOM_TARGET(copyTestData ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/DATA ${CMAKE_CURRENT_BINARY_DIR}/DATA )
# ADD_CUSTOM_TARGET(copyTestData2 ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TlpImportExportTest.cpp ${CMAKE_CURRENT_BINARY_DIR}/TlpImportExportTest.cpp )

ENABLE_TESTING()
ADD_TEST(TulipPluginsTestSuite tulip_plugins_tests )
SET_TESTS_PROPERTIES(TulipPluginsTestSuite PROPERTIES DEPENDS copyTestData)
# ADD_DEPENDENCIES(TulipPluginsTestSuite copyTestData)
