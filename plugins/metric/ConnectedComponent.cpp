#include "ConnectedComponent.h"
#include "tulip/ConnectedTest.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(ConnectedComponent,"Connected Component","David Auber","01/07/2002","Alpha","1.0","Component");

ConnectedComponent::ConnectedComponent(const PropertyContext &context):DoubleAlgorithm(context) {}
//======================================================
ConnectedComponent::~ConnectedComponent(){}
//======================================================
bool ConnectedComponent::run() {
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(graph, components);
  // assign the index of each component as value for its nodes
  unsigned int curComponent = 0;
  for (; curComponent < components.size(); curComponent++) {
    std::set<node>& component = components[curComponent];
    std::set<node>::const_iterator itNode = component.begin();
    double value = curComponent;
    for(; itNode!=component.end(); ++itNode) {
      doubleResult->setNodeValue(*itNode, value);
    }
  }
  // propagate nodes computed value to edges
  Iterator<edge> *itE=graph->getEdges();
    while (itE->hasNext()) {
      edge ite=itE->next();
      node source= graph->source(ite);
      node target= graph->target(ite);
      if (doubleResult->getNodeValue(source) == doubleResult->getNodeValue(target))
	doubleResult->setEdgeValue(ite, doubleResult->getNodeValue(source));
      else
	doubleResult->setEdgeValue(ite,curComponent);
    } delete itE;

  return true;
}
//======================================================







