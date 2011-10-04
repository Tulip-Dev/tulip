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

#include <QtCore/QDebug>

using namespace tlp;
using namespace std;

ParameterList tlp::WithParameter::getParameters() {
  return parameters;
}

bool ParameterList::hasField(std::string parameterName) const {
  return parameters.find(parameterName) != parameters.end();
}

void ParameterList::erase(string parameterName) {
  parameters.erase(parameterName);
}

Iterator<std::string>* ParameterList::getParametersNames() const {
  return new StlMapKeyIterator<string, Parameter>(parameters.begin(), parameters.end());
}

string ParameterList::getHelp(string parameterName) const {
  map<string, Parameter>::const_iterator it = parameters.find(parameterName);

  if(it != parameters.end()) {
    return parameters.find(parameterName)->second.help;
  }
  else {
#ifndef NDEBUG
    std::cout << "attemp to retrieve help of non-existing parameter: " << parameterName << std::endl;
#endif
    return string();
  }
}

std::string ParameterList::getTypeName(string parameterName) const {
  map<string, Parameter>::const_iterator it = parameters.find(parameterName);

  if(it != parameters.end()) {
    return it->second.type;
  }
  else {
#ifndef NDEBUG
    std::cout << "attemp to get value of non-existing parameter: " << parameterName << std::endl;
#endif
    return string();
  }
}

string ParameterList::getDefaultValue(string parameterName) const {
  map<string, Parameter>::const_iterator it = parameters.find(parameterName);

  if(it != parameters.end()) {
    return it->second.defaultValue;
  }
  else {
#ifndef NDEBUG
    std::cout << "attemp to get default value of non-existing parameter: " << parameterName << std::endl;
#endif
    return string();
  }
}

void ParameterList::setDefaultValue(string parameterName, string value) {
  map<string, Parameter>::iterator it = parameters.find(parameterName);

  if(it != parameters.end()) {
    it->second.defaultValue = value;
  }

#ifndef NDEBUG
  else {
    std::cout << "attemp to set default value on non-existing parameter: " << parameterName << std::endl;
  }

#endif
}

bool ParameterList::isMandatory(string parameterName) const {
  map<string, Parameter>::const_iterator it = parameters.find(parameterName);

  if(it != parameters.end()) {
    return it->second.mandatory;
  }
  else {
#ifndef NDEBUG
    std::cout << "attemp to retrieve whether non-existing parameter is mandatory: " << parameterName << std::endl;
#endif
    return false;
  }
}

template<typename TYPEINTERFACE>
void ParameterList::insertData(tlp::DataSet &dataSet,const std::string &param,const std::string &defaultValue) const {
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
 
void ParameterList::buildDefaultDataSet(DataSet &dataSet, Graph *g) const {
  string name;
  forEach(name,getParametersNames()) {
    string type = getTypeName(name);

    string defaultValue = getDefaultValue(name);

    CHECK_TYPE(tlp::BooleanType);
    CHECK_TYPE(tlp::IntegerType);
    CHECK_TYPE(tlp::UnsignedIntegerType);
    CHECK_TYPE(tlp::LongType);
    CHECK_TYPE(tlp::DoubleType);
    CHECK_TYPE(tlp::FloatType);
    CHECK_TYPE(tlp::StringType);
    CHECK_TYPE(tlp::ColorType);
    CHECK_TYPE(tlp::SizeType);

    if (type.compare(typeid(tlp::ColorScale).name()) == 0) {
      vector<Color> colors;
      ColorVectorType::fromString(colors,defaultValue);
      dataSet.set<ColorScale>(name,ColorScale(colors));
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
        dataSet.set<PropertyInterface*>(name,NULL);
      else
        dataSet.set<PropertyInterface*>(name,g->getProperty(defaultValue));

      continue;
    }
  }
}

