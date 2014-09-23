/**
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
#include <tulip/WithParameter.h>
#include <tulip/DataSet.h>
#include <tulip/PropertyTypes.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/StlIterator.h>
#include <tulip/ColorScale.h>
#include <tulip/ForEach.h>
#include <tulip/StringCollection.h>

using namespace tlp;
using namespace std;

const ParameterDescriptionList& tlp::WithParameter::getParameters() const {
  return parameters;
}

Iterator<ParameterDescription>* ParameterDescriptionList::getParameters() const {
  return new StlIterator<ParameterDescription, vector<ParameterDescription>::const_iterator>(parameters.begin(), parameters.end());
}

ParameterDescription* ParameterDescriptionList::getParameter(const std::string& name) {
  for(unsigned int i = 0; i < parameters.size(); ++i) {
    if (name == parameters[i].getName())
      return &parameters[i];
  }

#ifndef NDEBUG
  tlp::warning() << __PRETTY_FUNCTION__ << name << " does not exists";
#endif

  return NULL;
}

const std::string& ParameterDescriptionList::getDefaultValue(const string& name)  const {
  return ((ParameterDescriptionList *) this)->getParameter(name)->getDefaultValue();
}

void ParameterDescriptionList::setDefaultValue(const string& name,
    const string& val) {
  getParameter(name)->setDefaultValue(val);
}

void ParameterDescriptionList::setDirection(const string& name,
    ParameterDirection direction) {
  getParameter(name)->setDirection(direction);
}

bool ParameterDescriptionList::isMandatory(const string& name) const {
  return ((ParameterDescriptionList *) this)->getParameter(name)->isMandatory();
}

#define CHECK_PROPERTY(T)\
if (type.compare(typeid(T).name()) == 0) {\
  if (!g || defaultValue.empty() || !g->existProperty(defaultValue))\
    dataSet.set(name, (T*) NULL);               \
  else\
    dataSet.set(name, (T*) g->getProperty<T>(defaultValue));  \
  continue;\
}\
 
void ParameterDescriptionList::buildDefaultDataSet(DataSet &dataSet, Graph *g) const {
  ParameterDescription param;
  forEach(param, getParameters()) {
    const string& name = param.getName();
    const string& type = param.getTypeName();
    const string& defaultValue = param.getDefaultValue();

    DataTypeSerializer* dts = DataSet::typenameToSerializer(type);

    if (dts) {
#ifndef NDEBUG
      bool result =
#endif
        dts->setData(dataSet, name, defaultValue);
      assert(result);
      continue;
    }
    else {
      if (type.compare(typeid(tlp::ColorScale).name()) == 0) {
        vector<Color> colors;
        ColorVectorType::fromString(colors, defaultValue);
        dataSet.set(name, ColorScale(colors));
        continue;
      }
    }

    CHECK_PROPERTY(tlp::BooleanProperty);
    CHECK_PROPERTY(tlp::DoubleProperty);
    CHECK_PROPERTY(tlp::LayoutProperty);
    CHECK_PROPERTY(tlp::StringProperty);
    CHECK_PROPERTY(tlp::IntegerProperty);
    CHECK_PROPERTY(tlp::SizeProperty);
    CHECK_PROPERTY(tlp::ColorProperty);
    CHECK_PROPERTY(tlp::BooleanVectorProperty);
    CHECK_PROPERTY(tlp::DoubleVectorProperty);
    CHECK_PROPERTY(tlp::CoordVectorProperty);
    CHECK_PROPERTY(tlp::StringVectorProperty);
    CHECK_PROPERTY(tlp::IntegerVectorProperty);
    CHECK_PROPERTY(tlp::SizeVectorProperty);
    CHECK_PROPERTY(tlp::ColorVectorProperty);

    if (type.compare(typeid(NumericProperty*).name()) == 0) {
      if (!g || defaultValue.empty())
        dataSet.set(name, (NumericProperty*) NULL);
      else {
        PropertyInterface* prop = g->getProperty(defaultValue);

        if (!dynamic_cast<NumericProperty*>(prop)) {
          tlp::error() << "NumericProperty '" << defaultValue.c_str() << "' not found for parameter '" << name.c_str() << std::endl;
          prop = NULL;
        }

        dataSet.set(name, (NumericProperty *) prop);
      }

      continue;
    }

    if (type.compare(typeid(PropertyInterface*).name()) == 0) {
      if (!g || defaultValue.empty())
        dataSet.set(name, (PropertyInterface*) NULL);
      else {
        if (!g->existProperty(defaultValue)) {
          tlp::error() << "Property '" << defaultValue.c_str() << "' not found for parameter '" << name.c_str() << std::endl;
          dataSet.set(name, (PropertyInterface*) NULL);
        }
        else
          dataSet.set(name, g->getProperty(defaultValue));
      }
    }
  }
}

bool WithParameter::inputRequired() const {
  ParameterDescription param;
  forEach(param, parameters.getParameters()) {
    if (param.getDirection() != OUT_PARAM)
      return true;

    const string& type = param.getTypeName();

    if (type.compare(typeid(tlp::BooleanProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::ColorProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::DoubleProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::IntegerProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::LayoutProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::SizeProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::StringProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::BooleanVectorProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::ColorVectorProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::DoubleVectorProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::IntegerVectorProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::CoordVectorProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::SizeProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::StringProperty).name()) == 0)
      return true;

    if (type.compare(typeid(tlp::PropertyInterface*).name()) == 0)
      return true;
  }
  return false;
}
