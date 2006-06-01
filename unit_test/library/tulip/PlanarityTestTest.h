/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
 */

#ifndef PLANARITYGRAPHTEST_H
#define PLANARITYGRAPHTEST_H

#include <string>
#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>


class PlanarityTestTest : public CppUnit::TestFixture {

 public:
  void setUp();
  void tearDown();
  void planarGraphs();
  void notPlanarGraphs();
  void planarGraphsEmbedding();
  void planarMetaGraphsEmbedding();
  void notPlanarGraphsObstruction();

  static CppUnit::Test *suite();

 private:
  Graph *graph;
};

#endif 
