SET(TULIP_PLUGINS_TESTS_SRCS
  BasicPluginsTest.cpp
  BasicMetricTest.cpp
  BasicLayoutTest.cpp
  pluginstest.cpp)

SET_SOURCE_FILES_PROPERTIES(pluginstest.cpp pluginsloadingtest.cpp
                            PROPERTIES COMPILE_DEFINITIONS TULIP_BUILD_DIR="${CMAKE_BINARY_DIR}")

ADD_CUSTOM_TARGET(copy_data ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data VERBATIM)

UNIT_TEST(TulipPluginsTestSuite ${TULIP_PLUGINS_TESTS_SRCS})
SET_TESTS_PROPERTIES(TulipPluginsTestSuite PROPERTIES DEPENDS copy_data)

QTX_SET_INCLUDES_AND_DEFINITIONS()
UNIT_TEST(TulipPluginsLoadingTest pluginsloadingtest.cpp)
TARGET_LINK_LIBRARIES(TulipPluginsLoadingTest ${QT_LIBRARIES})
