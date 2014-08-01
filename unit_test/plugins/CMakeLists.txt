INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${CppUnit_INCLUDE_DIRS})

SET(prefix "${CMAKE_INSTALL_PREFIX}")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pluginstest.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/pluginstest.cpp" @ONLY)

SET(TULIP_PLUGINS_TESTS_SRCS
  BasicPluginsTest.cpp
  basicmetrictest.cpp
  basiclayouttest.cpp
  pluginstest.cpp)

ADD_EXECUTABLE(tulip_plugins_tests  ${TULIP_PLUGINS_TESTS_SRCS})

# link against tulip-ogl and tulip-gui libraries
# to ensure correct loading of some of the tested plugins
TARGET_LINK_LIBRARIES(tulip_plugins_tests ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${CppUnit_LIBRARIES})

ADD_CUSTOM_TARGET(copy_data ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data VERBATIM)

ADD_TEST(TulipPluginsTestSuite tulip_plugins_tests)
