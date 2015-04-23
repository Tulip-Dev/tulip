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

#ifndef PLANARITYGRAPHTEST_H
#define PLANARITYGRAPHTEST_H

#include <string>
#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class PlanarityTestTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PlanarityTestTest);
  CPPUNIT_TEST(planarGraphs);
  CPPUNIT_TEST(notPlanarGraphs);
  CPPUNIT_TEST(planarGraphsEmbedding);
  CPPUNIT_TEST(planarMetaGraphsEmbedding);
  CPPUNIT_TEST(planarEmbeddingFromLayoutGraphs);
  CPPUNIT_TEST(emptyGraph);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown();
  void planarGraphs();
  void notPlanarGraphs();
  void planarGraphsEmbedding();
  void planarMetaGraphsEmbedding();
  void planarEmbeddingFromLayoutGraphs();
  void emptyGraph();
private:
  tlp::Graph *graph;
};

#endif
