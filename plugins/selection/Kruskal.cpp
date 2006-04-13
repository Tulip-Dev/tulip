#include "Kruskal.h"

SELECTIONPLUGIN(Kruskal,"Kruskal","Anthony DON","14/04/03","Alpha","0","1");

using namespace std;

namespace {
  const char * paramHelp[] = {
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Metric" ) \
    HTML_HELP_DEF( "default", "\"viewMetric\"" ) \
    HTML_HELP_BODY() \
    "This parameter defines the metric used for edges weight." \
    HTML_HELP_CLOSE(),
  };
}
//======================================================
int Kruskal::getClass(const int i) {
  return (*classes)[i];
}
//======================================================
int Kruskal::makeUnion(const int p, const int q) {
  int x = getClass(p);
  int y = getClass(q);

  Iterator<node> *itN = superGraph->getNodes();
  while (itN->hasNext()) { 
    node n=itN->next();
    if(getClass(n.id) == y)
      (*classes)[n.id] = x;
  } delete itN;
  numClasses--;
  
}
//======================================================
bool Kruskal::edgeOk(const edge &e) {
  return (getClass(superGraph->source(e).id) !=  getClass(superGraph->target(e).id));
}
//======================================================
Kruskal::Kruskal(const PropertyContext &context):SelectionAlgorithm(context) {
  addParameter<Metric> ("Edge weight", paramHelp[0], "viewMetric");
}
//======================================================
Kruskal::~Kruskal() {
}
//======================================================
#include <tulip/ConnectedTest.h>
bool Kruskal::check(string &erreurMsg) {
  if (ConnectedTest::isConnected(superGraph)) {
    erreurMsg = "";
    return true;
  }
  else {
    erreurMsg = "The Graph must be connected";
    return false;
  }
}
//======================================================
///Calcul l'arbre couvrant minimal
bool Kruskal::run(){
  /* Initialisation */

  int nNodes = superGraph->numberOfNodes();
  numClasses = nNodes;
  classes = new map<int, int>;
  
  int classNumber = 0;
  Iterator<node> *itN = superGraph->getNodes();
  while (itN->hasNext()) { 
    node n=itN->next();
    (*classes)[n.id] = classNumber;
    classNumber++;
  }delete itN;

  std::list<edge> sortedEdges;
  Iterator<edge> *itE = superGraph->getEdges();
  while (itE->hasNext()) { 
    edge e=itE->next();
    sortedEdges.push_back(e);
  } delete itE;
  
  selectionObj->setAllNodeValue(true);
  selectionObj->setAllEdgeValue(false);

  Metric *edgeWeight = 0;
  if ( dataSet!=0) {
    dataSet->get("Edge Weight", edgeWeight);
  }
  if (edgeWeight == 0)
    edgeWeight = superGraph->getProperty<Metric>("viewMetric");
  /* Calcul */
  sortedEdges.sort<ltEdge>(ltEdge(edgeWeight));
  while(numClasses > 1) {
    edge cur;
    do {
      cur = sortedEdges.front();
      sortedEdges.pop_front();
    } while(! edgeOk(cur));
    
    selectionObj->setEdgeValue(cur, true);
    makeUnion(superGraph->source(cur).id, superGraph->target(cur).id);
  }
  delete classes;
  return true;
}
//=======================================================================
