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

std::string PropertyInterface::getTypename( PropertyInterface * propertyName ) {
  if (typeid((*propertyName)) == typeid(GraphProperty)) return "graph";
  if (typeid((*propertyName)) == typeid(DoubleProperty)) return "double";
  if (typeid((*propertyName)) == typeid(LayoutProperty)) return "layout";
  if (typeid((*propertyName)) == typeid(StringProperty)) return "string";
  if (typeid((*propertyName)) == typeid(IntegerProperty)) return "int";
  if (typeid((*propertyName)) == typeid(ColorProperty)) return "color";
  if (typeid((*propertyName)) == typeid(SizeProperty)) return "size";
  if (typeid((*propertyName)) == typeid(BooleanProperty)) return "bool";
  return "unknown";
}

PropertyInterface::~PropertyInterface() {
    notifyDestroy(this);
}

void PropertyInterface::notifySetNodeValue(PropertyInterface* p,
					   const node n) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifySetNodeValue(p, n);
  Observable::notifyObservers();
}

void PropertyInterface::notifySetEdgeValue(PropertyInterface* p,
					   const edge e) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifySetEdgeValue(p, e);
  Observable::notifyObservers();
}

void PropertyInterface::notifySetAllNodeValue(PropertyInterface* p) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifySetAllNodeValue(p);
  Observable::notifyObservers();
}

void PropertyInterface::notifySetAllEdgeValue(PropertyInterface* p) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifySetAllEdgeValue(p);
  Observable::notifyObservers();
}

void PropertyInterface::notifyDestroy(PropertyInterface* p) {
  // dispatch to PropertyObserver & Observer
  ObservableProperty::notifyDestroy(p);
  Observable::notifyDestroy();
}
