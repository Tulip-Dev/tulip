INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipBuildInclude})

SET(TULIP_OGL_LIBRARY_TESTS_SRCS
  Main.cpp
  GlQuadTreeLodCalculatorTest.cpp
  GlVertexArrayManagerTest.cpp
  )

ADD_EXECUTABLE(tulip_ogl_tests ${TULIP_OGL_LIBRARY_TESTS_SRCS})
TARGET_LINK_LIBRARIES(tulip_ogl_tests ${LibTulipName} ${LibTulipOglName} cppunit)

ENABLE_TESTING()
ADD_TEST(TulipOglLibraryTestSuite tulip_ogl_tests)
