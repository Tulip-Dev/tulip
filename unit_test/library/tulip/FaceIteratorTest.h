/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
