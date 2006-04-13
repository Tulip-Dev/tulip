#include <assert.h>
#include "SpanningTreeSelection.h"
#include <tulip/Selection.h>
#include <tulip/MethodFactory.h>

SELECTIONPLUGIN(SpanningTreeSelection,"Spanning Forest","David Auber","01/12/1999","Alpha","0","1");

using namespace std;

SpanningTreeSelection::SpanningTreeSelection(const PropertyContext &context):SelectionAlgorithm(context) 
{}

SpanningTreeSelection::~SpanningTreeSelection() {}

///===========================================================
///Calcul l'arbre couvrant
bool SpanningTreeSelection::run()
{
  list<node> fifo;

  Selection *nodeFlag=superGraph->getLocalProperty<Selection>("viewSelectionNodeFlag");
  if (superGraph->existProperty("viewSelection")) {
    Selection *viewSelection=superGraph->getProperty<Selection>("viewSelection");
    Iterator<node> *itN=superGraph->getNodes();
    for (;itN->hasNext();) { 
      node itn=itN->next();
      if (viewSelection->getNodeValue(itn)==true) {
	fifo.push_back(itn);
	nodeFlag->setNodeValue(itn,true);
      }
    } delete itN;
  }

  
  selectionObj->setAllEdgeValue(true);
  selectionObj->setAllNodeValue(true);

  bool ok=true;
  node tmp1;
  while (ok) {
    while (!fifo.empty()) {
      tmp1=fifo.front();
      fifo.pop_front();
      Iterator<edge> *itE=superGraph->getOutEdges(tmp1);
      for(;itE->hasNext();) {
	edge adjit=itE->next();
	if (!nodeFlag->getNodeValue(superGraph->target(adjit)))	{
	  nodeFlag->setNodeValue(superGraph->target(adjit),true);	    
	  fifo.push_back(superGraph->target(adjit));
	}
	else
	  selectionObj->setEdgeValue(adjit,false);
      } delete itE;
    }
    ok=false;
    bool degZ=false;
    node goodNode;
    Iterator<node> *itN=superGraph->getNodes();
    for(;itN->hasNext();) {
      node itn=itN->next();
      if (!nodeFlag->getNodeValue(itn)) {
	if (!ok) {
	  goodNode=itn;
	  ok=true;
	}
	if (superGraph->indeg(itn)==0) {
	  fifo.push_back(itn);
	  nodeFlag->setNodeValue(itn,true);
	  degZ=true;
	}
	if (!degZ) {
	  if (superGraph->indeg(itn)<superGraph->indeg(goodNode))
	    goodNode=itn;
	  else {
	    if (superGraph->indeg(itn)==superGraph->indeg(goodNode))
	      if (superGraph->outdeg(itn)>superGraph->outdeg(goodNode))
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
  superGraph->delLocalProperty("viewSelectionNodeFlag");
  return true;
}





