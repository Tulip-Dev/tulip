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

#include <tulip/Graph.h>
#include <tulip/StringProperty.h>
#include <tulip/DoubleProperty.h>

using namespace std;
using namespace tlp;

const string StringProperty::propertyTypename = "string";
const string StringVectorProperty::propertyTypename = "vector<string>";

// viewLabel
class ViewLabelCalculator : public AbstractStringProperty::MetaValueCalculator {
public:
  // set the meta node label to label of viewMetric max corresponding node
  void computeMetaValue(AbstractStringProperty *label, node mN, Graph *sg, Graph *) override {
    // nothing to do if viewMetric does not exist
    if (!sg->existProperty("viewMetric"))
      return;

    node viewMetricMaxNode;
    double vMax = -DBL_MAX;
    DoubleProperty *metric = sg->getProperty<DoubleProperty>("viewMetric");

    for (auto itn : sg->nodes()) {
      double value = metric->getNodeValue(itn);

      if (value > vMax) {
        vMax = value;
        viewMetricMaxNode = itn;
      }
    }

    if (viewMetricMaxNode.isValid())
      label->setNodeValue(mN, label->getNodeValue(viewMetricMaxNode));
  }
};

// meta value calculator for viewLabel
static ViewLabelCalculator vLabelCalc;

//=================================================================================
StringProperty::StringProperty(Graph *g, const std::string &n) : AbstractStringProperty(g, n) {
  if (n == "viewLabel") {
    setMetaValueCalculator(&vLabelCalc);
  }
}
//=================================================================================
PropertyInterface *StringProperty::clonePrototype(Graph *g, const std::string &n) const {
  if (!g)
    return nullptr;

  // allow to get an unregistered property (empty name)
  StringProperty *p = n.empty() ? new StringProperty(g) : g->getLocalProperty<StringProperty>(n);
  p->setAllNodeValue(getNodeDefaultValue());
  p->setAllEdgeValue(getEdgeDefaultValue());
  return p;
}
//=================================================================================
int StringProperty::compare(const node n1, const node n2) const {
  return getNodeValue(n1).compare(getNodeValue(n2));
}
//=================================================================================
int StringProperty::compare(const edge e1, const edge e2) const {
  return getEdgeValue(e1).compare(getEdgeValue(e2));
}
//=================================================================================
PropertyInterface *StringVectorProperty::clonePrototype(Graph *g, const std::string &n) const {
  if (!g)
    return nullptr;

  // allow to get an unregistered property (empty name)
  StringVectorProperty *p =
      n.empty() ? new StringVectorProperty(g) : g->getLocalProperty<StringVectorProperty>(n);
  p->setAllNodeValue(getNodeDefaultValue());
  p->setAllEdgeValue(getEdgeDefaultValue());
  return p;
}
