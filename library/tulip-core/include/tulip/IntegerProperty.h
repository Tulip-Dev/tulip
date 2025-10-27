/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>
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
  using IntegerMinMaxProperty::operator=;
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
  DEFINE_GET_CPP_CLASS_NAME;
  void setNodeValue(const node n, tlp::StoredType<int>::ReturnedConstValue v) override;

  // inner class used to extend the overloading of the operator[]
  // to set a node value
  class nodeValueRef : public AbstractProperty<tlp::IntegerType, tlp::IntegerType,
                                               tlp::NumericProperty>::nodeValueRef {
  public:
    constexpr nodeValueRef(IntegerProperty *prop, node n)
        : AbstractProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty>::nodeValueRef(
              prop, n) {}

    nodeValueRef &operator=(
        typename tlp::StoredType<typename IntegerType::RealType>::ReturnedConstValue val) noexcept {
      _prop->setNodeValue(_n, val);
      return *this;
    }

    // prefix increment
    nodeValueRef &operator++() {
      _prop->setNodeValue(_n, getValue() + 1);
      return *this;
    }

    // postfix increment
    auto operator++(int) {
      auto val = getValue();
      _prop->setNodeValue(_n, val + 1);
      return val;
    }

    // increment and assign
    nodeValueRef &operator+=(int val) {
      _prop->setNodeValue(_n, getValue() + val);
      return *this;
    }

    // prefix decrement
    nodeValueRef &operator--() {
      _prop->setNodeValue(_n, getValue() - 1);
      return *this;
    }

    // postfix decrement
    auto operator--(int) {
      auto val = getValue();
      _prop->setNodeValue(_n, val - 1);
      return val;
    }

    // decrement and assign
    nodeValueRef &operator-=(int val) {
      _prop->setNodeValue(_n, getValue() - val);
      return *this;
    }
  };

  // overload operator[] to set a node value
  constexpr nodeValueRef operator[](node n) {
    return nodeValueRef(this, n);
  }

  void setEdgeValue(const edge e, tlp::StoredType<int>::ReturnedConstValue v) override;

  // inner class used to extend the overloading of the operator[]
  // to set an edge value
  class edgeValueRef : public AbstractProperty<tlp::IntegerType, tlp::IntegerType,
                                               tlp::NumericProperty>::edgeValueRef {
  public:
    constexpr edgeValueRef(IntegerProperty *prop, edge e)
        : AbstractProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty>::edgeValueRef(
              prop, e) {}

    edgeValueRef &operator=(
        typename tlp::StoredType<typename IntegerType::RealType>::ReturnedConstValue val) noexcept {
      _prop->setEdgeValue(_e, val);
      return *this;
    }

    // prefix increment
    edgeValueRef &operator++() {
      _prop->setEdgeValue(_e, getValue() + 1);
      return *this;
    }

    // postfix increment
    auto operator++(int) {
      auto val = getValue();
      _prop->setEdgeValue(_e, val + 1);
      return val;
    }

    // increase value
    edgeValueRef &operator+=(int val) {
      _prop->setEdgeValue(_e, getValue() + val);
      return *this;
    }

    // prefix decrement
    edgeValueRef &operator--() {
      _prop->setEdgeValue(_e, getValue() - 1);
      return *this;
    }

    // postfix decrement
    auto operator--(int) {
      auto val = getValue();
      _prop->setEdgeValue(_e, val - 1);
      return val;
    }

    // decrease value
    edgeValueRef &operator-=(int val) {
      _prop->setEdgeValue(_e, getValue() - val);
      return *this;
    }
  };

  // overload operator[] to set an edge value
  constexpr edgeValueRef operator[](edge e) {
    return edgeValueRef(this, e);
  }

  void setAllNodeValue(tlp::StoredType<int>::ReturnedConstValue v) override;

  void setValueToGraphNodes(tlp::StoredType<int>::ReturnedConstValue v,
                            const Graph *graph) override;
  void setAllEdgeValue(tlp::StoredType<int>::ReturnedConstValue v) override;
  void setValueToGraphEdges(tlp::StoredType<int>::ReturnedConstValue v,
                            const Graph *graph) override;

  int compare(const node n1, const node n2) const override;
  int compare(const edge e1, const edge e2) const override;

  // NumericProperty interface
  double getNodeDoubleValue(const node n) const override {
    return getNodeValue(n);
  }
  double getNodeDoubleDefaultValue() const override {
    return getNodeDefaultValue();
  }
  double getNodeDoubleMin(const Graph *g = nullptr) override {
    return getNodeMin(g);
  }
  double getNodeDoubleMax(const Graph *g = nullptr) override {
    return getNodeMax(g);
  }
  double getEdgeDoubleValue(const edge e) const override {
    return getEdgeValue(e);
  }
  double getEdgeDoubleDefaultValue() const override {
    return getEdgeDefaultValue();
  }
  double getEdgeDoubleMin(const Graph *g = nullptr) override {
    return getEdgeMin(g);
  }
  double getEdgeDoubleMax(const Graph *g = nullptr) override {
    return getEdgeMax(g);
  }

  void nodesUniformQuantification(unsigned int) override;

  void edgesUniformQuantification(unsigned int) override;

  NumericProperty *copyProperty(Graph *g) override {
    IntegerProperty *newProp = new IntegerProperty(g);
    newProp->copy(this);

    return newProp;
  }

protected:
  void clone_handler(
      AbstractProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty> &) override;

private:
  // override Observable::treatEvent
  void treatEvent(const Event &) override;
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<int> value to graph elements.
 */
class TLP_SCOPE IntegerVectorProperty
    : public AbstractVectorProperty<tlp::IntegerVectorType, tlp::IntegerType> {
public:
  using AbstractVectorProperty<tlp::IntegerVectorType, tlp::IntegerType>::operator=;
  IntegerVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<IntegerVectorType, tlp::IntegerType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
  DEFINE_GET_CPP_CLASS_NAME;
};
} // namespace tlp
#endif
///@endcond
