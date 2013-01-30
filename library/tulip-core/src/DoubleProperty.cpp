/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <climits>
#include <tulip/ForEach.h>
#include <tulip/DoubleProperty.h>

using namespace std;
using namespace tlp;

const string DoubleProperty::propertyTypename="double";
const string DoubleVectorProperty::propertyTypename="vector<double>";

//=============================================================
// Predefined Meta Value Calculators
//=============================================================
typedef void (*DoubleNodePredefinedCalculator) (AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric,
    node mN, Graph* sg);

typedef void (*DoubleEdgePredefinedCalculator) (AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, edge mE,
    Iterator<edge>* itE);

// average values
static void computeNodeAvgValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, node mN, Graph* sg) {
  // nothing to do if the subgraph is not linked to the property graph
  if (sg!=metric->getGraph() && !metric->getGraph()->isDescendantGraph(sg)) {
#ifndef NDEBUG
    tlp::warning() << "Warning : " << __PRETTY_FUNCTION__ << " does not compute any value for a subgraph not linked to the graph of the property " << metric->getName().c_str();
#endif
    return;
  }

  double value = 0;
  unsigned int nbNodes = 0;
  node n;
  forEach(n, sg->getNodes()) {
    ++nbNodes;
    value += metric->getNodeValue(n);
  }

  if (nbNodes)
    metric->setNodeValue(mN, value/nbNodes);
}

static void computeEdgeAvgValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, edge mE, Iterator<edge>* itE) {
  double value = 0;
  unsigned int nbEdges = 0;

  while(itE->hasNext()) {
    edge e = itE->next();
    ++nbEdges;
    value += metric->getEdgeValue(e);
  }

  if (nbEdges)
    metric->setEdgeValue(mE, value/nbEdges);
}

// sum values
static void computeNodeSumValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, node mN, Graph* sg) {
  // nothing to do if the subgraph is not linked to the property graph
  if (sg!=metric->getGraph() && !metric->getGraph()->isDescendantGraph(sg)) {
#ifndef NDEBUG
    tlp::warning() << "Warning : " << __PRETTY_FUNCTION__ << " does not compute any value for a subgraph not linked to the graph of the property " << metric->getName().c_str();
#endif
    return;
  }

  double value = 0;
  node n;
  forEach(n, sg->getNodes()) {
    value += metric->getNodeValue(n);
  }
  metric->setNodeValue(mN, value);
}

static void computeEdgeSumValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, edge mE, Iterator<edge>* itE) {
  double value = 0;

  while(itE->hasNext()) {
    edge e = itE->next();
    value += metric->getEdgeValue(e);
  }

  metric->setEdgeValue(mE, value);
}

// max values
static void computeNodeMaxValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, node mN, Graph* sg) {
  // nothing to do if the subgraph is not linked to the property graph
  if (sg!=metric->getGraph() && !metric->getGraph()->isDescendantGraph(sg)) {
#ifndef NDEBUG
    tlp::warning() << "Warning : " << __PRETTY_FUNCTION__ << " does not compute any value for a subgraph not linked to the graph of the property " << metric->getName().c_str();
#endif
    return;
  }

  double value = -DBL_MAX;
  node n;
  forEach(n, sg->getNodes()) {
    const double& nVal = metric->getNodeValue(n);

    if (nVal > value)
      value = nVal;
  }
  metric->setNodeValue(mN, value);
}

static void computeEdgeMaxValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, edge mE, Iterator<edge>* itE) {
  double value = -DBL_MAX;

  while(itE->hasNext()) {
    const double& eVal = metric->getEdgeValue(itE->next());

    if (eVal > value)
      value = eVal;
  }

  metric->setEdgeValue(mE, value);
}

// min values
static void computeNodeMinValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, node mN, Graph* sg) {
  // nothing to do if the subgraph is not linked to the property graph
  if (sg!=metric->getGraph() && !metric->getGraph()->isDescendantGraph(sg)) {
#ifndef NDEBUG
    tlp::warning() << "Warning : " << __PRETTY_FUNCTION__ << " does not compute any value for a subgraph not linked to the graph of the property " << metric->getName().c_str();
#endif
    return;
  }

  double value = DBL_MAX;
  node n;
  forEach(n, sg->getNodes()) {
    const double& nVal = metric->getNodeValue(n);

    if (nVal < value)
      value = nVal;
  }
  metric->setNodeValue(mN, value);
}

static void computeEdgeMinValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, edge mE, Iterator<edge>* itE) {
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

class DoublePropertyPredefinedCalculator :public AbstractProperty<tlp::DoubleType, tlp::DoubleType>::MetaValueCalculator {
  DoubleNodePredefinedCalculator nodeCalc;
  DoubleEdgePredefinedCalculator edgeCalc;

public:
  DoublePropertyPredefinedCalculator(DoubleProperty::PredefinedMetaValueCalculator nCalc =
                                       DoubleProperty::AVG_CALC,
                                     DoubleProperty::PredefinedMetaValueCalculator eCalc =
                                       DoubleProperty::AVG_CALC)
    :AbstractProperty<tlp::DoubleType, tlp::DoubleType>::MetaValueCalculator(),
     nodeCalc(nodeCalculators[(int) nCalc]),
     edgeCalc(edgeCalculators[(int) eCalc]) {}

  void computeMetaValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, node mN, Graph* sg,
                        Graph*) {
    if (nodeCalc)
      nodeCalc(metric, mN, sg);
  }

  void computeMetaValue(AbstractProperty<tlp::DoubleType, tlp::DoubleType>* metric, edge mE,
                        Iterator<edge>* itE, Graph*) {
    if (edgeCalc)
      edgeCalc(metric, mE, itE);
  }
};

// default calculator
static DoublePropertyPredefinedCalculator avgCalculator;

//==============================
///Constructeur d'un DoubleProperty
DoubleProperty::DoubleProperty (Graph *sg, std::string n) : DoubleMinMaxProperty(sg, n, -DBL_MAX, DBL_MAX, -DBL_MAX, DBL_MAX) {
  assert(sg!=NULL);
  // the property observes the graph
  sg->addListener(this);
  // the computed meta value will be the average value
  setMetaValueCalculator(&avgCalculator);
}

void DoubleProperty::uniformQuantification(unsigned int k) {
  nodesUniformQuantification(k);
  edgesUniformQuantification(k);
}
//===============================================================
void DoubleProperty::nodesUniformQuantification(unsigned int k) {
  std::map<double,double> nodeMapping;

  {
    //build the histogram of node values
    map<double,int> histogram;
    Iterator<node> *itN=graph->getNodes();

    while (itN->hasNext()) {
      node itn=itN->next();
      const double& nodeValue=getNodeValue(itn);

      if (histogram.find(nodeValue)==histogram.end())
        histogram[nodeValue]=1;
      else
        histogram[nodeValue]+=1;
    }

    delete itN;
    //Build the color map
    map<double,int>::iterator it;
    double sum=0;
    double cK=double(graph->numberOfNodes())/double(k);
    int k2=0;

    for (it=histogram.begin(); it!=histogram.end(); ++it) {
      sum+=(*it).second;
      nodeMapping[(*it).first]=k2;

      while (sum>cK*double(k2+1)) ++k2;
    }
  }

  Iterator<node> *itN=graph->getNodes();

  while(itN->hasNext()) {
    node itn=itN->next();
    setNodeValue(itn,nodeMapping[getNodeValue(itn)]);
  }

  delete itN;
}
//===============================================================
void DoubleProperty::edgesUniformQuantification(unsigned int k) {
  std::map<double,double> edgeMapping;
  {
    //build the histogram of edges values
    map<double,int> histogram;
    Iterator<edge> *itE=graph->getEdges();

    while (itE->hasNext()) {
      edge ite=itE->next();
      const double& value=getEdgeValue(ite);

      if (histogram.find(value)==histogram.end())
        histogram[value]=1;
      else
        histogram[value]+=1;
    }

    delete itE;
    //===============================================================
    //Build the color map
    map<double,int>::iterator it;
    double sum=0;
    double cK=double(graph->numberOfEdges())/double(k);
    int k2=0;

    for (it=histogram.begin(); it!=histogram.end(); ++it) {
      sum+=(*it).second;
      edgeMapping[(*it).first]=k2;

      while (sum>cK*double(k2+1)) ++k2;
    }
  }

  Iterator<edge> *itE=graph->getEdges();

  while(itE->hasNext()) {
    edge ite=itE->next();
    setEdgeValue(ite,edgeMapping[getEdgeValue(ite)]);
  }

  delete itE;
}
//====================================================================
void DoubleProperty::clone_handler(AbstractProperty< DoubleType, DoubleType>& proxyC) {
  DoubleProperty *proxy=(DoubleProperty *)&proxyC;
  nodeValueUptodate = proxy->nodeValueUptodate;
  edgeValueUptodate = proxy->edgeValueUptodate;
  minNode = proxy->minNode;
  maxNode = proxy->maxNode;
  minEdge = proxy->minEdge;
  maxEdge = proxy->maxEdge;
}
//=================================================================================
void DoubleProperty::setNodeValue(const node n, const double &v) {
  DoubleMinMaxProperty::updateNodeValue(n, v);
  DoubleMinMaxProperty::setNodeValue(n, v);
}
//=================================================================================
void DoubleProperty::setEdgeValue(const edge e, const double &v) {
  DoubleMinMaxProperty::updateEdgeValue(e, v);
  DoubleMinMaxProperty::setEdgeValue(e, v);
}
//=================================================================================
void DoubleProperty::setAllNodeValue(const double &v) {
  DoubleMinMaxProperty::updateAllNodesValues(v);
  DoubleMinMaxProperty::setAllNodeValue(v);
}
//=================================================================================
void DoubleProperty::setAllEdgeValue(const double &v) {
  DoubleMinMaxProperty::updateAllEdgesValues(v);
  DoubleMinMaxProperty::setAllEdgeValue(v);
}
//=================================================================================
PropertyInterface* DoubleProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;

  // allow to get an unregistered property (empty name)
  DoubleProperty * p = n.empty()
                       ? new DoubleProperty(g) : g->getLocalProperty<DoubleProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=================================================================================
PropertyInterface* DoubleVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;

  // allow to get an unregistered property (empty name)
  DoubleVectorProperty * p = n.empty()
                             ? new DoubleVectorProperty(g) : g->getLocalProperty<DoubleVectorProperty>( n );
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
void DoubleProperty::setMetaValueCalculator(PropertyInterface::MetaValueCalculator* calc) {
  if (metaValueCalculator && metaValueCalculator != &avgCalculator &&
      typeid(metaValueCalculator) == typeid(DoublePropertyPredefinedCalculator))
    delete metaValueCalculator;

  metaValueCalculator = calc;
}
//=============================================================
void DoubleProperty::treatEvent(const Event& evt) {
  DoubleMinMaxProperty::treatEvent(evt);
}


