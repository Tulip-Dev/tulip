/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

using namespace tlp;
using namespace std;

const ParameterDescriptionList& tlp::WithParameter::getParameters() {
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
  std::cerr << __PRETTY_FUNCTION__ << name << " does not exists" << std::endl;
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

bool ParameterDescriptionList::isMandatory(const string& name) const {
  return ((ParameterDescriptionList *) this)->getParameter(name)->isMandatory();
}

template<typename TYPEINTERFACE>
void ParameterDescriptionList::insertData(tlp::DataSet &dataSet,const std::string &param,const std::string &defaultValue) const {
  if (dataSet.exist(param))
    return;

  typename TYPEINTERFACE::RealType value;

  if (defaultValue.size()==0)
    value = TYPEINTERFACE::defaultValue();

  else {
    bool result = TYPEINTERFACE::fromString(value,defaultValue);
    assert(result);

    if (!result)
      value = TYPEINTERFACE::defaultValue();
  }

  dataSet.set<typename TYPEINTERFACE::RealType>(param,value);
}

#define CHECK_TYPE(T)\
if (type.compare(typeid(T::RealType).name()) == 0)\
  insertData<T>(dataSet,name,defaultValue);

#define CHECK_PROPERTY(T)\
if (type.compare(typeid(T).name()) == 0) {\
  if (!g || defaultValue.size()==0 || !g->existProperty(defaultValue))\
    dataSet.set<T*>(name,NULL);\
  else\
    dataSet.set<T*>(name,g->getProperty<T>(defaultValue));\
  continue;\
}\
 
void ParameterDescriptionList::buildDefaultDataSet(DataSet &dataSet, Graph *g) const {
  ParameterDescription param;
  forEach(param, getParameters()) {
    const string& name = param.getName();
    const string& type = param.getTypeName();
    const string& defaultValue = param.getDefaultValue();

    CHECK_TYPE(tlp::BooleanType);
    CHECK_TYPE(tlp::IntegerType);
    CHECK_TYPE(tlp::UnsignedIntegerType);
    CHECK_TYPE(tlp::LongType);
    CHECK_TYPE(tlp::DoubleType);
    CHECK_TYPE(tlp::FloatType);
    CHECK_TYPE(tlp::StringType);
    CHECK_TYPE(tlp::ColorType);
    CHECK_TYPE(tlp::SizeType);
    CHECK_TYPE(tlp::PointType);
    CHECK_TYPE(tlp::LineType);

    if (type.compare(typeid(tlp::ColorScale).name()) == 0) {
      vector<Color> colors;
      ColorVectorType::fromString(colors, defaultValue);
      dataSet.set<ColorScale>(name, ColorScale(colors));
    }

    if (type.compare(typeid(tlp::StringCollection).name()) == 0) {
      StringCollection col;
      string::size_type lastPos = defaultValue.find_first_not_of(";");
      string::size_type pos = defaultValue.find_first_of(";", lastPos);

      while (string::npos != pos || string::npos != lastPos) {
        col.push_back(defaultValue.substr(lastPos, pos - lastPos));
        lastPos = defaultValue.find_first_not_of(";", pos);
        pos = defaultValue.find_first_of(";", lastPos);
      }

      dataSet.set<StringCollection>(name,col);
    }

    CHECK_PROPERTY(tlp::BooleanProperty);
    CHECK_PROPERTY(tlp::DoubleProperty);
    CHECK_PROPERTY(tlp::LayoutProperty);
    CHECK_PROPERTY(tlp::StringProperty);
    CHECK_PROPERTY(tlp::IntegerProperty);
    CHECK_PROPERTY(tlp::SizeProperty);
    CHECK_PROPERTY(tlp::ColorProperty);

    if (type.compare(typeid(PropertyInterface*).name()) == 0) {
      if (!g || defaultValue.size()==0 || !g->existProperty(defaultValue))
        dataSet.set<PropertyInterface*>(name, NULL);
      else
        dataSet.set<PropertyInterface*>(name, g->getProperty(defaultValue));

      continue;
    }
  }
}

