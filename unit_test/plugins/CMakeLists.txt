INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${CppUnit_INCLUDE_DIRS})

SET(TULIP_PLUGINS_TESTS_SRCS
  BasicPluginsTest.cpp
  basicmetrictest.cpp
  basiclayouttest.cpp
  pluginstest.cpp)

SET_SOURCE_FILES_PROPERTIES(pluginstest.cpp
                            PROPERTIES COMPILE_DEFINITIONS TULIP_BUILD_DIR="${CMAKE_BINARY_DIR}")

ADD_CUSTOM_TARGET(copy_data ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data VERBATIM)

UNIT_TEST(TulipPluginsTestSuite ${TULIP_PLUGINS_TESTS_SRCS})
SET_TESTS_PROPERTIES(TulipPluginsTestSuite PROPERTIES DEPENDS copy_data)
