#include "tulip/AbstractProperty.h"
#include "tulip/Graph.h"
#include "tulip/DoubleProperty.h"
#include "tulip/StringProperty.h"
#include "tulip/BooleanProperty.h"
#include "tulip/LayoutProperty.h"
#include "tulip/IntegerProperty.h"
#include "tulip/ColorProperty.h"
#include "tulip/SizeProperty.h"
#include "tulip/GraphProperty.h"

std::string PropertyInterface::getTypename( PropertyInterface * propertyName ) {
  if (typeid((*propertyName)) == typeid(GraphProperty)) return "sgproperty";
  if (typeid((*propertyName)) == typeid(DoubleProperty)) return "doubleproperty";
  if (typeid((*propertyName)) == typeid(LayoutProperty)) return "layoutproperty";
  if (typeid((*propertyName)) == typeid(StringProperty)) return "stringproperty";
  if (typeid((*propertyName)) == typeid(IntegerProperty)) return "integerproperty";
  if (typeid((*propertyName)) == typeid(ColorProperty)) return "colorproperty";
  if (typeid((*propertyName)) == typeid(SizeProperty)) return "sizeproperty";
  if (typeid((*propertyName)) == typeid(BooleanProperty)) return "boolproperty";
  return "unknown";
}
