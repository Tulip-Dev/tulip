#include <tulip/Graph.h>
#include <tulip/PlanarityTest.h>
#include <tulip/PlanarityTestImpl.h>
#include <tulip/BiconnectedTest.h>

using namespace std;
using namespace tlp;
//=================================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
PlanarityTest * PlanarityTest::instance=0;
#endif
#else
PlanarityTest * PlanarityTest::instance=0;
#endif
//=================================================================
bool PlanarityTest::isPlanar(Graph *sg){
  if(instance==0)
    instance = new PlanarityTest();
  return instance->compute(sg);
}
//=================================================================
bool PlanarityTest::planarEmbedding(Graph *sg) {
  if (!PlanarityTest::isPlanar(sg))
    return false;
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(sg, addedEdges);
  PlanarityTestImpl planarTest(sg);
  planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();
  for (; it!=addedEdges.end(); ++it)
    sg->delAllEdge(*it);
  return true;
}
//=================================================================
list<edge> PlanarityTest::getObstructionsEdges(Graph *sg) {
  if (PlanarityTest::isPlanar(sg))
    return list<edge>();
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(sg, addedEdges);
  PlanarityTestImpl planarTest(sg);
  planarTest.isPlanar(true);
  list<edge> tmpList = planarTest.getObstructions();
  {
    vector<edge>::const_iterator it = addedEdges.begin();
    for (; it!=addedEdges.end(); ++it)
      sg->delAllEdge(*it);
  }
  set<edge> tmpAdded(addedEdges.begin(), addedEdges.end());
  list<edge> result;
  for (list<edge>::iterator it = tmpList.begin(); it != tmpList.end(); ++it) {
    if (tmpAdded.find(*it) == tmpAdded.end()) 
      result.push_back(*it);
  }
  return result;
}
//=================================================================
bool PlanarityTest::compute(Graph *sg) { 
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned long)sg];
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(sg, addedEdges);
  PlanarityTestImpl planarTest(sg);
  resultsBuffer[(unsigned long)sg] = planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();
  for (; it!=addedEdges.end(); ++it)
    sg->delAllEdge(*it);
  sg->addObserver(this);
  return resultsBuffer[(unsigned long)sg];
}
//=================================================================
void PlanarityTest::addEdge(Graph *sg,const edge) {
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end())
    if (!resultsBuffer[(unsigned long)sg]) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void PlanarityTest::delEdge(Graph *sg,const edge) {
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)sg]) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void PlanarityTest::reverseEdge(Graph *sg,const edge) {
}
//=================================================================
void PlanarityTest::addNode(Graph *sg,const node) {
}
//=================================================================
void PlanarityTest::delNode(Graph *sg,const node) {
  if (resultsBuffer.find((unsigned long)sg)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)sg]) return;
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
void PlanarityTest::destroy(Graph *sg) {
  sg->removeObserver(this);
  resultsBuffer.erase((unsigned long)sg);
}
//=================================================================
