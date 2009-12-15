#include <cmath>
#include <tulip/TreeTest.h>
#include "TreeMap.h"

LAYOUTPLUGINOFGROUP(TreeMap,"Tree Map (Shneiderman)","David Auber","01/12/1999","ok","1.1","Tree");

using namespace std;
using namespace tlp;

static const char * paramHelp[] = 
    {
      // metric :
      HTML_HELP_OPEN() \
      HTML_HELP_DEF( "type", "Metric" ) \
      HTML_HELP_DEF( "values", "An existing metric property" ) \
      HTML_HELP_DEF( "default", "viewMetric if it exists" ) \
      HTML_HELP_BODY() \
      "This parameter defines the metric used to estimate the size allocated to each node." \
      HTML_HELP_CLOSE(),
    };


void TreeMap::dfsPlacement(node n, int depth, double x, double y, double width, double height,
			   bool direction, TLP_HASH_MAP<node,double> &value) {
  //Affecte la valeur du noeud courant
  layoutResult->setNodeValue(n,Coord(x+(double)width/2,y+(double)height/2,depth));
  size->setNodeValue(n,Size(width,height,1));
  if (graph->outdeg(n)==0) return;
  
  //Calcul la somme des valeurs des fils.
  double tmpSomme=value[n];

  x+=1;
  y+=1;
  width-=1;
  height-=1;
  double delta;
  double newX=x,newY=y;
  double newWidth=width,newHeight=height;
  
  Iterator<node> *itN=graph->getOutNodes(n);
  //interpolation horizontale
  if (direction) {
    delta=width/tmpSomme;
    while (itN->hasNext()) {
      node itn=itN->next();
      newWidth=delta*value[itn];
      dfsPlacement(itn,depth+4,newX,newY,newWidth,newHeight,false,value);
      newX+=delta*value[itn];
    }
  }
  else { //interpolation vertical
    while (itN->hasNext()) {
      node itn=itN->next();
      delta=height/tmpSomme;
      newHeight=delta*value[itn];
      dfsPlacement(itn,depth+4,newX,newY,newWidth,newHeight,true,value);
      newY+=delta*value[itn];
    }
  }delete itN;
}

TreeMap::TreeMap(const PropertyContext &context):LayoutAlgorithm(context){
  addParameter<DoubleProperty>("metric", paramHelp[0], 0, false);
}

TreeMap::~TreeMap() {}

double TreeMap::initVal(node n, TLP_HASH_MAP<node,double> &value) {
  if (graph->outdeg(n)==0) { 
    if (!((value[n] = metric ? metric->getNodeValue(n) : 1) >0))
      value[n]=1;
    return value[n];
  }
  double sum=0;
  Iterator<node> *itN=graph->getOutNodes(n);
  while (itN->hasNext()) {
    node itn=itN->next();
    sum+=initVal(itn,value);
  } delete itN;
  value[n]=sum;
  return sum;
}

bool TreeMap::run() {
  size = graph->getLocalProperty<SizeProperty>("viewSize");
  // ensure size updates will be kept after a pop
  preservePropertyUpdates(size);

  TLP_HASH_MAP<node,double> value(graph->numberOfNodes());

  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (graph->indeg(itn)==0) {
      initVal(itn,value);
      dfsPlacement(itn,1,0,0,1024,1024,true,value);
      break;
    }
  } delete itN;
 
  return true;
}

bool TreeMap::check(string &errorMsg) {
  if (!TreeTest::isTree(graph)) {
    errorMsg = "The Graph must be a Tree";
    return false;
  }
    
  metric = NULL;
  if (dataSet != 0)
    dataSet->get("metric", metric);
      
  if (!metric && graph->existProperty("viewMetric")) {
    metric = graph->getProperty<DoubleProperty>("viewMetric");
  }
  errorMsg = "";
  return true;
}






