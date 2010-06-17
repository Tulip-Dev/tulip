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
#include "FaceIteratorTest.h"
#include <cppunit/TestCaller.h>

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( FaceIteratorTest );
using namespace std;

//============================================================
// FaceIteratorTest
//============================================================
void FaceIteratorTest::build(){
  carte->clear();
  edges.clear();
  nodes.clear();
  
  for(unsigned int i = 0; i<10; i++)
    nodes.push_back(carte->addNode());

  edges.push_back(carte->addEdge(nodes[0],nodes[1]));
  edges.push_back(carte->addEdge(nodes[1],nodes[2]));
  edges.push_back(carte->addEdge(nodes[3],nodes[2]));

  edges.push_back(carte->addEdge(nodes[4],nodes[3]));
  edges.push_back(carte->addEdge(nodes[3],nodes[5]));
  edges.push_back(carte->addEdge(nodes[3],nodes[6]));

  edges.push_back(carte->addEdge(nodes[7],nodes[6]));
  edges.push_back(carte->addEdge(nodes[7],nodes[1]));

  edges.push_back(carte->addEdge(nodes[0],nodes[8]));
  edges.push_back(carte->addEdge(nodes[8],nodes[9]));
  edges.push_back(carte->addEdge(nodes[0],nodes[9]));

  edges.push_back(carte->addEdge(nodes[4],nodes[2]));
  edges.push_back(carte->addEdge(nodes[4],nodes[5]));
  edges.push_back(carte->addEdge(nodes[7],nodes[9]));
}

//============================================================
void FaceIteratorTest::testNodeFaceIterator(){
  build();
  carte->update();

  Iterator<Face>* itf = carte->getFaces();
  while(itf->hasNext()){
    Face f = itf->next();
    unsigned int i = 0;
    Iterator<node>* it = carte->getFaceNodes(f);
    while(it->hasNext()){
      node n = it->next();
      i++;
    }
    CPPUNIT_ASSERT_MESSAGE(" test NodeFaceIterator ",carte->facesEdges[f].size() == i);
    delete it;
  }
  delete itf;
}  

//============================================================
void FaceIteratorTest::testFaceAdjIterator(){
  build();
  carte->update();

  unsigned int i = 0;
  Iterator<Face>* it = carte->getFacesAdj(nodes[4]);
  while(it->hasNext()){
    it->next();
    i++;
  }
  delete it;
  CPPUNIT_ASSERT_MESSAGE(" test FaceAdjIterator nbFaces ", 3 == i);

  i = 0;
  it = carte->getFacesAdj(nodes[1]);
  while(it->hasNext()){
    it->next();
    i++;
  }
  delete it;
  CPPUNIT_ASSERT_MESSAGE(" test FaceadjIterator nbFaces ", 3 == i);

  i = 0;
  it = carte->getFacesAdj(nodes[8]);
  while(it->hasNext()){
    it->next();
    i++;
  }
  delete it;
  CPPUNIT_ASSERT_MESSAGE(" test FaceAdjIterator nbFaces ", 2 == i);
}

//============================================================
CppUnit::Test* FaceIteratorTest::suite(){
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite;
  suiteOfTests->addTest(new CppUnit::TestCaller<FaceIteratorTest>(
		"testNodeFaceIterator", &FaceIteratorTest::testNodeFaceIterator));
  suiteOfTests->addTest(new CppUnit::TestCaller<FaceIteratorTest>(
		"testFaceAdjIterator", &FaceIteratorTest::testFaceAdjIterator));
  return suiteOfTests;
}
