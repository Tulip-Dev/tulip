INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

SET(prefix "${CMAKE_INSTALL_PREFIX}")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pluginstest.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/pluginstest.cpp" @ONLY)

SET(TULIP_PLUGINS_TESTS_SRCS
  BasicPluginsTest.cpp
  basicmetrictest.cpp
  basiclayouttest.cpp
  pluginstest.cpp)

IF(USE_QT5)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
ENDIF(USE_QT5)

ADD_EXECUTABLE(tulip_plugins_tests  ${TULIP_PLUGINS_TESTS_SRCS})
TARGET_LINK_LIBRARIES(tulip_plugins_tests ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} cppunit)

ADD_CUSTOM_TARGET(copy_data ALL ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data VERBATIM)

ENABLE_TESTING()
ADD_TEST(TulipPluginsTestSuite tulip_plugins_tests )
