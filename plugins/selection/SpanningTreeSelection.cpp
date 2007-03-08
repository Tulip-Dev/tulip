#include <assert.h>
#include "SpanningTreeSelection.h"
#include <tulip/BooleanProperty.h>
#include <tulip/MethodFactory.h>

BOOLEANPLUGIN(SpanningTreeSelection,"Spanning Forest","David Auber","01/12/1999","Alpha","1.0");

using namespace std;
using namespace tlp;

SpanningTreeSelection::SpanningTreeSelection(const PropertyContext &context):BooleanAlgorithm(context) 
{}

SpanningTreeSelection::~SpanningTreeSelection() {}

///===========================================================
///Calcul l'arbre couvrant
bool SpanningTreeSelection::run()
{
  list<node> fifo;

  BooleanProperty *nodeFlag=graph->getLocalProperty<BooleanProperty>("viewSelectionNodeFlag");
  if (graph->existProperty("viewSelection")) {
    BooleanProperty *viewSelection=graph->getProperty<BooleanProperty>("viewSelection");
    Iterator<node> *itN=graph->getNodes();
    for (;itN->hasNext();) { 
      node itn=itN->next();
      if (viewSelection->getNodeValue(itn)==true) {
	fifo.push_back(itn);
	nodeFlag->setNodeValue(itn,true);
      }
    } delete itN;
  }

  
  booleanResult->setAllEdgeValue(true);
  booleanResult->setAllNodeValue(true);

  bool ok=true;
  node tmp1;
  while (ok) {
    while (!fifo.empty()) {
      tmp1=fifo.front();
      fifo.pop_front();
      Iterator<edge> *itE=graph->getOutEdges(tmp1);
      for(;itE->hasNext();) {
	edge adjit=itE->next();
	if (!nodeFlag->getNodeValue(graph->target(adjit)))	{
	  nodeFlag->setNodeValue(graph->target(adjit),true);	    
	  fifo.push_back(graph->target(adjit));
	}
	else
	  booleanResult->setEdgeValue(adjit,false);
      } delete itE;
    }
    ok=false;
    bool degZ=false;
    node goodNode;
    Iterator<node> *itN=graph->getNodes();
    for(;itN->hasNext();) {
      node itn=itN->next();
      if (!nodeFlag->getNodeValue(itn)) {
	if (!ok) {
	  goodNode=itn;
	  ok=true;
	}
	if (graph->indeg(itn)==0) {
	  fifo.push_back(itn);
	  nodeFlag->setNodeValue(itn,true);
	  degZ=true;
	}
	if (!degZ) {
	  if (graph->indeg(itn)<graph->indeg(goodNode))
	    goodNode=itn;
	  else {
	    if (graph->indeg(itn)==graph->indeg(goodNode))
	      if (graph->outdeg(itn)>graph->outdeg(goodNode))
		goodNode=itn;
	  }
	}
      }
    } delete itN;
    if (ok && (!degZ)) {
      fifo.push_back(goodNode);
      nodeFlag->setNodeValue(goodNode,true);
    }
  }
  graph->delLocalProperty("viewSelectionNodeFlag");
  return true;
}





