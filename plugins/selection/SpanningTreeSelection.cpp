#include <assert.h>
#include "SpanningTreeSelection.h"
#include <tulip/BooleanProperty.h>
#include <tulip/MethodFactory.h>
#include <tulip/GraphTools.h>

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
  if (graph->existProperty("viewSelection")) {
    BooleanProperty *viewSelection=graph->getProperty<BooleanProperty>("viewSelection");
    Iterator<node> *itN=graph->getNodes();
    for (;itN->hasNext();) { 
      node itn=itN->next();
      if (viewSelection->getNodeValue(itn)==true) {
	booleanResult->setNodeValue(itn, true);
      }
    } delete itN;
  }

  selectSpanningForest(graph, booleanResult); 
  return true;
}
