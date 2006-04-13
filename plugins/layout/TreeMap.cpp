#include <cmath>
#include <tulip/TreeTest.h>
#include "TreeMap.h"

LAYOUTPLUGIN(TreeMap,"Tree Map (Shneiderman)","David Auber","01/12/1999","ok","0","1");

using namespace std;

void TreeMap::dfsPlacement(node n, int depth, double x, double y, double width, double height,
			   bool direction, stdext::hash_map<node,double> &value) {
  //Affecte la valeur du noeud courant
  layoutResult->setNodeValue(n,Coord(x+(double)width/2,y+(double)height/2,depth));
  size->setNodeValue(n,Size(width,height,1));
  if (superGraph->outdeg(n)==0) return;
  
  //Calcul la somme des valeurs des fils.
  double tmpSomme=value[n];

  x+=1;
  y+=1;
  width-=1;
  height-=1;
  double delta;
  double newX=x,newY=y;
  double newWidth=width,newHeight=height;
  
  Iterator<node> *itN=superGraph->getOutNodes(n);
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

TreeMap::TreeMap(const PropertyContext &context):LayoutAlgorithm(context){}

TreeMap::~TreeMap() {}

double TreeMap::initVal(node n, stdext::hash_map<node,double> &value) {
  if (superGraph->outdeg(n)==0) { 
    if (!(value[n]=metric->getNodeValue(n)>0)) value[n]=1;
    return value[n];
  }
  double sum=0;
  Iterator<node> *itN=superGraph->getOutNodes(n);
  while (itN->hasNext()) {
    node itn=itN->next();
    sum+=initVal(itn,value);
  } delete itN;
  value[n]=sum;
  return sum;
}

bool TreeMap::run() {
  metric=superGraph->getProperty<Metric>("viewMetric");
  size=superGraph->getLocalProperty<Sizes>("viewSize");
  stdext::hash_map<node,double> value(superGraph->numberOfNodes());

  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (superGraph->indeg(itn)==0) {
      initVal(itn,value);
      dfsPlacement(itn,1,0,0,1024,1024,true,value);
      break;
    }
  } delete itN;
  return true;
}

bool TreeMap::check(string &erreurMsg) {
  if (TreeTest::isTree(superGraph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The Graph must be a Tree";
    return false;
  }
}

void TreeMap::reset()
{}









