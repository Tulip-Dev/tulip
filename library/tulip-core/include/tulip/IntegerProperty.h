/*
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

#ifndef TULIP_INT_H
#define TULIP_INT_H

#include <tulip/tuliphash.h>
#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/Observable.h>
#include <tulip/minmaxproperty.h>
#include <tulip/NumericProperty.h>

namespace tlp {

class Graph;
class PropertyContext;

typedef MinMaxProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty>
    IntegerMinMaxProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps an integer value to graph elements.
 */
class TLP_SCOPE IntegerProperty : public IntegerMinMaxProperty {

public:
  IntegerProperty(Graph *, const std::string &n = "");

  PropertyInterface *clonePrototype(Graph *, const std::string &) const;
  static const std::string propertyTypename;
  const std::string &getTypename() const {
    return propertyTypename;
  }
  virtual void setNodeValue(const node n, tlp::StoredType<int>::ReturnedConstValue v);
  virtual void setEdgeValue(const edge e, tlp::StoredType<int>::ReturnedConstValue v);
  virtual void setAllNodeValue(tlp::StoredType<int>::ReturnedConstValue v);

  virtual void setValueToGraphNodes(tlp::StoredType<int>::ReturnedConstValue v, const Graph *graph);
  virtual void setAllEdgeValue(tlp::StoredType<int>::ReturnedConstValue v);
  virtual void setValueToGraphEdges(tlp::StoredType<int>::ReturnedConstValue v, const Graph *graph);

  int compare(const node n1, const node n2) const;
  int compare(const edge e1, const edge e2) const;

  // NumericProperty interface
  virtual double getNodeDoubleValue(const node n) const {
    return getNodeValue(n);
  }
  virtual double getNodeDoubleDefaultValue() const {
    return getNodeDefaultValue();
  }
  virtual double getNodeDoubleMin(const Graph *g = nullptr) {
    return getNodeMin(g);
  }
  virtual double getNodeDoubleMax(const Graph *g = nullptr) {
    return getNodeMax(g);
  }
  virtual double getEdgeDoubleValue(const edge e) const {
    return getEdgeValue(e);
  }
  virtual double getEdgeDoubleDefaultValue() const {
    return getEdgeDefaultValue();
  }
  virtual double getEdgeDoubleMin(const Graph *g = nullptr) {
    return getEdgeMin(g);
  }
  virtual double getEdgeDoubleMax(const Graph *g = nullptr) {
    return getEdgeMax(g);
  }

  void nodesUniformQuantification(unsigned int);

  void edgesUniformQuantification(unsigned int);

  NumericProperty *copyProperty(Graph *g) {
    IntegerProperty *newProp = new IntegerProperty(g);
    newProp->copy(this);

    return newProp;
  }

  _DEPRECATED virtual void setAllNodeValue(tlp::StoredType<int>::ReturnedConstValue v,
                                           const Graph *graph);
  _DEPRECATED virtual void setAllEdgeValue(tlp::StoredType<int>::ReturnedConstValue v,
                                           const Graph *graph);

protected:
  virtual void
  clone_handler(AbstractProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty> &);

private:
  // override Observable::treatEvent
  void treatEvent(const Event &);
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<int> value to graph elements.
 */
class TLP_SCOPE IntegerVectorProperty
    : public AbstractVectorProperty<tlp::IntegerVectorType, tlp::IntegerType> {
public:
  IntegerVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<IntegerVectorType, tlp::IntegerType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const;
  static const std::string propertyTypename;
  const std::string &getTypename() const {
    return propertyTypename;
  }
};
}
#endif
///@endcond
