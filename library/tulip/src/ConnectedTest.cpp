#include "tulip/Graph.h"
#include "tulip/ConnectedTest.h"
#include "tulip/MutableContainer.h"

using namespace std;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
ConnectedTest * ConnectedTest::instance=0;
#endif
#else
ConnectedTest * ConnectedTest::instance=0;
#endif
//=================================================================
bool ConnectedTest::isConnected(Graph *sg) {
  if (instance==0)
    instance=new ConnectedTest();
  return instance->compute(sg);
}
//=================================================================
void ConnectedTest::makeConnected(Graph *sg, vector<edge> &addedEdges) {
  //  cerr << __PRETTY_FUNCTION__ << " : " << sg->getAttribute<string>("name") << endl;
  if (instance==0)
    instance=new ConnectedTest();
  sg->removeObserver(instance);
  instance->resultsBuffer.erase((unsigned long)sg);  
  vector<node> toLink;  
  instance->connect(sg, toLink);
  for (unsigned int i = 1; i < toLink.size(); ++i)
    addedEdges.push_back(sg->addEdge(toLink[i-1], toLink[i]));
  assert(ConnectedTest::isConnected(sg));
}
//=================================================================
unsigned int ConnectedTest::numberOfConnectedComponnents(Graph *sg) {
  if (sg->numberOfNodes()==0) return 0u;
  if (instance==0)
    instance=new ConnectedTest();
  sg->removeObserver(instance);
  vector<node> toLink;
  instance->connect(sg, toLink);
  unsigned int result;
  if (toLink.size()>0) 
    result =  toLink.size();
  else 
    result = 1u;
  instance->resultsBuffer[(unsigned long)sg] = (result == 1u);
  sg->addObserver(instance);
  return result;
}
//=================================================================
void connectedTest(Graph *sg, node n, 
		   MutableContainer<bool> &visited,
		   unsigned int &count) {
  if (visited.get(n.id)) return;
  visited.set(n.id,true);
  ++count;
  Iterator<node> *it=sg->getInOutNodes(n);
  while (it->hasNext()) {
    connectedTest(sg, it->next(), visited, count);
  } delete it;
}
//=================================================================
ConnectedTest::ConnectedTest(){}
//=================================================================
bool ConnectedTest::compute(Graph *sg) {
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)sg];
  if (sg->numberOfNodes()==0) return true;
  MutableContainer<bool> visited;
  visited.setAll(false);
  unsigned int count = 0;
  connectedTest(sg, sg->getOneNode(), visited, count);
  bool result = (count == sg->numberOfNodes());
  resultsBuffer[(unsigned long)sg]=result;
  sg->addObserver(this);
  return result;
}
//=================================================================
void ConnectedTest::connect(Graph *sg, vector<node> &toLink) {
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end()) {
    if (resultsBuffer[(unsigned long)sg])
      return;
  }
  if (sg->numberOfNodes()==0) return;
  MutableContainer<bool> visited;
  visited.setAll(false);
  unsigned int count = 0;
  Iterator<node> *itN = sg->getNodes();
  while(itN->hasNext()) {
    node n = itN->next();
    if (!visited.get(n.id)) {
      toLink.push_back(n);
      connectedTest(sg, n, visited, count);
    }
  }
}
//=================================================================
void ConnectedTest::addEdge(Graph *sg,const edge) {
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)sg]) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void ConnectedTest::delEdge(Graph *sg,const edge) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end())
    if (!resultsBuffer[(unsigned long)sg]) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void ConnectedTest::reverseEdge(Graph *sg,const edge) {
}
//=================================================================
void ConnectedTest::addNode(Graph *sg,const node) {
  resultsBuffer[(unsigned long)sg]=false;
}
//=================================================================
void ConnectedTest::delNode(Graph *sg,const node) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void ConnectedTest::destroy(Graph *sg) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================

