/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <cassert>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/PlanarityTest.h>
#include <tulip/PlanarConMap.h>
#include <tulip/ConnectedTest.h>
#include <tulip/LayoutProperty.h>
#include "PlanarityTestTest.h"

using namespace std;
using namespace tlp;

const std::string GRAPHPATH = "./DATA/graphs/";

static Graph* tlp_loadGraph(const std::string& filename) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *sg = tlp::importGraph("TLP Import", dataSet);
  return sg;
}

CPPUNIT_TEST_SUITE_REGISTRATION( PlanarityTestTest );
//==========================================================
void PlanarityTestTest::setUp() {
  //  graph = tlp::newGraph();
}
//==========================================================
void PlanarityTestTest::tearDown() {
  //  delete graph;
}
//==========================================================
void PlanarityTestTest::planarGraphs() {
  graph = tlp_loadGraph(GRAPHPATH + "planar/grid1010.tlp");
  CPPUNIT_ASSERT(PlanarityTest::isPlanar(graph));
  delete graph;
  graph = tlp_loadGraph(GRAPHPATH + "planar/unconnected.tlp");
  CPPUNIT_ASSERT(PlanarityTest::isPlanar(graph));
  delete graph;
  graph = tlp_loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");
  CPPUNIT_ASSERT(PlanarityTest::isPlanar(graph));
  delete graph;
}
//==========================================================
/*
 * TODO: move that function in LayoutProperty test.
 *
 */
void PlanarityTestTest::planarEmbeddingFromLayoutGraphs() {
  graph = tlp_loadGraph(GRAPHPATH + "planar/planar30drawnFPP.tlp.gz");
  LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
  layout->computeEmbedding(graph);
  CPPUNIT_ASSERT(PlanarityTest::isPlanarEmbedding(graph));
  delete graph;
  graph = tlp_loadGraph(GRAPHPATH + "planar/planar30drawnMM.tlp.gz");
  layout = graph->getProperty<LayoutProperty>("viewLayout");
  layout->computeEmbedding(graph);
  CPPUNIT_ASSERT(PlanarityTest::isPlanarEmbedding(graph));
  delete graph;
  graph = tlp_loadGraph(GRAPHPATH + "notplanar/k33lostInGrip.tlp.gz");
  layout = graph->getProperty<LayoutProperty>("viewLayout");
  layout->computeEmbedding(graph);
  CPPUNIT_ASSERT(!PlanarityTest::isPlanarEmbedding(graph));
  delete graph;
}
//==========================================================
void PlanarityTestTest::notPlanarGraphs() {
  graph = tlp_loadGraph(GRAPHPATH + "notplanar/k33lostInGrip.tlp.gz");
  CPPUNIT_ASSERT(!PlanarityTest::isPlanar(graph));
  delete graph;
  graph = tlp_loadGraph(GRAPHPATH + "notplanar/k33k55.tlp.gz");
  CPPUNIT_ASSERT(!PlanarityTest::isPlanar(graph));
  delete graph;
  graph = tlp_loadGraph(GRAPHPATH + "notplanar/k5lostingrid5050.tlp.gz");
  CPPUNIT_ASSERT(!PlanarityTest::isPlanar(graph));
  delete graph;
}
//==========================================================
unsigned int eulerIdentity(Graph *graph) {
  return graph->numberOfEdges() - graph->numberOfNodes()
         + 1u + ConnectedTest::numberOfConnectedComponents(graph);
}
//==========================================================
void PlanarityTestTest::planarGraphsEmbedding() {
  tlp::warning() << "==================================" << endl;
  graph = tlp_loadGraph(GRAPHPATH + "planar/grid1010.tlp");
  PlanarConMap *graphMap = computePlanarConMap(graph);
  //  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces());
  delete graphMap;
  delete graph;
  tlp::warning() << "==================================" << endl;
  graph = tlp_loadGraph(GRAPHPATH + "planar/unconnected.tlp");
  graph->setAttribute("name", string("unconnected"));
  // no planar connected map computed
  // beacause is not connected
  graphMap = computePlanarConMap(graph);
  CPPUNIT_ASSERT(graphMap == NULL);
  delete graph;
  tlp::warning() << "==================================" << endl;
  tlp::warning() << "unbiconnected" << endl;
  graph = tlp_loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");

  graphMap = computePlanarConMap(graph);

  //  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces());

  delete graphMap;
  delete graph;
  tlp::warning() << "==================================" << endl;
}
//==========================================================
void PlanarityTestTest::planarMetaGraphsEmbedding() {
  tlp::warning() << "===========MetaGraphsEmbedding=======================" << endl;
  graph = tlp_loadGraph(GRAPHPATH + "planar/grid1010.tlp");
  Graph * g = graph->addCloneSubGraph();
  set<node> toGroup;
  Iterator<node> * itn = g->getNodes();

  for(unsigned int i = 0; i < 10; ++i) {
    assert(itn->hasNext());
    toGroup.insert(itn->next());
  }

  g->createMetaNode(toGroup);
  toGroup.clear();

  for(unsigned int i = 0; i < 10; ++i) {
    assert(itn->hasNext());
    toGroup.insert(itn->next());
  }

  node meta2 = g->createMetaNode(toGroup);
  toGroup.clear();
  toGroup.insert(meta2);

  for(unsigned int i = 0; i < 10; ++i) {
    assert(itn->hasNext());
    toGroup.insert(itn->next());
  }

  g->createMetaNode(toGroup, false);
  toGroup.clear();
  delete itn;

  PlanarConMap *graphMap = computePlanarConMap(g);
  //  graphMap->makePlanar();
  CPPUNIT_ASSERT(PlanarityTest::isPlanar(g));//eulerIdentity(g), graphMap->nbFaces());
  CPPUNIT_ASSERT(PlanarityTest::isPlanar(graphMap));//eulerIdentity(g), graphMap->nbFaces());
  delete graphMap;
  graph->delSubGraph(g);
  delete graph;
  tlp::warning() << "==================================" << endl;
  /*
  graph = tlp::loadGraph(GRAPHPATH + "planar/unconnected.tlp");
  graph->setAttribute("name", string("unconnected"));
  graphMap = new PlanarConMap(graph);
  tlp::warning() << "Graph name : " << graph->getAttribute<string>("name") << endl;
  graphMap->makePlanar();*/
  /*
   * The number of faces must be adapted because the Planarity Test split the
   * external face into several faces (one by connected componnent).
   */
  /*  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces() - (ConnectedTest::numberOfConnectedComponnents(graph) - 1));
  delete graphMap;
  delete graph;
  tlp::warning() << "==================================" << endl;
  tlp::warning() << "unbiconnected" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");

  graphMap = new PlanarConMap(graph);

  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces());

  delete graphMap;
  delete graph;
  tlp::warning() << "==================================" << endl;*/
}

void PlanarityTestTest::emptyGraph() {
  graph = tlp::newGraph();
  CPPUNIT_ASSERT(PlanarityTest::isPlanar(graph));
  delete graph;
}
