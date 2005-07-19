#include <tulip/SuperGraph.h>
#include <tulip/PlanarityTest.h>
#include <tulip/PlanarityTestImpl.h>

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
bool PlanarityTest::isPlanar(SuperGraph *graph){
  if(instance==0)
    instance=new PlanarityTest();
  return instance->compute(graph);
}
//=================================================================
bool PlanarityTest::compute(SuperGraph *graph) { 
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end()) 
    return resultsBuffer[(unsigned int)graph];
  PlanarityTestImpl planarTest(graph);
  resultsBuffer[(unsigned int)graph] = planarTest.isPlanar();
  graph->addObserver(this);
  return resultsBuffer[(unsigned int)graph];
}
//=================================================================
void PlanarityTest::addEdge(SuperGraph *graph,const edge) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned int)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void PlanarityTest::delEdge(SuperGraph *graph,const edge) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned int)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void PlanarityTest::reverseEdge(SuperGraph *graph,const edge) {
}
//=================================================================
void PlanarityTest::addNode(SuperGraph *graph,const node) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned int)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void PlanarityTest::delNode(SuperGraph *graph,const node) {
  if (resultsBuffer.find((unsigned int)graph)!=resultsBuffer.end())
    if (resultsBuffer[(unsigned int)graph]) return;
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
void PlanarityTest::destroy(SuperGraph *graph) {
  graph->removeObserver(this);
  resultsBuffer.erase((unsigned int)graph);
}
//=================================================================
