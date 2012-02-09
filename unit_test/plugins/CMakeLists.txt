INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude})

SET(prefix ${CMAKE_INSTALL_PREFIX})
CONFIGURE_FILE(pluginstest.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/pluginstest.cpp)

SET(TULIP_PLUGINS_TESTS_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/pluginstest.cpp
    BasicPluginsTest.cpp
  )

ADD_EXECUTABLE(tulip_plugins_tests ${TULIP_PLUGINS_TESTS_SRCS})
TARGET_LINK_LIBRARIES(tulip_plugins_tests ${LibTulipName} cppunit)


ADD_CUSTOM_TARGET(copy_data ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)

ENABLE_TESTING()
ADD_TEST(TulipPluginsTestSuite tulip_plugins_tests)

