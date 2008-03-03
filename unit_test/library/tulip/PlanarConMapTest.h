//-*-c++-*-
/**
 Author: Romain BOURQUI
 Email : bourqui@labri.fr
 Last modification : 16/01/2006
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef PLANARCONMAPTEST_H
#define PLANARCONMAPTEST_H

#include <tulip/PlanarConMap.h>
#include <tulip/TlpTools.h>

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

namespace tlp {

class PlanarConMapTest : public CppUnit::TestFixture{
  
private :
  tlp::Graph* graph;
  tlp::PlanarConMap* carte;

  std::vector<edge> edges;
  std::vector<node> nodes; 

  void build();
  void build2();
  void build3();
  void build4();
  
public :
  void setUp(){
    graph = tlp::newGraph();
    //carte = new PlanarConMap(graph);
    edges.clear();
    nodes.clear();
  }
  
  void tearDown(){
    delete carte;
    delete graph;
  }

  void testAddEdgeMap();
  void testDelEdgeMap();
  void testNbFaces();

  void testUpdate();

  void testMergeFaces();
  void testSplitFace();

  void testSuccCycleEdge();   
  void testPrecCycleEdge();  

  void testComputeFaces();

  static CppUnit::Test *suite();

};

}
#endif
