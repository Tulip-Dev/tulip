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

std::string ParameterList::getValue(string parameterName) const {
  map<string, Parameter>::const_iterator it = parameters.find(parameterName);
  if(it != parameters.end()) {
    return it->second.data;
  }
  #ifndef NDEBUG
  else {
    std::cout << "attemp to get value of non-existing parameter: " << parameterName << std::endl;
    return string();
  }
  #endif
}

string ParameterList::getDefaultValue(string parameterName) const {
  map<string, Parameter>::const_iterator it = parameters.find(parameterName);
  if(it != parameters.end()) {
    return it->second.defaultValue;
  }
  #ifndef NDEBUG
  else {
    std::cout << "attemp to get default value of non-existing parameter: " << parameterName << std::endl;
    return string();
  }
  #endif
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

#undef TN
#define TN( T ) typeid(T).name()

void ParameterList::buildDefaultDataSet(DataSet &ioDataSet, Graph *inG) const {
  Iterator<string> * defIt;
  defIt = getParametersNames();
  
  while( defIt->hasNext() ) {
    const string & name  = defIt->next();
    const string & tname = getValue(name);
    // Already defined ?
    if( ioDataSet.exist(name) )
      continue;
    // Has def value ?
      const string & defv = getDefaultValue(name);
      if( defv.size() == 0 )
        continue;
      // bool
        if( tname == TN(bool) ){
          bool v;
          bool res = BooleanType::fromString( v, defv );
          assert( res );
          if( res )
            ioDataSet.set( name, v );
        }
        // int
        else if( tname == TN(int) ) {
          int v;
          bool res = IntegerType::fromString( v, defv );
          assert( res );
          if( res )
            ioDataSet.set( name, v );
        }
        // unsigned int
        else if( tname == TN(unsigned int) ) {
          unsigned int v = atol( defv.c_str() );
          ioDataSet.set( name, v );
        }
        // unsigned int
        else if( tname == TN(long) ) {
          long v = atol( defv.c_str() );
          ioDataSet.set( name, v );
        }
        // fp
        else if( tname == TN(double) ) {
          double v;
          bool res = DoubleType::fromString( v, defv );
          assert( res );
          if( res )
            ioDataSet.set( name, (double)v );
        }
        else if( tname == TN(float) ) {
          double v;
          bool res = DoubleType::fromString( v, defv );
          assert( res );
          if( res )
            ioDataSet.set( name, (float)v );
        }
        // string
        else if( tname == TN(string) ) {
          ioDataSet.set( name, defv );
        }
        // Color
        else if( tname == TN(Color) ) {
          Color v;
          bool res = ColorType::fromString( v, defv );
          assert( res );
          if( res ) {
            ioDataSet.set( name, v );
            //cout << v;
          }
        }
        // Size
        else if( tname == TN(Size) ) {
          Size v;
          bool res = SizeType::fromString( v, defv );
          assert( res );
          if( res )
            ioDataSet.set( name, v );
        } else
          // look for an already existing property
          if (inG && (inG->existProperty(defv))) {
            // BooleanProperty
            if( tname == TN(BooleanProperty) ) {
              ioDataSet.set( name, inG->getProperty<BooleanProperty>(defv) );
            }
            // DoubleProperty
            else if( tname == TN(DoubleProperty) ) {
              // look for an already existing property
              if (inG->existProperty(defv))
                ioDataSet.set( name, inG->getProperty<DoubleProperty>(defv) );
            }
            // LayoutProperty
            else if( tname == TN(LayoutProperty) ) {
              ioDataSet.set( name, inG->getProperty<LayoutProperty>(defv) );
            }
            // StringProperty
            else if( tname == TN(StringProperty) ) {
              ioDataSet.set( name, inG->getProperty<StringProperty>(defv) );
            }
            // IntegerProperty
            else if( tname == TN(IntegerProperty) ) {
              ioDataSet.set( name, inG->getProperty<IntegerProperty>(defv) );
            }
            // SizeProperty
            else if( tname == TN(SizeProperty) ) {
              ioDataSet.set( name, inG->getProperty<SizeProperty>(defv) );
            }
            // ColorProperty
            else if( tname == TN(ColorProperty) ) {
              ioDataSet.set( name, inG->getProperty<ColorProperty>(defv) );
            }
            //
            else if ( tname == TN(PropertyInterface*) ) {
              ioDataSet.set( name, inG->getProperty(defv) );
            }
          }
  }
  delete defIt;
}

