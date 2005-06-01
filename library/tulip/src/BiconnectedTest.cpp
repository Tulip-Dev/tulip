#include "tulip/SuperGraph.h"
#include "tulip/BiconnectedTest.h"
#include "tulip/MutableContainer.h"

using namespace std;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
BiconnectedTest * BiconnectedTest::instance=0;
#endif
#else
BiconnectedTest * BiconnectedTest::instance=0;
#endif
//=================================================================
bool biconnectedTest(SuperGraph *graph, node v, 
		  MutableContainer<bool> &mark,
		  MutableContainer<unsigned int> &low,
		  MutableContainer<unsigned int> &dfsNumber,
		  MutableContainer<node> &father,
		  unsigned int &count) {
  mark.set(v.id,true);
  dfsNumber.set(v.id,count);
  low.set(v.id,count);
  ++count;
  Iterator<node> *it=graph->getInOutNodes(v);
  while (it->hasNext()) {
    node w=it->next();
    if (!mark.get(w.id)) {
      if (dfsNumber.get(v.id)==1) {
	if (count != 2) {
	  //  cerr <<"Root Node :" << v.id << " is a separator" << endl;
	  delete it;
	  return false;
	}
      }
      father.set(w.id,v);
      if (!biconnectedTest(graph,w,mark,low,dfsNumber,father,count)) {
	delete it;
	return false;
      }
      if (dfsNumber.get(v.id)!=1) {
	if (low.get(w.id)>=dfsNumber.get(v.id)) {
	  delete it;
	  //cerr <<"Node :" << v.id << " is a separator" << endl;
	  return false;
	}
	else
	  low.set(v.id, low.get(v.id) <? low.get(w.id));
      }
    }
    else
      if (father.get(v.id)!=w) {
	low.set(v.id, low.get(v.id) <? dfsNumber.get(w.id));
      }
  } delete it;
  return true;
}
//=================================================================
BiconnectedTest::BiconnectedTest(){
}
//=================================================================
bool BiconnectedTest::isBiconnected(SuperGraph *graph) {
  if (instance==0)
    instance=new BiconnectedTest();
  return instance->compute(graph);
}
//=================================================================
bool BiconnectedTest::compute(SuperGraph *graph) { 
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned int)graph];
  graph->addObserver(this);
  MutableContainer<bool> mark;
  mark.setAll(false);
  MutableContainer<unsigned int> low;
  MutableContainer<unsigned int> dfsNumber;
  MutableContainer<node> father;
  unsigned int count = 1;
  bool result = false;
  Iterator<node> *it=graph->getNodes();
  if (it->hasNext())
    result=(biconnectedTest(graph,it->next(),mark,low,dfsNumber,father,count));
  delete it;
  if (count!=graph->numberOfNodes()+1) {
    //  cerr << "connected test" << endl; 
    result=false;
  } //connected test
  resultsBuffer[(unsigned int)graph]=result;
  return result;
}
//=================================================================
void BiconnectedTest::addEdge(SuperGraph *graph,const edge) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned int)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void BiconnectedTest::delEdge(SuperGraph *graph,const edge) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void BiconnectedTest::reverseEdge(SuperGraph *graph,const edge) {
}
//=================================================================
void BiconnectedTest::addNode(SuperGraph *graph,const node) {
  resultsBuffer[(unsigned int)graph]=false;
}
//=================================================================
void BiconnectedTest::delNode(SuperGraph *graph,const node) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void BiconnectedTest::destroy(SuperGraph *graph) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================

