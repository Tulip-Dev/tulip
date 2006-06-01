#include "tulip/Graph.h"
#include "tulip/AcyclicTest.h"
#include "tulip/BooleanProperty.h"
#include "tulip/StableIterator.h"

using namespace std;
using namespace tlp;

#ifdef _WIN32 
#ifdef DLL_EXPORT
AcyclicTest * AcyclicTest::instance=0;
#endif
#else
AcyclicTest * AcyclicTest::instance=0;
#endif
//**********************************************************************
AcyclicTest::AcyclicTest(){
}
//**********************************************************************
bool AcyclicTest::isAcyclic(const Graph *sg) {
  if (instance==0)
    instance = new AcyclicTest();

  if (instance->resultsBuffer.find((unsigned long)sg) == instance->resultsBuffer.end()) {
    instance->resultsBuffer[(unsigned long)sg] = acyclicTest(sg);
    sg->addObserver(instance);
  }
  
  return instance->resultsBuffer[(unsigned long)sg];
}
//**********************************************************************
void AcyclicTest::makeAcyclic(Graph* sg,vector<edge> &reversed, vector<tlp::SelfLoops> &selfLoops) {
  if (AcyclicTest::isAcyclic(sg)) return;

  //replace self loops by three edges and two nodes.
  StableIterator<edge> itE(sg->getEdges());
  while (itE.hasNext()) {
    edge e = itE.next();
    if (sg->source(e) == sg->target(e)) {
      node n1 = sg->addNode();
      node n2 = sg->addNode();
      selfLoops.push_back(tlp::SelfLoops(n1 , n2 , 
					 sg->addEdge(sg->source(e), n1) , 
					 sg->addEdge(n1,n2) , 
					 sg->addEdge(sg->source(e), n2) , 
					 e ));
      sg->delEdge(e);
    }
  }

  //find obstruction edges
  reversed.clear();
  acyclicTest(sg, &reversed);
  //  cerr << "reversed : " << reversed.size() << endl;
  if (reversed.size() > sg->numberOfEdges() / 2) {
    cerr << "[Warning]: " << __FUNCTION__ << ", is not efficient" << endl;
  }

  vector<edge>::const_iterator it = reversed.begin();
  for (; it != reversed.end(); ++it)
    sg->reverse(*it);

  assert(AcyclicTest::isAcyclic(sg));
 }

//=================================================================
bool AcyclicTest::dfsAcyclicTest(const Graph *sg, const node n, 
				 MutableContainer<bool> &visited, 
				 MutableContainer<bool> &finished,
				 vector<edge> *obstructionEdges) {
  visited.set(n.id,true);
  bool result = true;
  Iterator<edge> *it = sg->getOutEdges(n);
  while (it->hasNext()) {
    edge tmp = it->next();
    node nextNode = sg->target(tmp);
    if (visited.get(nextNode.id)) {
      if (!finished.get(nextNode.id)) {
	result = false;
	if (obstructionEdges != 0)
	  obstructionEdges->push_back(tmp);
	else {
	  break;
	}
      }
    }
    else {
      bool tmp = dfsAcyclicTest(sg, nextNode, visited, finished, obstructionEdges);
      result = tmp && result;
      if ((!result) && (obstructionEdges==0)) break;
    }
  } delete it;
  finished.set(n.id,true);
  return result;
}
//**********************************************************************
bool AcyclicTest::acyclicTest(const Graph *sg, vector<edge> *obstructionEdges) {
  MutableContainer<bool> visited;
  MutableContainer<bool> finished;
  visited.setAll(false);
  finished.setAll(false);
  bool result = true;
  Iterator<node> *it = sg->getNodes();
  while (it->hasNext()) {
    node curNode=it->next();
    if (!visited.get(curNode.id)) {
      if (!dfsAcyclicTest(sg, curNode, visited, finished, obstructionEdges)) {
	result = false;
	if (obstructionEdges == 0) {
	  break;
	}
      }
    }
  } delete it;
  return result;
}
//**********************************************************************
void AcyclicTest::destroy(Graph *sg) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//**********************************************************************
void AcyclicTest::reverseEdge(Graph *sg,const edge e) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//**********************************************************************
void AcyclicTest::addEdge(Graph *sg,const edge e) {
  if (resultsBuffer[(unsigned long)sg]==false) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//**********************************************************************
void AcyclicTest::delEdge(Graph *sg,const edge e) {
  if (resultsBuffer[(unsigned long)sg]==true) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//**********************************************************************
