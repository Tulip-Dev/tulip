/*
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

#ifndef _TULIPWITHPARAMETER
#define _TULIPWITHPARAMETER

#include <string>
#include <typeinfo>
#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {

class Graph;
class DataSet;
template<class itType >
struct Iterator;

/**
 * @ingroup Plugins
 * @brief The ParameterDirection enum describes how a parameter is passed to a plugin
 * The parameter can be seen as passing parameters to a C++ functions and works as follow:
 * @list
 * @li IN_PARAM: The parameter is passes by value
 * @li OUT_PARAM: The parameter is a return value
 * @li INOUT_PARAM: The parameter is passed by reference.
 * @endlist
 */
enum ParameterDirection { IN_PARAM=0, OUT_PARAM = 1, INOUT_PARAM = 2 };

/**
 * @ingroup Plugins
 * @brief Describes a plugin's parameter.
 *
 * A plugin parameter consists of the following information:
 * @list
 * @li A name (std::string) which can be used to retrieve the value of the parameter when running the plugin.
 * @li A type (std::string) which is the C++ typename of the parameter.
 * @li A help string (std::string) which gives additional information about the parameter and its possible values.
 * @li A default value which is mapped on the parameter if no value has been entered by the user.
 * @li The mandatory flag (bool) which tells if the parameter is optional or not.
 * @li A ParameterDirection (enum).
 * @endlist
 *
 * @see tlp::ParameterDirection
 * @see tlp::ParameterDescriptionList
 * @see tlp::WithParameter
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
  /**
   * @return The parameter's name
   */
  const std::string& getName() const {
    return name;
  }
  /**
   * @return The parameter's C++ type name
   */
  const std::string& getTypeName() const {
    return type;
  }
  /**
   * @return The parameter's help string
   */
  const std::string& getHelp() const {
    return help;
  }
  /**
   * @return The parameter's default value.
   */
  const std::string& getDefaultValue() const {
    return defaultValue;
  }
  /**
   * @brief Set the parameter's default value.
   */
  void setDefaultValue(const std::string& defVal) {
    defaultValue = defVal;
  }
  /**
   * @return Whether the parameter is mandatory or not.
   */
  bool isMandatory() const {
    return mandatory;
  }
  /**
   * @return The parameter's direction
   */
  ParameterDirection getDirection() const {
    return direction;
  }
  /**
   * @return Set the parameter's direction
   */
  void setDirection(ParameterDirection dir) {
    direction = dir;
  }
};

/**
  * @ingroup Plugins
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
    * @param direction The parameter's direction (see tlp::ParameterDirection for details)
    * @return void
    **/
  template<typename T> void add(const std::string& parameterName,
                                const std::string& help,
                                const std::string& defaultValue,
                                bool isMandatory = true,
                                ParameterDirection direction = IN_PARAM) {
    for (unsigned int i = 0; i < parameters.size(); ++i) {
      if (parameters[i].getName() == parameterName) {
#ifndef NDEBUG
        tlp::warning() << "ParameterDescriptionList::addVar " << parameterName << " already exists" << std::endl;
#endif
        return;
      }
    }

    ParameterDescription newParameter(parameterName,
                                      typeid(T).name(),
                                      help,
                                      defaultValue,
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
   * @brief Sets the direction of a parameter.
   *
   * @param parameterName The name of the parameter to set the value of.
   * @param dir The new direction for this parameter.
   * @return void
   **/
  void setDirection(const std::string& parameterName,
                    ParameterDirection direction);

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
  void buildDefaultDataSet(DataSet& ioDataSet, Graph* inG = NULL) const;

  /**
   * @brief Returns the number of parameters.
   *
   * @return The number of parameters
   **/
  inline unsigned int size() const {
    return parameters.size();
  }

  /**
   * @brief Test if the list is empty
   *
   * @return bool true if the parameter description list is empty, false otherwise.
   **/
  inline bool empty() const {
    return parameters.empty();
  }

private:
  ParameterDescription* getParameter(const std::string& parameterName);
  std::vector<ParameterDescription> parameters;
};


#define HTML_HELP_OPEN() "<!DOCTYPE html><html><head>\
<style type=\"text/css\">.body { font-family: \"Segoe UI\", Candara, \"Bitstream Vera Sans\", \"DejaVu Sans\", \"Bitstream Vera Sans\", \"Trebuchet MS\", Verdana, \"Verdana Ref\", sans-serif; }\
    .paramtable { width: 100%; border: 0px; border-bottom: 1px solid #C9C9C9; padding: 5px; }\
    .help { font-style: italic; font-size: 90%; }</style>\
</head><body><table border=\"0\" class=\"paramtable\">"

#define HTML_HELP_DEF(A,B) "<tr><td><b>" A "</b><td>" B "</td></tr>"

#define HTML_HELP_BODY() "</table><p class=\"help\">"

#define HTML_HELP_CLOSE() "</p></body></html>"

/**
 * @ingroup Plugins
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
  const tlp::ParameterDescriptionList& getParameters() const;

  /**
   * @brief Adds an IN parameter to the plug-in.
   *
   * @param name The name of the parameter to add.
   * @param help A description of the parameter, that will be displayed to the user. Defaults to "".
   * @param defaultValue The default value the parameter should take, to be the initial value in the GUI. Defaults to "".
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T>
  void addInParameter(const std::string &name,
                      const std::string &help,
                      const std::string &defaultValue,
                      bool isMandatory = true) {
    parameters.template add<T>(name, help, defaultValue, isMandatory, IN_PARAM);
  }

  /**
   * @brief Adds an OUT parameter to the plug-in.
   *
   * @param name The name of the parameter to add.
   * @param help A description of the parameter, that will be displayed to the user. Defaults to "".
   * @param defaultValue The default value the parameter should take, to be the initial value in the GUI. Defaults to "".
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T>
  void addOutParameter(const std::string &name,
                       const std::string &help=std::string(),
                       const std::string &defaultValue = std::string(),
                       bool isMandatory = true) {
    parameters.template add<T>(name, help, defaultValue, isMandatory, OUT_PARAM);
  }

  /**
   * @brief Adds an INOUT parameter to the plug-in.
   *
   * @param name The name of the parameter to add.
   * @param help A description of the parameter, that will be displayed to the user. Defaults to "".
   * @param defaultValue The default value the parameter should take, to be the initial value in the GUI. Defaults to "".
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T>
  void addInOutParameter(const std::string &name,
                         const std::string &help=std::string(),
                         const std::string &defaultValue = std::string(),
                         bool isMandatory = true) {
    parameters.template add<T>(name, help, defaultValue, isMandatory, INOUT_PARAM);
  }

  /**
   * @brief Adds an IN parameter to the plug-in (deprecated)
   *
   * @see addInParameter()
   * @see addOutParameter()
   *
   * @param name The name of the parameter to add.
   * @param help A description of the parameter, that will de displayed to the user. Defaults to 0.
   * @param defaultValue The default value the parameter should take, to be the initial value in the GUI. Defaults to 0.
   * @param isMandatory Whether this parameter requires a value or not. Defaults to true.
   * @return void
   **/
  template<typename T>
  _DEPRECATED void addParameter(const std::string &name,
                                const std::string &help=std::string(),
                                const std::string &defaultValue = std::string(),
                                bool isMandatory = true) {
    addInParameter<T>(name, help, defaultValue, isMandatory);
  }

  /**
   * @brief indicates whether the embedded parameters require some user input
   *
   * @return true if an input parameter or a property output parameter exists
   **/
  bool inputRequired() const;

protected:

  /**
   * @brief The internal structure storing the parameters.
   **/
  ParameterDescriptionList parameters;
};


}
#endif
