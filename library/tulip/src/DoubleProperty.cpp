/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <float.h>
#include "tulip/ForEach.h"
#include "tulip/DoubleProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/DoubleAlgorithm.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//=============================================================
// Predefined Meta Value Calculators
//=============================================================
typedef void (*DoubleNodePredefinedCalculator) (AbstractDoubleProperty* metric,
						node mN, Graph* sg);

typedef void (*DoubleEdgePredefinedCalculator) (AbstractDoubleProperty* metric, edge mE,
						Iterator<edge>* itE);

// average values
static void computeNodeAvgValue(AbstractDoubleProperty* metric, node mN, Graph* sg) {
  double value = 0;
  unsigned int nbNodes = 0;
  node n;
  forEach(n, sg->getNodes()) {
    ++nbNodes;
    value += metric->getNodeValue(n);
  }
  metric->setNodeValue(mN, value/nbNodes);
}

static void computeEdgeAvgValue(AbstractDoubleProperty* metric, edge mE, Iterator<edge>* itE) {
  double value = 0;
  unsigned int nbEdges = 0;
  while(itE->hasNext()) {
    edge e = itE->next();
    ++nbEdges;
    value += metric->getEdgeValue(e);
  }
  metric->setEdgeValue(mE, value/nbEdges);
}

// sum values
static void computeNodeSumValue(AbstractDoubleProperty* metric, node mN, Graph* sg) {
  double value = 0;
  node n;
  forEach(n, sg->getNodes()) {
    value += metric->getNodeValue(n);
  }
  metric->setNodeValue(mN, value);
}

static void computeEdgeSumValue(AbstractDoubleProperty* metric, edge mE, Iterator<edge>* itE) {
  double value = 0;
  while(itE->hasNext()) {
    edge e = itE->next();
    value += metric->getEdgeValue(e);
  }
  metric->setEdgeValue(mE, value);
}

// max values
static void computeNodeMaxValue(AbstractDoubleProperty* metric, node mN, Graph* sg) {
  double value = -DBL_MAX;
  node n;
  forEach(n, sg->getNodes()) {
    const double& nVal = metric->getNodeValue(n);
    if (nVal > value)
      value = nVal;
  }
  metric->setNodeValue(mN, value);
}

static void computeEdgeMaxValue(AbstractDoubleProperty* metric, edge mE, Iterator<edge>* itE) {
  double value = -DBL_MAX;
  while(itE->hasNext()) {
    const double& eVal = metric->getEdgeValue(itE->next());
    if (eVal > value)
      value = eVal;
  }
  metric->setEdgeValue(mE, value);
}

// min values
static void computeNodeMinValue(AbstractDoubleProperty* metric, node mN, Graph* sg) {
  double value = DBL_MAX;
  node n;
  forEach(n, sg->getNodes()) {
    const double& nVal = metric->getNodeValue(n);
    if (nVal < value)
      value = nVal;
  }
  metric->setNodeValue(mN, value);
}

static void computeEdgeMinValue(AbstractDoubleProperty* metric, edge mE, Iterator<edge>* itE) {
  double value = DBL_MAX;
  while(itE->hasNext()) {
    const double& eVal = metric->getEdgeValue(itE->next());
    if (eVal < value)
      value = eVal;
  }
  metric->setEdgeValue(mE, value);
}

// 2 arrays to hold the predefined functions
DoubleNodePredefinedCalculator nodeCalculators[] = {
  NULL,
  computeNodeAvgValue,
  computeNodeSumValue,
  computeNodeMaxValue,
  computeNodeMinValue
};

DoubleEdgePredefinedCalculator edgeCalculators[] = {
  NULL,
  computeEdgeAvgValue,
  computeEdgeSumValue,
  computeEdgeMaxValue,
  computeEdgeMinValue
};

class DoublePropertyPredefinedCalculator :public AbstractDoubleProperty::MetaValueCalculator {
  DoubleNodePredefinedCalculator nodeCalc;
  DoubleEdgePredefinedCalculator edgeCalc;

public:
  DoublePropertyPredefinedCalculator(DoubleProperty::PredefinedMetaValueCalculator nCalc =
				     DoubleProperty::AVG_CALC,
				     DoubleProperty::PredefinedMetaValueCalculator eCalc =
				     DoubleProperty::AVG_CALC)
    :AbstractDoubleProperty::MetaValueCalculator(),
     nodeCalc(nodeCalculators[(int) nCalc]),
     edgeCalc(edgeCalculators[(int) eCalc]) {}

  void computeMetaValue(AbstractDoubleProperty* metric, node mN, Graph* sg,
			Graph*) {
    if (nodeCalc)
      nodeCalc(metric, mN, sg);
  }

  void computeMetaValue(AbstractDoubleProperty* metric, edge mE,
			Iterator<edge>* itE, Graph*) {
    if (edgeCalc)
      edgeCalc(metric, mE, itE);
  }
};

// default calculator
static DoublePropertyPredefinedCalculator avgCalculator;

//==============================
///Constructeur d'un DoubleProperty
DoubleProperty::DoubleProperty (Graph *sg, std::string n):AbstractProperty<DoubleType,DoubleType,DoubleAlgorithm>(sg, n),
  minMaxOkNode(false),minMaxOkEdge(false) {
  // the property observes itself; see beforeSet... methods
  addPropertyObserver(this);
  // but do not need to be in observables
  removeObservable(this);
  // the computed meta value will be the average value
  setMetaValueCalculator(&avgCalculator);
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
      const double& nodeValue=getNodeValue(itn);
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
      const double& value=getEdgeValue(ite);
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
  unsigned int sgi = sg->getId();
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
  unsigned int sgi = sg->getId();
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
  unsigned int sgi = sg->getId();
  if (minMaxOkEdge.find(sgi)==minMaxOkEdge.end()) minMaxOkEdge[sgi]=false;
  if (!minMaxOkEdge[sgi]) computeMinMaxEdge(sg);
  return minE[sgi];
}
//====================================================================
///Renvoie le Maximum de la metrique associe aux aretes du DoubleProperty
double DoubleProperty::getEdgeMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOkEdge.find(sgi)==minMaxOkEdge.end()) minMaxOkEdge[sgi]=false;
  if (!minMaxOkEdge[sgi]) computeMinMaxEdge(sg);
  return maxE[sgi];
}
//=========================================================
void DoubleProperty::computeMinMaxNode(Graph *sg) {
  double maxN2,minN2;
  if (sg==0) sg=graph;
  Iterator<node> *itN=sg->getNodes();
  if (itN->hasNext()) {
    node itn=itN->next();
    const double& tmp=getNodeValue(itn);
    maxN2=tmp;
    minN2=tmp;
  }
  while (itN->hasNext()) {
    node itn=itN->next();
    const double& tmp=getNodeValue(itn);
    if (tmp>maxN2) maxN2=tmp;
    if (tmp<minN2) minN2=tmp;
  } delete itN;

  unsigned int sgi = sg->getId();

  minMaxOkNode[sgi]=true;  
  minN[sgi]=minN2;
  maxN[sgi]=maxN2;
}
//=========================================================
void DoubleProperty::computeMinMaxEdge(Graph *sg) {
  double maxE2,minE2;
  if (sg==0) sg=graph;
  Iterator<edge> *itE=sg->getEdges();
  if (itE->hasNext()) {
    edge ite=itE->next();
    const double& tmp=getEdgeValue(ite);
    maxE2=tmp;
    minE2=tmp;
  }
  while (itE->hasNext()) {
    edge ite=itE->next();
    const double& tmp=getEdgeValue(ite);
    if (tmp>maxE2) maxE2=tmp;
    if (tmp<minE2) minE2=tmp;
  } delete itE;

  unsigned int sgi = sg->getId();

  minMaxOkEdge[sgi]=true;
  minE[sgi]=minE2;
  maxE[sgi]=maxE2;
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
void DoubleProperty::beforeSetNodeValue(PropertyInterface*, const node) {
  minMaxOkNode.clear();
}
void DoubleProperty::beforeSetEdgeValue(PropertyInterface*, const edge) {
  minMaxOkEdge.clear();
}
void DoubleProperty::beforeSetAllNodeValue(PropertyInterface*) {
  minMaxOkNode.clear();
}
void DoubleProperty::beforeSetAllEdgeValue(PropertyInterface*) {
  minMaxOkEdge.clear();
}
//=================================================================================
PropertyInterface* DoubleProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  DoubleProperty * p = g->getLocalProperty<DoubleProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=================================================================================
PropertyInterface* DoubleVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  DoubleVectorProperty * p = g->getLocalProperty<DoubleVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=============================================================
void DoubleProperty::setMetaValueCalculator(PredefinedMetaValueCalculator nodeCalc,
					    PredefinedMetaValueCalculator edgeCalc) {
  setMetaValueCalculator(new DoublePropertyPredefinedCalculator(nodeCalc, edgeCalc));
}
//=============================================================
void DoubleProperty::setMetaValueCalculator(MetaValueCalculator* calc) {
  if (metaValueCalculator && metaValueCalculator != &avgCalculator &&
      typeid(metaValueCalculator) == typeid(DoublePropertyPredefinedCalculator))
    delete metaValueCalculator;
  metaValueCalculator = calc;
}

