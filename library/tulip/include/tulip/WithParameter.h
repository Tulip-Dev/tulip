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
#include <typeinfo>

#include <tulip/tulipconf.h>
#include <tulip/Iterator.h>
#include <tulip/DataSet.h>

namespace tlp {
/** \addtogroup plugins */
/*@{*/
/*
 * @brief indicates the direction type of a parameter
 **/
enum ParameterDirection { IN_PARAM=0, OUT_PARAM = 1, INOUT_PARAM = 2 };
/**
 * @brief Describes a parameter with a type, default value, whether or not is it mandatory and a help string describing what this parameter should be used for.
 **/
class TLP_SCOPE ParameterDescription {
private:
  std::string name;
  std::string type;
  std::string help;
  std::string defaultValue;
  bool mandatory;
  ParameterDirection direction;
public:
  ParameterDescription() {}
  ParameterDescription(const std::string& name, const std::string& type,
                       const std::string& help, const std::string& defaultValue,
                       bool mandatory, ParameterDirection direction)
    : name(name), type(type), help(help),
      defaultValue(defaultValue), mandatory(mandatory), direction(direction) {
  }
  const std::string& getName() const {
    return name;
  }
  const std::string& getTypeName() const {
    return type;
  }
  const std::string& getHelp() const {
    return help;
  }
  const std::string& getDefaultValue() const {
    return defaultValue;
  }
  void setDefaultValue(const std::string& defVal) {
    defaultValue = defVal;
  }
  bool isMandatory() {
    return mandatory;
  }
  ParameterDirection getDirection() const {
    return direction;
  }
};

/**
  * @brief This class describes parameters taken by a plugin.
  *
  * It is used by WithParameter to store parameters.
  * Each parameter is identified by a name, has a default value, a value, a help string, and a boolean indicating whether it is mandatory or optional.
  **/
struct TLP_SCOPE ParameterDescriptionList {

  ParameterDescriptionList() {}

  /**
    * @brief Adds a new parameter of type T to the list.
    *
    * @param parameterName The name of the parameter.
    * @param help The help string of this parameter. Defaults to std::string().
    * @param defaultValue The default value of this parameter. Defaults to std::string().
    * @param isMandatory Whether this parameter is mandatory or optional. Defaults to true.
    * @return void
    **/
  template<typename T> void add(const char* parameterName,
                                const char* help = NULL,
                                const char* defaultValue = NULL,
                                bool isMandatory = true,
                                ParameterDirection direction = IN_PARAM) {
    for (unsigned int i = 0; i < parameters.size(); ++i) {
      if (parameters[i].getName() == parameterName) {
#ifndef NDEBUG
        std::cerr << "ParameterDescriptionList::addVar " << parameterName << " already exists" << std::endl;
#endif
        return;
      }
    }

    ParameterDescription newParameter(parameterName,
                                      typeid(T).name(),
                                      help ? help : "",
                                      defaultValue ? defaultValue : "",
                                      isMandatory, direction);
    parameters.push_back(newParameter);
  }

  /**
   * @brief Retrieves an Iterator on the parameters.
   *
   * @return An iterator over the parameters :Iterator<ParameterDescription>*
   **/
  tlp::Iterator<ParameterDescription>* getParameters() const;

  /**
   * @brief retrieves the default value of a parameter.
   *
   * @param parameterName The name of the parameter to retrieve the default value of.
   * @return The default value of the parameter:string
   **/
  const std::string& getDefaultValue(const std::string& parameterName) const;

  /**
   * @brief Sets the default value of a parameter.
   *
   * @param parameterName The name of the parameter to set the value of.
   * @param value The new value for this parameter.
   * @return void
   **/
  void setDefaultValue(const std::string& parameterName,
                       const std::string& value);

  /**
   * @brief Retrieves whether a parameter is mandatory or optional.
   *
   * @param parameterName The name of the parameter for which to check if it is mandatory or optional.
   * @return bool true if the parameter is mandatory, false if it is optional.
   **/
  bool isMandatory(const std::string& parameterName) const;

  /**
   * @brief Builds a DataSet containing the default values for each parameter.
   * If the DataSet has a key which is equal to a parameter's name, the existing value is kept.
   *
   * @param ioDataSet The input dataset on which the parameters names will be associated with their default values.
   * @param inG A graph on which to create properties if any parameter is of a property type. Defaults to 0.
   * @return void
   **/
  void buildDefaultDataSet(DataSet& ioDataSet, Graph* inG = NULL ) const;

  unsigned int size() const {
    return parameters.size();
  }
private:
  ParameterDescription* getParameter(const std::string& parameterName);
  template<typename TYPEINTERFACE>
  void insertData(tlp::DataSet &,const std::string &param,const std::string &defaultValue) const;
  std::vector<ParameterDescription> parameters;
};

// Standard ParameterDescriptionList's HTML Help

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
 * "The meaning of theses values depends on the chosen color model." \
 * HTML_HELP_CLOSE(),
 *
 * // colormodel
 * HTML_HELP_OPEN() \
 * HTML_HELP_DEF( "type", "int" ) \
 * HTML_HELP_DEF( "values", "[0,1]" ) \
 * HTML_HELP_DEF( "default", "0" ) \
 * HTML_HELP_BODY() \
 * "This value defines the type of color interpolation. The following values are valid:" \
 * "<ul><li>0: linear mapping on hue;</li><li>1: RGB interpolation</li></ul>" \
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
   * @return :ParameterDescriptionList the parameters of the plug-in.
   **/
  const tlp::ParameterDescriptionList& getParameters();

  /**
   * @brief Adds an IN parameter to the plug-in.
   *
   * @param str The name of the parameter to add.
   * @param inHelp A description of the parameter, that will be displayed to the user. Defaults to 0.
   * @param inDefValue The default value the parameter should take, to be the initial value in the GUI. Defaults to 0.
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T> void addInParameter(const char* str,
      const char* inHelp=NULL,
      const char* inDefValue = NULL,
      bool isMandatory = true) {
    parameters.template add<T>(str,inHelp,inDefValue,isMandatory, IN_PARAM);
  }

  /**
   * @brief Adds an OUT parameter to the plug-in.
   *
   * @param str The name of the parameter to add.
   * @param inHelp A description of the parameter, that will be displayed to the user. Defaults to 0.
   * @param inDefValue The default value the parameter should take, to be the initial value in the GUI. Defaults to 0.
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T> void addOutParameter(const char* str,
      const char* inHelp=NULL,
      const char* inDefValue = NULL,
      bool isMandatory = true) {
    parameters.template add<T>(str,inHelp,inDefValue,isMandatory, OUT_PARAM);
  }

  /**
   * @brief Adds an INOUT parameter to the plug-in.
   *
   * @param str The name of the parameter to add.
   * @param inHelp A description of the parameter, that will be displayed to the user. Defaults to 0.
   * @param inDefValue The default value the parameter should take, to be the initial value in the GUI. Defaults to 0.
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T> void addInOutParameter(const char* str,
      const char* inHelp=NULL,
      const char* inDefValue = NULL,
      bool isMandatory = true) {
    parameters.template add<T>(str,inHelp,inDefValue,isMandatory, INOUT_PARAM);
  }

  /**
   * @brief Adds an IN parameter to the plug-in (deprecated)
   *
   * @param parameterName The name of the parameter to add.
   * @param help A description of the parameter, that will de displayed to the user. Defaults to 0.
   * @param defaultValue The default value the parameter should take, to be the initial value in the GUI. Defaults to 0.
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T> void addParameter(const char* parameterName,
                                         const char* help= NULL,
                                         const char* defaultValue = NULL,
                                         bool isMandatory = true) {
    addInParameter<T>(parameterName, help, defaultValue, isMandatory);
  }
protected:

  /**
   * @brief The internal structure storing the parameters.
   **/
  ParameterDescriptionList parameters;
};
/*@}*/

}
#endif
