//-*-c++-*-

#include <float.h>
#include "tulip/DoubleProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/DoubleAlgorithm.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//==============================
///Constructeur d'un DoubleProperty
DoubleProperty::DoubleProperty (Graph *sg):AbstractProperty<DoubleType,DoubleType,DoubleAlgorithm>(sg),
  minMaxOkNode(false),minMaxOkEdge(false) {
  //  propertyProxy=this;
}
//==============================
///Destructeur d'un metric   
///Vide pour l'instant, cela peut
///provoquer des fuites m�moires
DoubleProperty::~DoubleProperty() {
  notifyDestroy();
}    

void DoubleProperty::uniformQuantification(unsigned int k) {
  std::map<double,double> nodeMapping;
  std::map<double,double> edgeMapping;

  //===============================================================
  //build the histogram of node values
  {
    map<double,int> histogram;
    Iterator<node> *itN=graph->getNodes();
    while (itN->hasNext()) {
	  node itn=itN->next();
      double nodeValue=getNodeValue(itn);
      if (histogram.find(nodeValue)==histogram.end()) 
	histogram[nodeValue]=1;
      else
	histogram[nodeValue]+=1;
    } delete itN;
    //===============================================================
    //Build the color map
    map<double,int>::iterator it;
    double sum=0;
    double cK=double(graph->numberOfNodes())/double(k);
    int k2=0;
    for (it=histogram.begin();it!=histogram.end();++it) {
      sum+=(*it).second;
      nodeMapping[(*it).first]=k2;
      while (sum>cK*double(k2+1)) ++k2;
    }
  }
  //===============================================================
  //build the histogram of edges values
  {
    map<double,int> histogram;
    Iterator<edge> *itE=graph->getEdges();
    while (itE->hasNext()) {
      edge ite=itE->next();
      double value=getEdgeValue(ite);
      if (histogram.find(value)==histogram.end()) 
	histogram[value]=1;
      else
	histogram[value]+=1;
    } delete itE;
    //===============================================================
    //Build the color map
    map<double,int>::iterator it;
    double sum=0;
    double cK=double(graph->numberOfEdges())/double(k);
    int k2=0;
    for (it=histogram.begin();it!=histogram.end();++it) {
      sum+=(*it).second;
      edgeMapping[(*it).first]=k2;
      while (sum>cK*double(k2+1)) ++k2;
    }
  }

  Iterator<node> *itN=graph->getNodes();
  while(itN->hasNext()) {
    node itn=itN->next();
    setNodeValue(itn,nodeMapping[getNodeValue(itn)]);
  } delete itN;
  Iterator<edge> *itE=graph->getEdges();
  while(itE->hasNext()) {
    edge ite=itE->next();
    setEdgeValue(ite,edgeMapping[getEdgeValue(ite)]);
  } delete itE;
}

//====================================================================
///Renvoie le minimum de la metrique associe aux noeuds du DoubleProperty
double DoubleProperty::getNodeMin(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned long sgi=(unsigned long)sg;
  if (minMaxOkNode.find(sgi)==minMaxOkNode.end()) minMaxOkNode[sgi]=false;
  if (!minMaxOkNode[sgi]) computeMinMaxNode(sg);
  return minN[sgi];

  //  if (!minMaxOkNode) computeMinMaxNode(sg);
  //  return minN;
}
//====================================================================
///Renvoie le maximum de la metrique associe aux noeuds du DoubleProperty
double DoubleProperty::getNodeMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned long sgi=(unsigned long)sg;
  if (minMaxOkNode.find(sgi)==minMaxOkNode.end()) minMaxOkNode[sgi]=false;
  if (!minMaxOkNode[sgi]) computeMinMaxNode(sg);
  return maxN[sgi];

  //if (!minMaxOkNode) computeMinMaxNode(sg);
  //return maxN;
}
//====================================================================
///Renvoie le Minimum de la metrique associe aux aretes du DoubleProperty
double DoubleProperty::getEdgeMin(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned long sgi=(unsigned long)sg;
  if (minMaxOkEdge.find(sgi)==minMaxOkEdge.end()) minMaxOkEdge[sgi]=false;
  if (!minMaxOkEdge[sgi]) computeMinMaxEdge(sg);
  return minE[sgi];
}
//====================================================================
///Renvoie le Maximum de la metrique associe aux aretes du DoubleProperty
double DoubleProperty::getEdgeMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned long sgi=(unsigned long)sg;
  if (minMaxOkEdge.find(sgi)==minMaxOkEdge.end()) minMaxOkEdge[sgi]=false;
  if (!minMaxOkEdge[sgi]) computeMinMaxEdge(sg);
  return maxE[sgi];
}
//=========================================================
void DoubleProperty::computeMinMaxNode(Graph *sg) {
  double tmp;
  double maxN2,minN2;
  if (sg==0) sg=graph;
  Iterator<node> *itN=sg->getNodes();
  if (itN->hasNext()) {
    node itn=itN->next();
    tmp=getNodeValue(itn);
    maxN2=tmp;
    minN2=tmp;
  }
  while (itN->hasNext()) {
    node itn=itN->next();
    tmp=getNodeValue(itn);
    if (tmp>maxN2) maxN2=tmp;
    if (tmp<minN2) minN2=tmp;
  } delete itN;

  unsigned long sgi=(unsigned long)sg;

  minMaxOkNode[sgi]=true;  
  minN[sgi]=minN2;
  maxN[sgi]=maxN2;
}
//=========================================================
void DoubleProperty::computeMinMaxEdge(Graph *sg) {
  double tmp;
  double maxE2,minE2;
  if (sg==0) sg=graph;
  Iterator<edge> *itE=sg->getEdges();
  if (itE->hasNext()) {
    edge ite=itE->next();
    tmp=getEdgeValue(ite);
    maxE2=tmp;
    minE2=tmp;
  }
  while (itE->hasNext()) {
    edge ite=itE->next();
    tmp=getEdgeValue(ite);
    if (tmp>maxE2) maxE2=tmp;
    if (tmp<minE2) minE2=tmp;
  } delete itE;

  unsigned long sgi=(unsigned long)sg;

  minMaxOkEdge[sgi]=true;
  minE[sgi]=minE2;
  maxE[sgi]=maxE2;
}
//=============================================================================
///Poign� permettnet le reset des variables du DoubleProperty par le AbstractProperty
void DoubleProperty::reset_handler() {
  minMaxOkNode.clear();
  minMaxOkEdge.clear();
}
//=============================================================================
///Poign� permettnet le recompute il faut absolument mettre le sgProperties
///currentMetricProxy � this sinon le r�sultat est impr�visible
void DoubleProperty::recompute_handler() {
  //  graph->getPropertyManager()->currentAbstractProperty=this;
  minMaxOkNode.clear();
  minMaxOkEdge.clear();
}
//=================================================================================
void DoubleProperty::clone_handler(AbstractProperty<DoubleType,DoubleType> &proxyC) {
  DoubleProperty *proxy=(DoubleProperty *)&proxyC;
  minMaxOkNode=proxy->minMaxOkNode;
  minMaxOkEdge=proxy->minMaxOkEdge;
  minN=proxy->minN;
  maxN=proxy->maxN;
  minE=proxy->minE;
  maxE=proxy->maxE;
}
//=================================================================================
void DoubleProperty::setNodeValue_handler(const node n, const DoubleType::RealType &) {
  minMaxOkNode.clear();
}
void DoubleProperty::setEdgeValue_handler(const edge e, const DoubleType::RealType &) {
  minMaxOkEdge.clear();
}
void DoubleProperty::setAllNodeValue_handler(const DoubleType::RealType &) {
  minMaxOkNode.clear();
}
void DoubleProperty::setAllEdgeValue_handler(const DoubleType::RealType &) {
  minMaxOkEdge.clear();
}

//=================================================================================
PropertyInterface* DoubleProperty::clonePrototype(Graph * g, std::string n) {
  if( !g )
    return 0;
  DoubleProperty * p = g->getLocalProperty<DoubleProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=============================================================
void DoubleProperty::copy( const node n0, const node n1, PropertyInterface * p ) {
  if( !p )
    return;
  DoubleProperty * tp = dynamic_cast<DoubleProperty*>(p);
  assert( tp );
  setNodeValue( n0, tp->getNodeValue(n1) );
}
//=============================================================
void DoubleProperty::copy( const edge e0, const edge e1, PropertyInterface * p ) {
  if( !p )
    return;
  DoubleProperty * tp = dynamic_cast<DoubleProperty*>(p);
  assert( tp );
  setEdgeValue( e0, tp->getEdgeValue(e1) );
}
//=============================================================

