/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
namespace tlp {
  typedef tlp::Iterator<tlp::ParameterDescription> IteratorParameter;
}

template <typename T>
void addParameter(tlp::WithParameter *withParam,
                  const std::string & str,
                  const std::string & inHelp = "",
                  const std::string & inDefValue = "",
                  bool isMandatory = true,
                  bool inParam = true,
                  bool outParam = false,
                  const std::string &valuesDescription = "") {
  if (inParam && !outParam) {
    withParam->addInParameter<T>(str, inHelp, inDefValue, isMandatory, valuesDescription);
  } else if (!inParam && outParam) {
    withParam->addOutParameter<T>(str, inHelp, inDefValue, isMandatory, valuesDescription);
  } else if (inParam && outParam) {
    withParam->addInOutParameter<T>(str, inHelp, inDefValue, isMandatory, valuesDescription);
  }
}
%End

namespace tlp {
%TypeHeaderCode
#include <tulip/WithParameter.h>
#include <tulip/ColorScale.h>
#include <tulip/StringCollection.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
%End

      enum ParameterDirection /BaseType=IntEnum/ { IN_PARAM=tlp::ParameterDirection::IN_PARAM, OUT_PARAM = tlp::ParameterDirection::OUT_PARAM, INOUT_PARAM = tlp::ParameterDirection::INOUT_PARAM };

class ParameterDescription {

public:

  const std::string& getName() const;

  const std::string& getTypeName() const;

  const std::string& getHelp() const;

  const std::string& getDefaultValue() const;

  bool isMandatory() const;

  tlp::ParameterDirection getDirection() const;

};

typedef tlp::Iterator<tlp::ParameterDescription> IteratorParameter;

struct ParameterDescriptionList {

  tlp::IteratorParameter* getParameters() const /TransferBack, TypeHint="Iterator[tlp.ParameterDescription]"/;

  unsigned int size() const;

  bool empty() const;

};


struct WithParameter {

%Docstring
This class describes parameters for a plug-in.
These parameters can be of multiple types, and are used to generate a GUI 
that will be shown when the plug-in in invoked by the user.
%End

// ==================================================================================

  void addStringParameter(const std::string & str,
                          const std::string & help = "",
                          const std::string & defaultValue = "",
                          bool isMandatory = true,
                          bool inParam = true, bool outParam = false,
                          const std::string &valuesDescription = "");
%Docstring
tlp.WithParameter.addStringParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a string parameter to the plug-in.

:param name: the name of the string parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<std::string>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addStringCollectionParameter(const std::string & str,
                                    const std::string & help = "",
                                    const std::string & defaultValue = "",
                                    bool isMandatory = true,
                                    bool inParam = true, bool outParam = false,
                                    const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addStringCollectionParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.StringCollection` parameter to the plug-in.

:param name: the name of the :class:`tlp.StringCollection` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "entry1;entry2;...;entryN") gives the list of the possible values of the parameter to be defined; these values are separated by the character ';' and the first value is the default one.
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::StringCollection>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End


// ==================================================================================

  void addIntegerParameter(const std::string & str,
                           const std::string & help = "",
                           const std::string & defaultValue = "",
                           bool isMandatory = true,
                           bool inParam = true, bool outParam = false,
                           const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addIntegerParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds an integer parameter to the plug-in.

:param name: the name of the integer parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "[0-9]+") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<int>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

 void addUnsignedIntegerParameter(const std::string & str,
                                  const std::string & help = "",
                                  const std::string & defaultValue = "",
                                  bool isMandatory = true,
                                  bool inParam = true, bool outParam = false,
                                  const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addUnsignedIntegerParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 5.0

Adds an unsigned integer parameter to the plug-in.

:param name: the name of the unsigned integer parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "[0-9]+") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
 addParameter<unsigned int>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addFloatParameter(const std::string & str,
                         const std::string & help = "",
                         const std::string & defaultValue = "",
                         bool isMandatory = true,
                         bool inParam = true, bool outParam = false,
                         const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addFloatParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a floating point number parameter to the plug-in.

:param name: the name of the floating point number parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "[0-9]*[.][0-9]+") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<double>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addBooleanParameter(const std::string & str,
                           const std::string & help = "",
                           const std::string & defaultValue = "",
                           bool isMandatory = true,
                           bool inParam = true, bool outParam = false,
                           const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addBooleanParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a boolean parameter to the plug-in.

:param name: the name of the boolean parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value ("true" or "false") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<bool>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addDoublePropertyParameter(const std::string & str,
                                  const std::string & help = "",
                                  const std::string & defaultValue = "",
                                  bool isMandatory = true,
                                  bool inParam = true, bool outParam = false,
                                  const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addDoublePropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.DoubleProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.DoubleProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::DoubleProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addDoubleVectorPropertyParameter(const std::string & str,
                                        const std::string & help = "",
                                        const std::string & defaultValue = "",
                                        bool isMandatory = true,
                                        bool inParam = true, bool outParam = false,
                                        const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addDoubleVectorPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 4.2

Adds a :class:`tlp.DoubleVectorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.DoubleVectorProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::DoubleVectorProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addIntegerPropertyParameter(const std::string & str,
                                   const std::string & help = "",
                                   const std::string & defaultValue = "",
                                   bool isMandatory = true,
                                   bool inParam = true, bool outParam = false,
                                   const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addIntegerPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.IntegerProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.IntegerProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::IntegerProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addIntegerVectorPropertyParameter(const std::string & str,
                                         const std::string & help = "",
                                         const std::string & defaultValue = "",
                                         bool isMandatory = true,
                                         bool inParam = true, bool outParam = false,
                                         const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addIntegerVectorPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 4.2

Adds a :class:`tlp.IntegerVectorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.IntegerProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::IntegerVectorProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addLayoutPropertyParameter(const std::string & str,
                                  const std::string & help = "",
                                  const std::string & defaultValue = "",
                                  bool isMandatory = true,
                                  bool inParam = true, bool outParam = false,
                                  const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addLayoutPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.LayoutProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.LayoutProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::LayoutProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addCoordVectorPropertyParameter(const std::string & str,
                                       const std::string & help = "",
                                       const std::string & defaultValue = "",
                                       bool isMandatory = true,
                                       bool inParam = true, bool outParam = false,
                                       const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addCoordVectorPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 4.2

Adds a :class:`tlp.CoorVectorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.LayoutProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::CoordVectorProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addSizePropertyParameter(const std::string & str,
                                const std::string & help = "",
                                const std::string & defaultValue = "",
                                bool isMandatory = true,
                                bool inParam = true, bool outParam = false,
                                const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addSizePropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.SizeProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.SizeProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
    addParameter<tlp::SizeProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addSizeVectorPropertyParameter(const std::string & str,
                                      const std::string & help = "",
                                      const std::string & defaultValue = "",
                                      bool isMandatory = true,
                                      bool inParam = true, bool outParam = false,
                                      const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addSizeVectorPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 4.2

Adds a :class:`tlp.SizeVectorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.SizeVectorProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::SizeVectorProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addColorPropertyParameter(const std::string & str,
                                 const std::string & help = "",
                                 const std::string & defaultValue = "",
                                 bool isMandatory = true,
                                 bool inParam = true, bool outParam = false,
                                 const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addColorPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.ColorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.ColorProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::ColorProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addColorVectorPropertyParameter(const std::string & str,
                                       const std::string & help = "",
                                       const std::string & defaultValue = "",
                                       bool isMandatory = true,
                                       bool inParam = true, bool outParam = false,
                                       const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addColorVectorPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 4.2

Adds a :class:`tlp.ColorVectorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.ColorVectorProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::ColorVectorProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addBooleanPropertyParameter(const std::string & str,
                                   const std::string & help = "",
                                   const std::string & defaultValue = "",
                                   bool isMandatory = true,
                                   bool inParam = true, bool outParam = false,
                                   const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addBooleanPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.BooleanProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.BooleanProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::BooleanProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addBooleanVectorPropertyParameter(const std::string & str,
                                         const std::string & help = "",
                                         const std::string & defaultValue = "",
                                         bool isMandatory = true,
                                         bool inParam = true, bool outParam = false,
                                         const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addBooleanVectorPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 4.2

Adds a :class:`tlp.BooleanVectorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.BooleanVectorProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::BooleanVectorProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addStringPropertyParameter(const std::string & str,
                                  const std::string & help = "",
                                  const std::string & defaultValue = "",
                                  bool isMandatory = true,
                                  bool inParam = true, bool outParam = false,
                                  const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addStringPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.StringProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.StringProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::StringProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addStringVectorPropertyParameter(const std::string & str,
                                        const std::string & help = "",
                                        const std::string & defaultValue = "",
                                        bool isMandatory = true,
                                        bool inParam = true, bool outParam = false,
                                        const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addStringVectorPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 4.2

Adds a :class:`tlp.StringVectorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.StringVectorProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::StringVectorProperty>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addColorParameter(const std::string & str,
                         const std::string & help = "",
                         const std::string & defaultValue = "",
                         bool isMandatory = true,
                         bool inParam = true, bool outParam = false,
                         const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addColorParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.Color` parameter to the plug-in.

:param name: the name of the :class:`tlp.Color` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "(R, G, B, A)", e.g. for red : "(255,0,0,255)") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::Color>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addColorScaleParameter(const std::string & str,
                              const std::string & help = "",
                              const std::string & defaultValue = "",
                              bool isMandatory = true,
                              bool inParam = true, bool outParam = false,
                              const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addColorScaleParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.ColorScale` parameter to the plug-in.

:param name: the name of the :class:`tlp.ColorScale` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "((R, G, B, A), ..., (R, G, B, A))", e.g. gradient from black to white : "((0,0,0,255), (255,255,255,255))") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::ColorScale>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

  void addPropertyParameter(const std::string & str,
                            const std::string & help = "",
                            const std::string & defaultValue = "",
                            bool isMandatory = true,
                            bool inParam = true, bool outParam = false,
                            const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

Adds a :class:`tlp.PropertyInterface` parameter to the plug-in.

:param name: the name of the :class:`tlp.PropertyInterface` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
  addParameter<tlp::PropertyInterface*>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

 void addNumericPropertyParameter(const std::string & str,
                                  const std::string & help = "",
                                  const std::string & defaultValue = "",
                                  bool isMandatory = true,
                                  bool inParam = true, bool outParam = false,
                                  const std::string &valuesDescription = "");

%Docstring
tlp.WithParameter.addNumericPropertyParameter(name, help="", defaultValue="", isMandatory=True, inParam=True, outParam=False, valuesDescription="")

.. versionadded:: 4.10

Adds a :class:`tlp.NumericProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.NumericProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
:param valuesDescription: An optional description of the values the parameter can take.
:type valuesDescription: string
%End

%MethodCode
 addParameter<tlp::NumericProperty*>(sipCpp, *a0, *a1, *a2, a3, a4, a5, *a6);
%End

// ==================================================================================

void addFileParameter(const std::string & str,
                      bool mustExist = true,
                      const std::string & help = "",
                      const std::string & defaultValue = "",
                      bool isMandatory = true);

%Docstring
tlp.WithParameter.addFileParameter(name, mustExist=True, help="", defaultValue="", isMandatory=True)

.. versionadded:: 4.9

Adds a file parameter to the plug-in. A dialog will be created by the Tulip GUI in order to select the file.

:param name: the name of the file parameter to add
:type name: string
:param mustExist: specify if the file must exist or not. This is used as a hint by the Tulip GUI to correctly configure the dialog that will be created for selecting the file.
:type mustExist: boolean
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean
%End

%MethodCode
  if (a1) {
    addParameter<std::string>(sipCpp, std::string("file::") + *a0, *a2, *a3, a4, true, false);
  } else {
    addParameter<std::string>(sipCpp, std::string("anyfile::") + *a0, *a2, *a3, a4, true, false);
  }

%End

// ==================================================================================

void addDirectoryParameter(const std::string & str,
                           const std::string & help = "",
                           const std::string & defaultValue = "",
                           bool isMandatory = true);

%Docstring
tlp.WithParameter.addDirectoryParameter(name, help="", defaultValue="", isMandatory=True)

.. versionadded:: 4.9

Adds a directory parameter to the plug-in. A dialog will be created by the Tulip GUI in order to select an existing folder.

:param name: the name of the directory parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean
%End

%MethodCode
  addParameter<std::string>(sipCpp, std::string("dir::") + *a0, *a1, *a2, a3, true, false);
%End

};


};
