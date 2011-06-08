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
#ifndef _TULIPWITHPARAMETER
#define _TULIPWITHPARAMETER

#include <string>

#include <tulip/tulipconf.h>
#include <tulip/Iterator.h>
#include <tulip/Reflect.h>

namespace tlp {
/** \addtogroup plugins */
/*@{*/

/**
  * @brief This class describes parameters taken by a plugin.
  *
  * It is used by WithParameter to store parameters.
  * Each parameter is identified by a name, has a default value, a value, a help string, and a boolean indicating whether it is mandatory or optional.
  **/
struct TLP_SCOPE ParameterList {

  /**
    * @brief Adds a new parameter of type T to the list.
    *
    * @param parameterName The name of the parameter.
    * @param inHelp The help string of this parameter. Defaults to std::string().
    * @param inDefValue The default value of this parameter. Defaults to std::string().
    * @param isMandatory Whether this parameter is mandatory or optional. Defaults to true.
    * @return void
    **/
  template<typename T> void add(const std::string parameterName,
                                const std::string inHelp = std::string(),
                                const std::string inDefValue = std::string(),
                                bool isMandatory = true){
    if (parameters.find(parameterName) == parameters.end()) {
      Parameter newParameter(std::string(typeid(T).name()), inHelp, inDefValue, isMandatory);
      parameters[parameterName] = newParameter;
    }
    #ifndef NDEBUG
    else {
      std::cerr << "ParameterList::addVar " << parameterName << " already exists" << std::endl;
    }
    #endif
  }

  /**
  * @brief Retrieves the value of a parameter.
  *
  * @param parameterName The name of the parameter to retrieve the value of.
  * @return The value of the parameter, or an empty string:string
  **/
  std::string getValue(std::string parameterName) const;

  /**
   * @brief Erases a parameter from the list
   *
   * @param parameterName The name of the parameter to erase.
   * @return void
   **/
  void erase(std::string parameterName);
  
  /**
   * @brief Sets the value of a parameter.
   *
   * @param parameterName The name of the parameter to set the value of.
   * @param value The new value for this parameter.
   * @return void
   **/
  void setDefaultValue(std::string parameterName, std::string value);

  /**
   * @brief Checks whether a parameter exists.
   *
   * @param parameterName The name of the parameter to check the existence of.
   * @return bool Whether the parameter exists.
   **/
  bool hasField(std::string parameterName) const;

  
  /**
   * @brief Retrieves an Iterator on the names of the parameters.
   *
   * @return An iterator over the names of the parameters :Iterator< std::string >*
   **/
  tlp::Iterator<std::string>* getParametersNames() const;

  
  /**
   * @brief Retrieves the help string of a parameter.
   *
   * @param parameterName The name of the parameter to retrieve the help string of.
   * @return the help string of this parameter:string
   **/
  std::string getHelp(std::string parameterName) const;

  /**
   * @brief retrieves the default value of a parameter.
   *
   * @param parameterName The name of the parameter to retrieve the default value of.
   * @return The default value of the parameter:string
   **/
  std::string getDefaultValue(std::string parameterName) const;

  
  /**
   * @brief Retrieves whether a parameter is mandatory or optional.
   *
   * @param parameterName The name of the parameter for which to check if it is mandatory or optional.
   * @return bool true if the parameter is mandatory, false if it is optional. 
   **/
  bool isMandatory(std::string parameterName) const;
  
  /**
   * @brief Builds a DataSet containing the default values for each parameter.
   * If the DataSet has a key which is equal to a parameter's name, the existing value is kept.
   *
   * @param ioDataSet The input dataset on which the parameters names will be associated with their default values.
   * @param inG A graph on which to create properties if any parameter is of a property type. Defaults to 0.
   * @return void
   **/
  void buildDefaultDataSet( DataSet & ioDataSet, Graph * inG = 0 ) const;
private:
  struct Parameter {
    Parameter() {}
    Parameter(std::string data, std::string help, std::string defaultValue, bool mandatory): data(data), help(help), defaultValue(defaultValue), mandatory(mandatory) {
    }
    std::string data;
    std::string help;
    std::string defaultValue;
    bool mandatory;
  };
  
  std::map<std::string, Parameter> parameters;
};

// Standard ParameterList's HTML Help

#define HTML_HELP_OPEN()  "<table><tr><td>" \
"<table CELLSPACING=5 bgcolor=\"#EEEEEE\">"
#define HTML_HELP_DEF(A,B)  "<tr><td><b>" A "</b></td><td><FONT COLOR=\"red\">" B "</td></tr>"
#define HTML_HELP_BODY()  "</table></td><td>"
#define HTML_HELP_CLOSE() "</td></tr></table>"

/*
 * Usage :
 * 
 * const char * paramHelp[] = {
 * // property
 * HTML_HELP_OPEN() \
 * HTML_HELP_DEF( "type", "DoubleProperty" ) \
 * HTML_HELP_BODY() \
 * "This metricProxy is used to affect scalar values to graph items." \
 * "The meaning of theses values depends of the choosen color model." \
 * HTML_HELP_CLOSE(),
 * 
 * // colormodel
 * HTML_HELP_OPEN() \
 * HTML_HELP_DEF( "type", "int" ) \
 * HTML_HELP_DEF( "values", "[0,1]" ) \
 * HTML_HELP_DEF( "default", "0" ) \
 * HTML_HELP_BODY() \
 * "This value defines the type of color interpolation. Following values are valid :" \
 * "<ul><li>0: linear mapping on hue ;</li><li>1: RGB interpolation</li></ul>" \
 * HTML_HELP_CLOSE(),
};

...
addParameter<DoubleProperty>("property",paramHelp[0]);
addParameter<int>("colormodel",paramHelp[1]);
...

*/
  
/**
 * @brief This class describes parameters on a plug-in.
 * 
 * These parameters can be of any type, and are used to generate a GUI that will be shown when the plug-in in invoked by the user.
 * It is mainly used by algorithms to display options to the user, e.g. a clustering algorithm can let the user choose which measure to use.
 **/
struct TLP_SCOPE WithParameter {
  
  /**
   * @brief Retrieves the parameters.
   *
   * @return :ParameterList the parameters of the plug-in.
   **/
  tlp::ParameterList getParameters();
  
  /**
   * @brief Adds a parameter to the plug-in.
   *
   * @param str The name of the parameter to add.
   * @param inHelp A description of the parameter, that will de displayed to the user. Defaults to 0.
   * @param inDefValue The default value the parameter should take, to be the initial value in the GUI. Defaults to 0.
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T> void addParameter(const std::string &str,
           const std::string &inHelp=std::string(),
           const std::string &inDefValue = std::string(),
					 bool isMandatory = true)
  { parameters.template add<T>(str,inHelp,inDefValue,isMandatory); }
protected:
  
  /**
   * @brief The internal structure storing the parameters.
   **/
  ParameterList parameters;
};
/*@}*/

}
#endif


