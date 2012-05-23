#ifndef GLQUADTREELODCALCULATORTEST_H
#define GLQUADTREELODCALCULATORTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>



namespace tlp {
class GlQuadTreeLODCalculator;
class GlScene;
class Graph;
}
/**
 * @brief Unitary test for the GlQuadTreeLodCalculator object
 */
class GlQuadTreeLodCalculatorTest : public CppUnit::TestFixture {
  tlp::GlScene* scene;
  tlp::GlQuadTreeLODCalculator* quadTreeLodCalculator;
  tlp::Graph* graph;
public:
  GlQuadTreeLodCalculatorTest();
  void setUp();
  void tearDown();

  void testInputDataModifications();

  CPPUNIT_TEST_SUITE( GlQuadTreeLodCalculatorTest );
  CPPUNIT_TEST( testInputDataModifications );
  CPPUNIT_TEST_SUITE_END();
};

#endif // GLQUADTREELODCALCULATORTEST_H
