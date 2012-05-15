#include "GlQuadTreeLodCalculatorTest.h"
#include <tulip/GlScene.h>
#include <tulip/GlQuadTreeLODCalculator.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlLayer.h>

CPPUNIT_TEST_SUITE_REGISTRATION( GlQuadTreeLodCalculatorTest );

GlQuadTreeLodCalculatorTest::GlQuadTreeLodCalculatorTest():scene(NULL),quadTreeLodCalculator(NULL)
{
}

void GlQuadTreeLodCalculatorTest::setUp(){

    //Build lod calculator
    quadTreeLodCalculator = new tlp::GlQuadTreeLODCalculator();
    //Build scene
    scene = new tlp::GlScene(quadTreeLodCalculator);
    //Add data
    tlp::GlLayer* layer=new tlp::GlLayer("Main");
    scene->addLayer(layer);
    graph = tlp::newGraph();
    //Add nodes.
    std::vector<tlp::node> addedNodes;
    graph->addNodes(50,addedNodes);
    tlp::GlGraphComposite* graphComposite=new tlp::GlGraphComposite(graph);
    scene->addGlGraphCompositeInfo(layer,graphComposite);
    layer->addGlEntity(graphComposite,"graph");
}

void GlQuadTreeLodCalculatorTest::tearDown(){
    delete scene;
    //The destruction of the scene destruct the lod calculator.
    quadTreeLodCalculator = NULL;
    scene=NULL;
    delete graph;
}

void GlQuadTreeLodCalculatorTest::testInputDataModifications(){

    tlp::Vector<int,4> viewport;
    viewport[0]=0;
    viewport[1]=0;
    viewport[2]=200;
    viewport[3]=200;
    //Compute lod for elements
    quadTreeLodCalculator->compute(viewport,viewport);
    CPPUNIT_ASSERT(!quadTreeLodCalculator->needEntities());
    tlp::GlGraphInputData* inputData = scene->getGlGraphComposite()->getInputData();
    //Overload some properties
    tlp::LayoutProperty newLayout(inputData->getGraph());
    tlp::ColorProperty newColor(inputData->getGraph());
    inputData->setElementLayout(&newLayout);
    inputData->setElementColor(&newColor);
    CPPUNIT_ASSERT(quadTreeLodCalculator->needEntities());
    //Compute new lod
    quadTreeLodCalculator->compute(viewport,viewport);
    CPPUNIT_ASSERT(!quadTreeLodCalculator->needEntities());
    //Restore original properties
    inputData->reloadAllProperties();
    //Be sure the object need to be updated again
    CPPUNIT_ASSERT(quadTreeLodCalculator->needEntities());
}
