INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

SET(prefix "${CMAKE_INSTALL_PREFIX}")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pluginstest.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/pluginstest.cpp" @ONLY)

SET(TULIP_PLUGINS_TESTS_SRCS
  BasicPluginsTest.cpp
  pluginstest.cpp)

ADD_EXECUTABLE(tulip_plugins_tests  ${TULIP_PLUGINS_TESTS_SRCS})
TARGET_LINK_LIBRARIES(tulip_plugins_tests ${LibTulipCoreName} cppunit)

ADD_CUSTOM_TARGET(copy_adj_mat ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/adj_mat.txt ${CMAKE_CURRENT_BINARY_DIR}/adj_mat.txt)
ADD_CUSTOM_TARGET(copy_cmpb ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CMPb.gml ${CMAKE_CURRENT_BINARY_DIR}/CMPb.gml)
ADD_CUSTOM_TARGET(copy_dot ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/graph.dot ${CMAKE_CURRENT_BINARY_DIR}/graph.dot)
ADD_CUSTOM_TARGET(copy_unconnected ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/unconnected.tlp ${CMAKE_CURRENT_BINARY_DIR}/unconnected.tlp)

ENABLE_TESTING()
ADD_TEST(TulipPluginsTestSuite tulip_plugins_tests )
