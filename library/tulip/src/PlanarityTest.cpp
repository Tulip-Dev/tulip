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
bool PlanarityTest::isPlanar(Graph *graph){
  if(instance==0)
    instance = new PlanarityTest();
  return instance->compute(graph);
}
//=================================================================
bool PlanarityTest::planarEmbedding(Graph *graph) {
  if (!PlanarityTest::isPlanar(graph))
    return false;
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();
  for (; it!=addedEdges.end(); ++it)
    graph->delAllEdge(*it);
  return true;
}
//=================================================================
list<edge> PlanarityTest::getObstructionsEdges(Graph *graph) {
  if (PlanarityTest::isPlanar(graph))
    return list<edge>();
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);
  list<edge> tmpList = planarTest.getObstructions();
  {
    vector<edge>::const_iterator it = addedEdges.begin();
    for (; it!=addedEdges.end(); ++it)
      graph->delAllEdge(*it);
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
bool PlanarityTest::compute(Graph *graph) { 

  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned int)graph];
  else if(graph->numberOfNodes()==0){
    resultsBuffer[(unsigned int)graph] = true;
    return true;
  }
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  resultsBuffer[(unsigned long)graph] = planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();
  for (; it!=addedEdges.end(); ++it)
    graph->delAllEdge(*it);
  graph->addObserver(this);
  return resultsBuffer[(unsigned long)graph];
}
//=================================================================
void PlanarityTest::addEdge(Graph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (!resultsBuffer[(unsigned long)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void PlanarityTest::delEdge(Graph *graph,const edge) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void PlanarityTest::reverseEdge(Graph *graph,const edge) {
}
//=================================================================
void PlanarityTest::addNode(Graph *graph,const node) {
}
//=================================================================
void PlanarityTest::delNode(Graph *graph,const node) {
  if (resultsBuffer.find((unsigned long)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned long)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
void PlanarityTest::destroy(Graph *graph) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned long)graph);
}
//=================================================================
