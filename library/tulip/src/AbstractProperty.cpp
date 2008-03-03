#include "tulip/Graph.h"
#include "tulip/DoubleProperty.h"
#include "tulip/StringProperty.h"
#include "tulip/BooleanProperty.h"
#include "tulip/LayoutProperty.h"
#include "tulip/IntegerProperty.h"
#include "tulip/ColorProperty.h"
#include "tulip/SizeProperty.h"
#include "tulip/GraphProperty.h"

std::string tlp::PropertyInterface::getTypename( tlp::PropertyInterface * propertyName ) {
  if (typeid((*propertyName)) == typeid(tlp::GraphProperty)) return "graph";
  if (typeid((*propertyName)) == typeid(tlp::DoubleProperty)) return "double";
  if (typeid((*propertyName)) == typeid(tlp::LayoutProperty)) return "layout";
  if (typeid((*propertyName)) == typeid(tlp::StringProperty)) return "string";
  if (typeid((*propertyName)) == typeid(tlp::IntegerProperty)) return "int";
  if (typeid((*propertyName)) == typeid(tlp::ColorProperty)) return "color";
  if (typeid((*propertyName)) == typeid(tlp::SizeProperty)) return "size";
  if (typeid((*propertyName)) == typeid(tlp::BooleanProperty)) return "bool";
  return "unknown";
}
