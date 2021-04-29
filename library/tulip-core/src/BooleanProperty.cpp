/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/BooleanProperty.h>

using namespace std;
using namespace tlp;

const string BooleanProperty::propertyTypename = "bool";
const string BooleanVectorProperty::propertyTypename = "vector<bool>";

//=================================================================================
void BooleanProperty::reverse(const Graph *sg) {
  if (sg == nullptr)
    sg = graph;

  for (auto n : sg->nodes()) {
    notifyBeforeSetNodeValue(n);
    nodeProperties.invertBooleanValue(n.id);
    notifyAfterSetNodeValue(n);
  }

  for (auto e : sg->edges()) {
    notifyBeforeSetEdgeValue(e);
    edgeProperties.invertBooleanValue(e.id);
    notifyAfterSetEdgeValue(e);
  }
}

//=================================================================================
void BooleanProperty::reverseEdgeDirection(Graph *sg) {
  if (sg == nullptr)
    sg = graph;

  for (auto e : sg->edges()) {
    if (getEdgeValue(e))
      sg->reverse(e);
  }
}

//=================================================================================
PropertyInterface *BooleanProperty::clonePrototype(Graph *g, const std::string &n) const {
  if (g == nullptr)
    return nullptr;

  // allow to get an unregistered property (empty name)
  BooleanProperty *p = n.empty() ? new BooleanProperty(g) : g->getLocalProperty<BooleanProperty>(n);
  p->setAllNodeValue(getNodeDefaultValue());
  p->setAllEdgeValue(getEdgeDefaultValue());
  return p;
}
//=================================================================================
PropertyInterface *BooleanVectorProperty::clonePrototype(Graph *g, const std::string &n) const {
  if (!g)
    return nullptr;

  // allow to get an unregistered property (empty name)
  BooleanVectorProperty *p =
      n.empty() ? new BooleanVectorProperty(g) : g->getLocalProperty<BooleanVectorProperty>(n);
  p->setAllNodeValue(getNodeDefaultValue());
  p->setAllEdgeValue(getEdgeDefaultValue());
  return p;
}
