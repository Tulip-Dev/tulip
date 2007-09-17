#include "ConnectedComponent.h"

using namespace tlp;

DOUBLEPLUGINOFGROUP(ConnectedComponent,"Connected Component","David Auber","01/07/2002","Alpha","1.0","Component");

ConnectedComponent::ConnectedComponent(const PropertyContext &context):DoubleAlgorithm(context) {}
//======================================================
ConnectedComponent::~ConnectedComponent(){}
//======================================================
bool ConnectedComponent::run() {
  computeConnectedComponents(graph, doubleResult);
  return true;
}
//======================================================







