/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#ifndef PROPERTYWRAPPER_H
#define PROPERTYWRAPPER_H

#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/SizeProperty.h>

/**
 * @brief simple implementation of the copy-on-write idiom.
 */
template<typename PROPERTYTYPE, typename Type>
class ValueWrapper {
public:
  ValueWrapper(PROPERTYTYPE* prop, tlp::node n) : _prop(prop), _n(n) {
  }

  ValueWrapper(PROPERTYTYPE* prop, tlp::edge e) : _prop(prop), _e(e) {
  }

  void operator=(Type other) {
    if(_n.isValid())
      _prop->setNodeValue(_n, other);

    if(_e.isValid())
      _prop->setEdgeValue(_e, other);
  }

  /**
   * @brief Implicit conversion to Type
   */
  operator Type() const {
    if(_n.isValid())
      return _prop->getNodeValue(_n);

    if(_e.isValid())
      return _prop->getEdgeValue(_e);

    std::cout << "WTF!?" << std::endl;
    return Type();
  }

  /**
   * @brief operator= when prop[n] = prop[n2]
   * @param other
   */
  void operator=(ValueWrapper<PROPERTYTYPE, Type> other) {
    if(_n.isValid())
      _prop->setNodeValue(_n, Type(other));

    if(_e.isValid())
      _prop->setEdgeValue(_e, Type(other));
  }

private:
  PROPERTYTYPE* _prop;
  tlp::node _n;
  tlp::edge _e;
};

template<typename PROPERTYTYPE, typename Type>
class PropertyWrapper {
public:
  PropertyWrapper(tlp::PropertyInterface* internal) {
    PROPERTYTYPE* castedInternal = dynamic_cast<PROPERTYTYPE*>(internal);

    if(castedInternal == nullptr) {
      tlp::error() << "error: could not convert tulip property to " << tlp::demangleTlpClassName(typeid(PROPERTYTYPE).name()) << std::endl;
    }

    _internal = castedInternal;
  }
  PropertyWrapper() : _internal(nullptr) {}

  bool isValid() const {
    return _internal != nullptr;
  }

  void setAllNodeValue(Type value) {
    _internal->setAllNodeValue(value);
  }

  void setAllEdgeValue(Type value) {
    _internal->setAllEdgeValue(value);
  }

  Type getNodeValue(tlp::node n) const {
    return _internal->getNodeValue(n);
  }
  void setNodeValue(tlp::node n, Type value) {
    _internal->setNodeValue(n, value);
  }

  Type getEdgeValue(tlp::edge e) const {
    return _internal->getEdgeValue(e);
  }
  void setEdgeValue(tlp::edge e, Type value) {
    _internal->setEdgeValue(e, value);
  }

  Type operator[](tlp::node n) const {
    return _internal->getNodeValue(n);
  }
  Type operator[](tlp::edge e) const {
    return _internal->getEdgeValue(e);
  }
  ValueWrapper<PROPERTYTYPE, Type> operator[](tlp::node n) {
    return ValueWrapper<PROPERTYTYPE, Type>(_internal, n);
  }

  ValueWrapper<PROPERTYTYPE, Type> operator[](tlp::edge e) {
    return ValueWrapper<PROPERTYTYPE, Type>(_internal, e);
  }

  PROPERTYTYPE* internal() const {
    return _internal;
  }

  operator PROPERTYTYPE*() {
    return _internal;
  }

private:
  PROPERTYTYPE* _internal;
};

template<typename PROPERTYTYPE, typename NodeType, typename EdgeType>
class ComplexValueWrapper {
public:
  ComplexValueWrapper(PROPERTYTYPE* prop, tlp::node n) : _prop(prop), _n(n) {
  }

  ComplexValueWrapper(PROPERTYTYPE* prop, tlp::edge e) : _prop(prop), _e(e) {
  }

  void operator=(NodeType other) {
    if(_n.isValid())
      _prop->setNodeValue(_n, other);
  }

  void operator=(EdgeType other) {
    if(_e.isValid())
      _prop->setEdgeValue(_e, other);
  }

  operator NodeType() const {
    if(_n.isValid())
      return _prop->getNodeValue(_n);
  }
  operator EdgeType() const {
    if(_e.isValid())
      return _prop->getEdgeValue(_e);
  }

private:
  PROPERTYTYPE* _prop;
  tlp::node _n;
  tlp::edge _e;
};

template<typename PROPERTYTYPE, typename NodeType, typename EdgeType>
class ComplexPropertyWrapper {
public:
  ComplexPropertyWrapper(tlp::PropertyInterface* internal) {
    PROPERTYTYPE* castedInternal = dynamic_cast<PROPERTYTYPE*>(internal);

    if(castedInternal == nullptr) {
      tlp::error() << "error: could not convert tulip property to " << tlp::demangleTlpClassName(typeid(PROPERTYTYPE).name()) << std::endl;
    }

    _internal = castedInternal;
  }
  ComplexPropertyWrapper() : _internal(nullptr) {}

  bool isValid() const {
    return _internal != nullptr;
  }

  void setAllNodeValue(NodeType value) {
    _internal->setAllNodeValue(value);
  }

  void setAllEdgeValue(EdgeType value) {
    _internal->setAllEdgeValue(value);
  }

  NodeType getNodeValue(tlp::node n) const {
    return _internal->getNodeValue(n);
  }
  void setNodeValue(tlp::node n, EdgeType value) {
    _internal->setNodeValue(n, value);
  }

  NodeType getEdgeValue(tlp::edge e) const {
    return _internal->getEdgeValue(e);
  }
  void setEdgeValue(tlp::edge e, NodeType value) {
    _internal->setEdgeValue(e, value);
  }

  NodeType operator[](tlp::node n) const {
    return _internal->getNodeValue(n);
  }
  EdgeType operator[](tlp::edge e) const {
    return _internal->getEdgeValue(e);
  }
  ComplexValueWrapper<PROPERTYTYPE, NodeType, EdgeType> operator[](tlp::node n) {
    return ComplexValueWrapper<PROPERTYTYPE, NodeType, EdgeType>(_internal, n);
  }

  ComplexValueWrapper<PROPERTYTYPE, NodeType, EdgeType> operator[](tlp::edge e) {
    return ComplexValueWrapper<PROPERTYTYPE, NodeType, EdgeType>(_internal, e);
  }

  PROPERTYTYPE* internal() const;

  operator PROPERTYTYPE*() {
    return _internal;
  }

private:
  PROPERTYTYPE* _internal;
};

typedef PropertyWrapper<tlp::DoubleProperty, double> DoublePropertyWrapper;
typedef PropertyWrapper<tlp::IntegerProperty, int> IntegerPropertyWrapper;
typedef PropertyWrapper<tlp::BooleanProperty, bool> BooleanPropertyWrapper;
typedef PropertyWrapper<tlp::ColorProperty, tlp::Color> ColorPropertyWrapper;
typedef PropertyWrapper<tlp::StringProperty, std::string> StringPropertyWrapper;
typedef PropertyWrapper<tlp::SizeProperty, tlp::Size> SizePropertyWrapper;
typedef ComplexPropertyWrapper<tlp::LayoutProperty, tlp::Coord, tlp::LineType> LayoutPropertyWrapper;

#endif // PROPERTYWRAPPER_H
