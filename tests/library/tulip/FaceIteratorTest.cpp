/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include "FaceIteratorTest.h"

#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(FaceIteratorTest);

//============================================================
// FaceIteratorTest
//============================================================
void FaceIteratorTest::setUp() {
  map = computePlanarConMap(graph = tlp::newGraph());

  map->clear();
  edges.clear();
  nodes.clear();

  for (unsigned int i = 0; i < 10; i++)
    nodes.push_back(map->addNode());

  edges.push_back(map->addEdge(nodes[0], nodes[1]));
  edges.push_back(map->addEdge(nodes[1], nodes[2]));
  edges.push_back(map->addEdge(nodes[3], nodes[2]));

  edges.push_back(map->addEdge(nodes[4], nodes[3]));
  edges.push_back(map->addEdge(nodes[3], nodes[5]));
  edges.push_back(map->addEdge(nodes[3], nodes[6]));

  edges.push_back(map->addEdge(nodes[7], nodes[6]));
  edges.push_back(map->addEdge(nodes[7], nodes[1]));

  edges.push_back(map->addEdge(nodes[0], nodes[8]));
  edges.push_back(map->addEdge(nodes[8], nodes[9]));
  edges.push_back(map->addEdge(nodes[0], nodes[9]));

  edges.push_back(map->addEdge(nodes[4], nodes[2]));
  edges.push_back(map->addEdge(nodes[4], nodes[5]));
  edges.push_back(map->addEdge(nodes[7], nodes[9]));

  map->update();
}

void FaceIteratorTest::tearDown() {
  delete map;
  delete graph;
}

//============================================================
void FaceIteratorTest::testNodeFaceIterator() {
  for (const tlp::Face &f : map->getFaces()) {
    unsigned int i = iteratorCount(map->getFaceNodes(f));
    CPPUNIT_ASSERT_EQUAL_MESSAGE(" test NodeFaceIterator ", map->nbFacesEdges(f), i);
  }
}

//============================================================
void FaceIteratorTest::testFaceAdjIterator() {
  unsigned int i = iteratorCount(map->getFacesAdj(nodes[4]));
  CPPUNIT_ASSERT_EQUAL_MESSAGE(" test FaceAdjIterator nbFaces ", 3u, i);

  i = iteratorCount(map->getFacesAdj(nodes[1]));

  CPPUNIT_ASSERT_EQUAL_MESSAGE(" test FaceadjIterator nbFaces ", 3u, i);

  i = iteratorCount(map->getFacesAdj(nodes[8]));

  CPPUNIT_ASSERT_EQUAL_MESSAGE(" test FaceAdjIterator nbFaces ", 2u, i);
}
