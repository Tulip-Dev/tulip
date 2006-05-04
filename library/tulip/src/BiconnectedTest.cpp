#include "tulip/Graph.h"
#include "tulip/StableIterator.h"
#include "tulip/BiconnectedTest.h"
#include "tulip/ConnectedTest.h"
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
bool BiconnectedTest::isBiconnected(Graph *sg) {
  if (instance==0)
    instance=new BiconnectedTest();
  return instance->compute(sg);
}
//=================================================================
void BiconnectedTest::makeBiconnected(Graph *sg, vector<edge> &addedEdges) {
  //  cerr << __PRETTY_FUNCTION__ << " : " << sg->getAttribute<string>("name") << endl;
  //  if (BiconnectedTest::isBiconnected(sg)) return;
  if (instance==0) instance=new BiconnectedTest();
  sg->removeObserver(instance);
  instance->resultsBuffer.erase((unsigned long)sg);  
  instance->connect(sg, addedEdges);
  assert(BiconnectedTest::isBiconnected(sg));
}
//=================================================================
BiconnectedTest::BiconnectedTest(){
}
//=================================================================
void makeBiconnectedDFS(Graph *sg, node from, 
			MutableContainer<int> &low,
			MutableContainer<int> &dfsNumber,
			MutableContainer<node> &father,
			unsigned int &count,
			vector<edge> &addedEdges) {
  node u;
  dfsNumber.set(from.id, count++);
  low.set(from.id, dfsNumber.get(from.id));
  StableIterator<node> itN(sg->getInOutNodes(from));
  while (itN.hasNext()) {
    node w = itN.next();
    if (from == w) continue;
    if (!u.isValid()) u = w;
    if (dfsNumber.get(w.id) == -1) {
      father.set(w.id, from);
      makeBiconnectedDFS(sg, w, low, dfsNumber, father, count, addedEdges);
      if (low.get(w.id) == dfsNumber.get(from.id)) {
	if (w == u && father.get(from.id).isValid())
	  addedEdges.push_back(sg->addEdge(w, father.get(from.id)));
	if (w != u) 
	  addedEdges.push_back(sg->addEdge(u, w));
      }
      low.set(from.id, std::min(low.get(from.id), low.get(w.id)));
    }
    else
      low.set(from.id, std::min(low.get(from.id), dfsNumber.get(w.id)));
  } 
}
//=================================================================
bool biconnectedTest(Graph *sg, node v, 
		     MutableContainer<bool> &mark,
		     MutableContainer<unsigned int> &low,
		     MutableContainer<unsigned int> &dfsNumber,
		     MutableContainer<node> &father,
		     unsigned int &count) {
  mark.set(v.id,true);
  dfsNumber.set(v.id,count);
  low.set(v.id,count);
  ++count;
  Iterator<node> *it=sg->getInOutNodes(v);
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
      if (!biconnectedTest(sg,w,mark,low,dfsNumber,father,count)) {
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
	  low.set(v.id, std::min(low.get(v.id), low.get(w.id)));
      }
    }
    else
      if (father.get(v.id)!=w) {
	low.set(v.id, std::min(low.get(v.id), dfsNumber.get(w.id)));
      }
  } delete it;
  return true;
}
//=================================================================
void BiconnectedTest::connect(Graph *sg, vector<edge> &addedEdges) {
  //  cerr << __PRETTY_FUNCTION__ << " : " << sg->getAttribute<string>("name") << endl;
  ConnectedTest::makeConnected(sg, addedEdges);
  MutableContainer<int> low;
  MutableContainer<int> dfsNumber;
  dfsNumber.setAll(-1);
  MutableContainer<node> father;
  father.setAll(node());
  unsigned int count = 0;
  makeBiconnectedDFS(sg, sg->getOneNode(), low, dfsNumber, father, count, addedEdges);
}
//=================================================================
bool BiconnectedTest::compute(Graph *sg) { 
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)sg];

  MutableContainer<bool> mark;
  mark.setAll(false);
  MutableContainer<unsigned int> low;
  MutableContainer<unsigned int> dfsNumber;
  MutableContainer<node> father;
  unsigned int count = 1;
  bool result = false;
  Iterator<node> *it=sg->getNodes();
  if (it->hasNext())
    result=(biconnectedTest(sg,it->next(),mark,low,dfsNumber,father,count));
  delete it;
  if (count!=sg->numberOfNodes()+1) {
    //  cerr << "connected test" << endl; 
    result=false;
  } //connected test
  resultsBuffer[(unsigned long)sg]=result;
  sg->addObserver(this);
  return result;
}
//=================================================================
void BiconnectedTest::addEdge(Graph *sg,const edge) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)sg]) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void BiconnectedTest::delEdge(Graph *sg,const edge) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end())
    if (!resultsBuffer[(unsigned long)sg]) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void BiconnectedTest::reverseEdge(Graph *sg,const edge) {
}
//=================================================================
void BiconnectedTest::addNode(Graph *sg,const node) {
  //  cerr << __PRETTY_FUNCTION__  << (unsigned)sg << endl;
  resultsBuffer[(unsigned long)sg]=false;
}
//=================================================================
void BiconnectedTest::delNode(Graph *sg,const node) {
  //  cerr << __PRETTY_FUNCTION__  << (unsigned)sg << endl;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void BiconnectedTest::destroy(Graph *sg) {
  //  cerr << __PRETTY_FUNCTION__ << (unsigned)sg << endl;
  //  cerr << "Graph name : " << sg->getAttribute<string>("name") << endl;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================

