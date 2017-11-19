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
#include <tulip/ColorProperty.h>

using namespace std;
using namespace tlp;

const string ColorProperty::propertyTypename = "color";
const string ColorVectorProperty::propertyTypename = "vector<color>";

class ViewColorCalculator : public AbstractColorProperty::MetaValueCalculator {
public:
  void computeMetaValue(AbstractColorProperty *color, node mN, Graph *, Graph *) override {
    // meta node color is half opaque white
    color->setNodeValue(mN, Color(255, 255, 255, 127));
  }

  void computeMetaValue(AbstractColorProperty *color, edge mE, Iterator<edge> *itE,
                        Graph *) override {
    // meta edge color is the color of the first underlying edge
    color->setEdgeValue(mE, color->getEdgeValue(itE->next()));
  }
};

// meta value calculator for viewColor
static ViewColorCalculator vColorCalc;

// Comparison of colors using hsv color space
// Return 0 if the colors are equal otherwise return -1 if the first object is lower than the second
// and 1 if the first object is greater than the second.
static int compareHSVValues(const Color &c1, const Color &c2);

//=================================================================================
ColorProperty::ColorProperty(Graph *g, const std::string &n) : AbstractColorProperty(g, n) {
  if (n == "viewColor") {
    setMetaValueCalculator(&vColorCalc);
  }
}
//=================================================================================
PropertyInterface *ColorProperty::clonePrototype(Graph *g, const std::string &n) const {
  if (!g)
    return nullptr;

  // allow to get an unregistered property (empty name)
  ColorProperty *p = n.empty() ? new ColorProperty(g) : g->getLocalProperty<ColorProperty>(n);
  p->setAllNodeValue(getNodeDefaultValue());
  p->setAllEdgeValue(getEdgeDefaultValue());
  return p;
}
//=================================================================================
PropertyInterface *ColorVectorProperty::clonePrototype(Graph *g, const std::string &n) const {
  if (!g)
    return nullptr;

  // allow to get an unregistered property (empty name)
  ColorVectorProperty *p =
      n.empty() ? new ColorVectorProperty(g) : g->getLocalProperty<ColorVectorProperty>(n);
  p->setAllNodeValue(getNodeDefaultValue());
  p->setAllEdgeValue(getEdgeDefaultValue());
  return p;
}

int ColorProperty::compare(const node n1, const node n2) const {
  return compareHSVValues(getNodeValue(n1), getNodeValue(n2));
}

int ColorProperty::compare(const edge e1, const edge e2) const {
  return compareHSVValues(getEdgeValue(e1), getEdgeValue(e2));
}

int compareHSVValues(const Color &c1, const Color &c2) {
  if (c1.getH() == c2.getH()) {
    if (c1.getS() == c2.getS()) {
      if (c1.getV() == c2.getV()) {
        return 0;
      } else {
        return c1.getV() < c2.getV() ? -1 : 1;
      }
    } else {
      return c1.getS() < c2.getS() ? -1 : 1;
    }
  } else {
    return c1.getH() < c2.getH() ? -1 : 1;
  }
}
