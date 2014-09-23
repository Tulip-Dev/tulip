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

#include <string>
#include <iostream>

#include <cppunit/TestCaller.h>
#include "ExistEdgeTest.h"

using namespace tlp;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( ExistEdgeTest );

void ExistEdgeTest::setUp() {
  graph = newGraph();
  n0 = graph->addNode();
  n1 = graph->addNode();
  e0 = graph->addEdge(n0, n1);
  e1 = graph->addEdge(n1, n0);
}

void ExistEdgeTest::tearDown() {
  delete graph;
}

void ExistEdgeTest::testExistEdge() {
  CPPUNIT_ASSERT(graph->existEdge(n0, n0).isValid() == false);
  CPPUNIT_ASSERT(graph->existEdge(n0, n0, false).isValid() == false);
  CPPUNIT_ASSERT(graph->existEdge(n1, n1).isValid() == false);
  CPPUNIT_ASSERT(graph->existEdge(n1, n1, false).isValid() == false);
  CPPUNIT_ASSERT(graph->existEdge(n0, n1) == e0);
  CPPUNIT_ASSERT(graph->existEdge(n0, n1, false) == e0);
  CPPUNIT_ASSERT(graph->existEdge(n1, n0) == e1);
  CPPUNIT_ASSERT(graph->existEdge(n1, n0, false) == e0);
}

