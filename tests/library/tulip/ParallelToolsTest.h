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
#ifndef PARALLEL_TOOLS_TEST_H
#define PARALLEL_TOOLS_TEST_H

#include <string>

#include "CppUnitIncludes.h"

#include <tulip/Graph.h>

class ParallelToolsTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(ParallelToolsTest);
  CPPUNIT_TEST(testParallelMapIndices);
  CPPUNIT_TEST(testParallelMapNodes);
  CPPUNIT_TEST(testParallelMapEdges);
  CPPUNIT_TEST(testParallelMapNodesAndIndices);
  CPPUNIT_TEST(testParallelMapEdgesAndIndices);
  CPPUNIT_TEST(testCriticalSection);
  CPPUNIT_TEST(testNumberOfThreads);
  CPPUNIT_TEST_SUITE_END();

private:
  tlp::Graph *_graph;

public:
  void setUp() override;
  void tearDown() override;
  void testParallelMapIndices();
  void testParallelMapNodes();
  void testParallelMapEdges();
  void testParallelMapNodesAndIndices();
  void testParallelMapEdgesAndIndices();
  void testCriticalSection();
  void testNumberOfThreads();
};

#endif
