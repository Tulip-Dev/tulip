/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef Tulip_FaceIteratorTest_h
#define Tulip_FaceIteratorTest_h

#include <tulip/FaceIterator.h>
#include <tulip/PlanarConMap.h>
#include <tulip/TlpTools.h>

#include "CppUnitIncludes.h"

class FaceIteratorTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(FaceIteratorTest);
  CPPUNIT_TEST(testNodeFaceIterator);
  CPPUNIT_TEST(testFaceAdjIterator);
  CPPUNIT_TEST_SUITE_END();

private:
  tlp::Graph *graph;
  tlp::PlanarConMap *map;

  std::vector<tlp::edge> edges;
  std::vector<tlp::node> nodes;

public:
  void setUp();
  void tearDown();

  void testNodeFaceIterator();
  void testFaceAdjIterator();
};

#endif
