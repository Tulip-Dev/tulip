#include "GlVertexArrayManagerTest.h"
#include <tulip/GlGraphInputData.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlVertexArrayVisitor.h>
CPPUNIT_TEST_SUITE_REGISTRATION( GlVertexArrayManagerTest );

GlVertexArrayManagerTest::GlVertexArrayManagerTest() {
}

void GlVertexArrayManagerTest::setUp() {
  graph = tlp::newGraph();
  //Add nodes.
  std::vector<tlp::node> addedNodes;
  graph->addNodes(50,addedNodes);
  composite = new tlp::GlGraphComposite(graph);
  inputData = composite->getInputData();
  //Get the GlVertexArray manager.
  vertexArrayManager = inputData->getGlVertexArrayManager();
}

void GlVertexArrayManagerTest::tearDown() {
  //Force to reload old properties.
  inputData->reloadAllProperties();
  //Reinit vertex array manager
  vertexArrayManager->setInputData(inputData);
  delete composite;
  composite = NULL;
  inputData = NULL;
  vertexArrayManager = NULL;
  delete graph;
}

void GlVertexArrayManagerTest::testInputDataModifications() {

  tlp::GlVertexArrayVisitor vertexArrayVisitor(inputData);
  composite->acceptVisitor(&vertexArrayVisitor);
  vertexArrayManager->setHaveToComputeAll(false);
  CPPUNIT_ASSERT(!vertexArrayManager->haveToCompute());

  //Overload layout properties
  tlp::LayoutProperty newLayout(inputData->getGraph());
  inputData->setElementLayout(&newLayout);
  CPPUNIT_ASSERT(vertexArrayManager->haveToCompute());

  //Reset value
  composite->acceptVisitor(&vertexArrayVisitor);
  vertexArrayManager->setHaveToComputeAll(false);
  CPPUNIT_ASSERT(!vertexArrayManager->haveToCompute());

  //Oveload size
  tlp::SizeProperty newSize(inputData->getGraph());
  inputData->setElementSize(&newSize);
  CPPUNIT_ASSERT(vertexArrayManager->haveToCompute());

  //Reset value
  composite->acceptVisitor(&vertexArrayVisitor);
  vertexArrayManager->setHaveToComputeAll(false);
  CPPUNIT_ASSERT(!vertexArrayManager->haveToCompute());

  //Oveload shape
  tlp::IntegerProperty newShape(inputData->getGraph());
  inputData->setElementShape(&newShape);
  CPPUNIT_ASSERT(vertexArrayManager->haveToCompute());

  //Reset value
  composite->acceptVisitor(&vertexArrayVisitor);
  vertexArrayManager->setHaveToComputeAll(false);
  CPPUNIT_ASSERT(!vertexArrayManager->haveToCompute());

  //Oveload rotation
  tlp::DoubleProperty newRotation(inputData->getGraph());
  inputData->setElementRotation(&newRotation);
  CPPUNIT_ASSERT(vertexArrayManager->haveToCompute());

  //Reset value
  composite->acceptVisitor(&vertexArrayVisitor);
  vertexArrayManager->setHaveToComputeAll(false);
  CPPUNIT_ASSERT(!vertexArrayManager->haveToCompute());

  //Overload color property
  tlp::ColorProperty newColor(inputData->getGraph());
  inputData->setElementColor(&newColor);
  CPPUNIT_ASSERT(vertexArrayManager->haveToCompute());

  //Reset value
  composite->acceptVisitor(&vertexArrayVisitor);
  vertexArrayManager->setHaveToComputeAll(false);
  CPPUNIT_ASSERT(!vertexArrayManager->haveToCompute());

  //Overload border color property
  tlp::ColorProperty newBorderColor(inputData->getGraph());
  inputData->setElementBorderColor(&newBorderColor);
  CPPUNIT_ASSERT(vertexArrayManager->haveToCompute());

  //Restore original properties
  inputData->reloadAllProperties();
  vertexArrayManager->setInputData(inputData);
  composite->acceptVisitor(&vertexArrayVisitor);
  vertexArrayManager->setHaveToComputeAll(false);
}
