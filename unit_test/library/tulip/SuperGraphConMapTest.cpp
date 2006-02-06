#include "SuperGraphConMapTest.h"
#include <tulip/PlanarityTest.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( SuperGraphConMapTest );

using namespace std;

//============================================================
// SuperGraphConMapTest
//============================================================
void SuperGraphConMapTest::build(){                                             
  graph->clear();carte->clear();edges.clear();nodes.clear();
  for(unsigned int i = 0; i < 5;i++)
    nodes.push_back(graph->addNode());

  edges.push_back(graph->addEdge(nodes[0], nodes[1]));
  edges.push_back(graph->addEdge(nodes[0], nodes[2]));
  edges.push_back(graph->addEdge(nodes[0], nodes[3]));
  edges.push_back(graph->addEdge(nodes[1], nodes[2])); 
  edges.push_back(graph->addEdge(nodes[3], nodes[4]));
  
  delete carte;
  carte = new SuperGraphConMap(graph);
}

//============================================================
void SuperGraphConMapTest::build2(){                                
  graph->clear();carte->clear();edges.clear();nodes.clear();
  for(int i = 0; i<6; i++)
    nodes.push_back(carte->addNode());
    
  edges.push_back(graph->addEdge(nodes[0], nodes[1])); 
  edges.push_back(graph->addEdge(nodes[1], nodes[2])); 
  edges.push_back(graph->addEdge(nodes[2], nodes[3])); 
  edges.push_back(graph->addEdge(nodes[4], nodes[3])); 
  
  edges.push_back(graph->addEdge(nodes[4], nodes[5])); 
  edges.push_back(graph->addEdge(nodes[0], nodes[5])); 
  delete carte;
  carte = new SuperGraphConMap(graph);
}

//============================================================
void SuperGraphConMapTest::build3(){                                             
  graph->clear(); carte->clear();edges.clear();nodes.clear();
  for(int i = 0; i<6; i++)
    nodes.push_back(graph->addNode());
    
  edges.push_back(graph->addEdge(nodes[0], nodes[1])); 
  edges.push_back(graph->addEdge(nodes[0], nodes[2])); 
  edges.push_back(graph->addEdge(nodes[0], nodes[3])); 
  edges.push_back(graph->addEdge(nodes[0], nodes[4])); 
  edges.push_back(graph->addEdge(nodes[0], nodes[5])); 
  delete carte;
  carte = new SuperGraphConMap(graph);
}

//============================================================
bool SuperGraphConMapTest::containFace(Face f){

  Iterator<Face>* it = carte->getFaces();
  while(it->hasNext()){
    Face tmp = it->next();
    if(tmp == f) return true;
  }
  delete it;
  return false;
}

//============================================================
void SuperGraphConMapTest::testAddEdgeMap(){                                               

  build();
  vector<vector<edge> > cycles(5);
  cycles[0].push_back(edges[0]);      cycles[1].push_back(edges[0]);
  cycles[0].push_back(edges[1]);      cycles[1].push_back(edges[3]);
  cycles[0].push_back(edges[2]);      

  cycles[2].push_back(edges[1]);      cycles[3].push_back(edges[2]);
  cycles[2].push_back(edges[3]);      cycles[3].push_back(edges[4]);

  cycles[4].push_back(edges[4]);
  
  for(unsigned int i = 0; i<nodes.size(); i++){
    vector<edge> tmp;
    Iterator<edge>* it = carte->getInOutEdges(nodes[i]);
    while(it->hasNext()){
      edge e = it->next();
      tmp.push_back(e);
    }
    delete it;
    
    CPPUNIT_ASSERT_MESSAGE(" test AddEdgeMap cycle ",tmp == cycles[i]);
  }
  
  CPPUNIT_ASSERT_MESSAGE(" test AddEdgeMap dispositif decorateur ",carte->numberOfNodes() == graph->numberOfNodes());
}

//============================================================
void SuperGraphConMapTest::testNbFaces(){                                                    

  build();
  carte->update();
  CPPUNIT_ASSERT_MESSAGE(" test nbFaces ",2 == carte->nbFaces());
}

//============================================================
void SuperGraphConMapTest::testUpdate(){                                                  

  build();
  carte->update();
  unsigned int tmp = carte->nbFaces();
  edge e = graph->addEdge(nodes[0],nodes[4]);
  carte->addEdgeMap(e);
  carte->update();
  
  CPPUNIT_ASSERT_MESSAGE(" test update() ", (tmp+1) == carte->nbFaces());   
}

//============================================================
void SuperGraphConMapTest::testSplitFace(){

  build2();
  carte->update();
  Face tmp;

  Iterator<Face>* it = carte->getFaces();
  tmp = it->next();
  unsigned int nb = carte->facesEdges[tmp].size();  

  /*   test 1    */
  edge e = graph->addEdge(nodes[0],nodes[3]);
  Face f = carte->splitFace(tmp,e);
  CPPUNIT_ASSERT_MESSAGE(" test splitFace edge ", carte->isElement(carte->existEdge(nodes[0], nodes[3])));
  CPPUNIT_ASSERT_MESSAGE(" test splitFace ",  nb == (carte->facesEdges[f].size()+carte->facesEdges[tmp].size()-2));
  for(unsigned int i = 0; i<7; i++)
    CPPUNIT_ASSERT_MESSAGE(" test splitFace edge exist", carte->isElement(edges[i]));

  /*   test 2    */
  nb = carte->facesEdges[tmp].size();
  e = graph->addEdge(nodes[5],nodes[3]);
  f = carte->splitFace(tmp,e);
    CPPUNIT_ASSERT_MESSAGE(" test splitFace edge 2", carte->isElement(carte->existEdge(nodes[5], nodes[3])));
  CPPUNIT_ASSERT_MESSAGE(" test splitFace 2",  nb == (carte->facesEdges[f].size()+carte->facesEdges[tmp].size()-2));
  for(unsigned int i = 0; i<8; i++)
    CPPUNIT_ASSERT_MESSAGE(" test splitFace edge exist 2", carte->isElement(edges[i]));


  build2();
  carte->update();
  it = carte->getFaces();
  tmp = it->next();
  nb = carte->facesEdges[tmp].size();  
  
  /*   test 3    */
  e = graph->addEdge(nodes[1],nodes[4]);
  f = carte->splitFace(tmp,e);
  CPPUNIT_ASSERT_MESSAGE(" test splitFace edge 3", carte->isElement(carte->existEdge(nodes[1], nodes[4])));
  CPPUNIT_ASSERT_MESSAGE(" test splitFace 3",  nb == (carte->facesEdges[f].size()+carte->facesEdges[tmp].size()-2));
  for(unsigned int i = 0; i<7; i++)
    CPPUNIT_ASSERT_MESSAGE(" test splitFace edge exist 3", carte->isElement(edges[i]));

   /*   test 4    */
  nb = carte->facesEdges[tmp].size();
  e = graph->addEdge(nodes[1],nodes[5]);
  f = carte->splitFace(tmp,e);
  CPPUNIT_ASSERT_MESSAGE(" test splitFace edge 4", carte->isElement(carte->existEdge(nodes[1], nodes[5])));
  CPPUNIT_ASSERT_MESSAGE(" test splitFace 4",  nb == (carte->facesEdges[f].size()+carte->facesEdges[tmp].size()-2));
  for(unsigned int i = 0; i<8; i++)
    CPPUNIT_ASSERT_MESSAGE(" test splitFace edge exist 4", carte->isElement(edges[i]));

}

//============================================================
void SuperGraphConMapTest::testSuccCycleEdge(){                                       

  build3();

  vector<edge> cycles;
  cycles.push_back(edges[0]);
  cycles.push_back(edges[1]);
  cycles.push_back(edges[2]);
  cycles.push_back(edges[3]);
  cycles.push_back(edges[4]); 
  cycles.push_back(edges[0]);
  
  unsigned int i = 0;
  Iterator<edge>* it = carte->getInOutEdges(nodes[0]);
  edge e = it->next();
  CPPUNIT_ASSERT_MESSAGE(" test SuccCycleEdge deb",e == cycles[0]);
  do{
    e = carte->succCycleEdge(e,nodes[0]);
    i++;
    CPPUNIT_ASSERT_MESSAGE(string(" test SuccCycleEdge cycle "),e == cycles[i]);
  }
  while(e != edges[0] && i<4);
  e = carte->succCycleEdge(e,nodes[0]);
  i++;
  CPPUNIT_ASSERT_MESSAGE(" test SuccCycleEdge end",e == cycles[i]);

  delete it;

}
   
//============================================================
void SuperGraphConMapTest::testPrecCycleEdge(){                                     

  build3();

  vector<edge> cycles;
  cycles.push_back(edges[0]);
  cycles.push_back(edges[4]);
  cycles.push_back(edges[3]);
  cycles.push_back(edges[2]);
  cycles.push_back(edges[1]);
  cycles.push_back(edges[0]);
  
  unsigned int i = 0;
  Iterator<edge>* it = carte->getInOutEdges(nodes[0]);
  edge e = it->next();
  CPPUNIT_ASSERT_MESSAGE(" test SuccCycleEdge deb",e == cycles[0]);
  do{
    e = carte->precCycleEdge(e,nodes[0]);
    i++;
    CPPUNIT_ASSERT_MESSAGE(string(" test SuccCycleEdge cycle "),e == cycles[i]);
  }
  while((e != edges[0]) && (i<4));
  e = carte->precCycleEdge(e,nodes[0]);
  i++;
  CPPUNIT_ASSERT_MESSAGE(" test SuccCycleEdge end",e == cycles[i]);

  delete it;
}  

//============================================================                         
void SuperGraphConMapTest::testComputeFaces(){

  build();
  carte->update();
  int fc = carte->nbFaces();
  int e = carte->numberOfEdges();
  int n = carte->numberOfNodes();
  CPPUNIT_ASSERT_MESSAGE(" test ComputesFaces 1", fc == e - n + 2);


  build2();
  carte->update();
  fc = carte->nbFaces();
  e = carte->numberOfEdges();
  n = carte->numberOfNodes();
  CPPUNIT_ASSERT_MESSAGE(" test ComputesFaces 2", fc == e - n + 2);


  build3();
  carte->update();
  fc = carte->nbFaces();
  e = carte->numberOfEdges();
  n = carte->numberOfNodes();
  CPPUNIT_ASSERT_MESSAGE(" test ComputesFaces 3 ", fc == e - n + 2);
}

//============================================================
CppUnit::Test* SuperGraphConMapTest::suite(){                    
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite;
  suiteOfTests->addTest(new CppUnit::TestCaller<SuperGraphConMapTest>("testAddEdgeMap", &SuperGraphConMapTest::testAddEdgeMap));
  suiteOfTests->addTest(new CppUnit::TestCaller<SuperGraphConMapTest>("testNbFaces", &SuperGraphConMapTest::testNbFaces));
  suiteOfTests->addTest(new CppUnit::TestCaller<SuperGraphConMapTest>("testUpdate", &SuperGraphConMapTest::testUpdate));
  suiteOfTests->addTest(new CppUnit::TestCaller<SuperGraphConMapTest>("testSuccCycleEdge", &SuperGraphConMapTest::testSuccCycleEdge));
  suiteOfTests->addTest(new CppUnit::TestCaller<SuperGraphConMapTest>("testPrecCycleEdge", &SuperGraphConMapTest::testPrecCycleEdge));
  suiteOfTests->addTest(new CppUnit::TestCaller<SuperGraphConMapTest>("testSplitFace", &SuperGraphConMapTest::testSplitFace));
  suiteOfTests->addTest(new CppUnit::TestCaller<SuperGraphConMapTest>("testComputeFaces", &SuperGraphConMapTest::testComputeFaces));
  return suiteOfTests;
}
//============================================================
