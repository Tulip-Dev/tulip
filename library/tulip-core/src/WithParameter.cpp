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
#include <tulip/Reflect.h>
#include <tulip/PropertyTypes.h>

#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/StlIterator.h>

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

#undef TYPE_NAME
#define TYPE_NAME( T ) typeid(T).name()

void ParameterList::buildDefaultDataSet(DataSet &ioDataSet, Graph *inG) const {
  Iterator<string> * parameterNames = getParametersNames();
  
  while( parameterNames->hasNext() ) {
    const string & parameterName  = parameterNames->next();
    const string & parameterTypename = getTypeName(parameterName);
    // Already defined ?
    if( ioDataSet.exist(parameterName) )
      continue;
    // Has def value ?
      const string & defaultValue = getDefaultValue(parameterName);
      if( defaultValue.size() == 0 )
        continue;
      // bool
        if( parameterTypename == TYPE_NAME(bool) ){
          bool v;
          bool res = BooleanType::fromString( v, defaultValue );
          assert( res );
          if( res )
            ioDataSet.set( parameterName, v );
        }
        // int
        else if( parameterTypename == TYPE_NAME(int) ) {
          int v;
          bool res = IntegerType::fromString( v, defaultValue );
          assert( res );
          if( res )
            ioDataSet.set( parameterName, v );
        }
        // unsigned int
        else if( parameterTypename == TYPE_NAME(unsigned int) ) {
          unsigned int v = atol( defaultValue.c_str() );
          ioDataSet.set( parameterName, v );
        }
        // unsigned int
        else if( parameterTypename == TYPE_NAME(long) ) {
          long v = atol( defaultValue.c_str() );
          ioDataSet.set( parameterName, v );
        }
        // fp
        else if( parameterTypename == TYPE_NAME(double) ) {
          double v;
          bool res = DoubleType::fromString( v, defaultValue );
          assert( res );
          if( res )
            ioDataSet.set( parameterName, (double)v );
        }
        else if( parameterTypename == TYPE_NAME(float) ) {
          double v;
          bool res = DoubleType::fromString( v, defaultValue );
          assert( res );
          if( res )
            ioDataSet.set( parameterName, (float)v );
        }
        // string
        else if( parameterTypename == TYPE_NAME(string) ) {
          ioDataSet.set( parameterName, defaultValue );
        }
        // Color
        else if( parameterTypename == TYPE_NAME(Color) ) {
          Color v;
          bool res = ColorType::fromString( v, defaultValue );
          assert( res );
          if( res ) {
            ioDataSet.set( parameterName, v );
            //cout << v;
          }
        }
        // Size
        else if( parameterTypename == TYPE_NAME(Size) ) {
          Size v;
          bool res = SizeType::fromString( v, defaultValue );
          assert( res );
          if( res )
            ioDataSet.set( parameterName, v );
        } else
          // look for an already existing property
          if (inG && (inG->existProperty(defaultValue))) {
            // BooleanProperty
            if( parameterTypename == TYPE_NAME(BooleanProperty) ) {
              ioDataSet.set( parameterName, inG->getProperty<BooleanProperty>(defaultValue) );
            }
            // DoubleProperty
            else if( parameterTypename == TYPE_NAME(DoubleProperty) ) {
              // look for an already existing property
              if (inG->existProperty(defaultValue))
                ioDataSet.set( parameterName, inG->getProperty<DoubleProperty>(defaultValue) );
            }
            // LayoutProperty
            else if( parameterTypename == TYPE_NAME(LayoutProperty) ) {
              ioDataSet.set( parameterName, inG->getProperty<LayoutProperty>(defaultValue) );
            }
            // StringProperty
            else if( parameterTypename == TYPE_NAME(StringProperty) ) {
              ioDataSet.set( parameterName, inG->getProperty<StringProperty>(defaultValue) );
            }
            // IntegerProperty
            else if( parameterTypename == TYPE_NAME(IntegerProperty) ) {
              ioDataSet.set( parameterName, inG->getProperty<IntegerProperty>(defaultValue) );
            }
            // SizeProperty
            else if( parameterTypename == TYPE_NAME(SizeProperty) ) {
              ioDataSet.set( parameterName, inG->getProperty<SizeProperty>(defaultValue) );
            }
            // ColorProperty
            else if( parameterTypename == TYPE_NAME(ColorProperty) ) {
              ioDataSet.set( parameterName, inG->getProperty<ColorProperty>(defaultValue) );
            }
            //
            else if ( parameterTypename == TYPE_NAME(PropertyInterface*) ) {
              ioDataSet.set( parameterName, inG->getProperty(defaultValue) );
            }
          }
  }
  delete parameterNames;
}

