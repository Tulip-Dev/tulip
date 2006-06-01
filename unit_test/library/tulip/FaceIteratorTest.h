//-*-c++-*-
/**
 Author: Sophie BARDET & Alexandre BURNETT
 Email : sophie.bardet@gmail.com, aburnett397@gmail.com
 Last modification : 19/06/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_FaceIteratorTest_h
#define Tulip_FaceIteratorTest_h

#include <tulip/FaceIterator.h>
#include <tulip/TlpTools.h>

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class FaceIteratorTest : public CppUnit::TestFixture{
  
private :
  PlanarConMap* carte;

  std::vector<edge> edges;
  std::vector<node> nodes; 

  void build();
  
public :
  void setUp(){
    carte = new PlanarConMap(tlp::newGraph());
    edges.clear();
    nodes.clear();
  }
  
  void tearDown(){
    delete carte;
  }

  void testNodeFaceIterator();
  void testFaceAdjIterator();

  static CppUnit::Test *suite();

};

#endif
