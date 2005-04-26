#include <cassert>

#include <tulip/SelectionProxy.h>
#include <tulip/MethodFactory.h>

#include "SpanningDagSelection.h"

SELECTIONPLUGIN(SpanningDagSelection,"SpanningDag","David Auber","01/12/1999","Alpha","0","1");

using namespace std;

SpanningDagSelection::SpanningDagSelection(const PropertyContext &context):Selection(context) {}

SpanningDagSelection::~SpanningDagSelection() {}

void  SpanningDagSelection::dfsAcyclicTest(const node &n,SelectionProxy *visited,SelectionProxy *finished) {
  visited->setNodeValue(n,true);
  Iterator<edge> *it=superGraph->getOutEdges(n);
  while (it->hasNext()) {
    edge tmp=it->next();
    if ((visited->getNodeValue(superGraph->target(tmp)))==true) {
      if ((finished->getNodeValue(superGraph->target(tmp)))==true) 
	selectionProxy->setEdgeValue(tmp,true);
    }
    else {
      selectionProxy->setEdgeValue(tmp,true);
      dfsAcyclicTest(superGraph->target(tmp),visited,finished);
    }
  } delete it;
  finished->setNodeValue(n,true);
}

bool SpanningDagSelection::run() {
  SelectionProxy *visited=superGraph->getLocalProperty<SelectionProxy>("SpanningDagSelectionAcyclicTestVisited");
  SelectionProxy *finished=superGraph->getLocalProperty<SelectionProxy>("SpanningDagSelectionAcyclicTestFinished");
  SelectionProxy *parameter=superGraph->getProperty<SelectionProxy>("viewSelection");

  visited->setAllNodeValue(false);
  finished->setAllNodeValue(false);

  Iterator<node> *it=superGraph->getNodes();
  while (it->hasNext()) {
      node curNode=it->next();
      if ((!visited->getNodeValue(curNode)) && (parameter->getNodeValue(curNode)) ) {
	dfsAcyclicTest(curNode,visited,finished);
      }
  } delete it;

  //On regarde pour les sommets restant non traité par la selection passée en pamaètre
  it=superGraph->getNodes();
  while (it->hasNext()) {
    node curNode=it->next();
    if (!visited->getNodeValue(curNode)) {
      dfsAcyclicTest(curNode,visited,finished);
    }
  } delete it;
  
  selectionProxy->setAllNodeValue(true);
  superGraph->delLocalProperty("SpanningDagSelectionAcyclicTestVisited");
  superGraph->delLocalProperty("SpanningDagSelectionAcyclicTestFinished");
  return true;
}

bool SpanningDagSelection::check(string &erreurMsg) {
  erreurMsg="";
  return (true);
}

void SpanningDagSelection::reset() {
}
