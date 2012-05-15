#ifndef GLQUADTREELODCALCULATORTEST_H
#define GLQUADTREELODCALCULATORTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>



namespace tlp{
class GlQuadTreeLODCalculator;
class GlScene;
}
/**
 * @brief Unitary test for the GlQuadTreeLodCalculator object
 */
class GlQuadTreeLodCalculatorTest : public CppUnit::TestFixture
{
    tlp::GlScene* scene;
    tlp::GlQuadTreeLODCalculator* quadTreeLodCalculator;
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
