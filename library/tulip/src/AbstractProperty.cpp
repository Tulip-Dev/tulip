#include <iostream>
#include <stdlib.h>
#include "tulip/AbstractProperty.h"
#include "tulip/DoubleProperty.h"
#include "tulip/StringProperty.h"
#include "tulip/BooleanProperty.h"
#include "tulip/LayoutProperty.h"
#include "tulip/IntegerProperty.h"
#include "tulip/ColorProperty.h"
#include "tulip/SizeProperty.h"
#include "tulip/GraphProperty.h"

using namespace tlp;

std::string PropertyInterface::getTypename(const PropertyInterface* propertyName ) {
  if (typeid((*propertyName)) == typeid(GraphProperty)) return "graph";
  if (typeid((*propertyName)) == typeid(DoubleProperty)) return "double";
  if (typeid((*propertyName)) == typeid(LayoutProperty)) return "layout";
  if (typeid((*propertyName)) == typeid(StringProperty)) return "string";
  if (typeid((*propertyName)) == typeid(IntegerProperty)) return "int";
  if (typeid((*propertyName)) == typeid(ColorProperty)) return "color";
  if (typeid((*propertyName)) == typeid(SizeProperty)) return "size";
  if (typeid((*propertyName)) == typeid(BooleanProperty)) return "bool";
  if (typeid((*propertyName)) == typeid(DoubleVectorProperty)) return "vector<double>";
  if (typeid((*propertyName)) == typeid(StringVectorProperty)) return "vector<string>";
  if (typeid((*propertyName)) == typeid(IntegerVectorProperty)) return "vector<int>";
  if (typeid((*propertyName)) == typeid(CoordVectorProperty)) return "vector<coord>";
  if (typeid((*propertyName)) == typeid(ColorVectorProperty)) return "vector<color>";
  if (typeid((*propertyName)) == typeid(SizeVectorProperty)) return "vector<size>";
  if (typeid((*propertyName)) == typeid(BooleanVectorProperty)) return "vector<bool>";
  return "unknown";
}

PropertyInterface::~PropertyInterface() {
  // check if the current property is not registered
  // as a property of a graph
  if (graph && !name.empty() &&
      graph->existLocalProperty(name)) {
    std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Serious bug; you have deleted a registered graph property named '"  << name.c_str() << "'" << std::endl;
    abort();
  }
  notifyDestroy(this);
}

void PropertyInterface::notifyAfterSetNodeValue(PropertyInterface* p,
						const node n) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifyAfterSetNodeValue(p, n);
  Observable::notifyObservers();
}

void PropertyInterface::notifyAfterSetEdgeValue(PropertyInterface* p,
						const edge e) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifyAfterSetEdgeValue(p, e);
  Observable::notifyObservers();
}

void PropertyInterface::notifyAfterSetAllNodeValue(PropertyInterface* p) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifyAfterSetAllNodeValue(p);
  Observable::notifyObservers();
}

void PropertyInterface::notifyAfterSetAllEdgeValue(PropertyInterface* p) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifyAfterSetAllEdgeValue(p);
  Observable::notifyObservers();
}

void PropertyInterface::notifyDestroy(PropertyInterface* p) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifyDestroy(p);
  Observable::notifyDestroy();
}
