#ifndef GLVERTEXARRAYMANAGERTEST_H
#define GLVERTEXARRAYMANAGERTEST_H
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
class GlGraphInputData;
class GlVertexArrayManager;
class GlGraphRenderingParameters;
class GlGraphComposite;
class Graph;
}
class GlVertexArrayManagerTest : public CppUnit::TestFixture {
  tlp::GlGraphComposite* composite;
  tlp::GlGraphInputData* inputData;
  tlp::GlVertexArrayManager* vertexArrayManager;
  tlp::Graph* graph;
public:
  GlVertexArrayManagerTest();
  void setUp();
  void tearDown();

  void testInputDataModifications();

  CPPUNIT_TEST_SUITE( GlVertexArrayManagerTest );
  CPPUNIT_TEST( testInputDataModifications );
  CPPUNIT_TEST_SUITE_END();
};

#endif // GLVERTEXARRAYMANAGERTEST_H
