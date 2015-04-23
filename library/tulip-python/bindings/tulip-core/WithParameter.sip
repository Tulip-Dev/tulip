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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
template <typename T>
void addParameter(tlp::WithParameter *withParam,
             const std::string & str,
             const std::string & inHelp = "",
             const std::string & inDefValue = "",
             bool isMandatory = true,
             bool inParam = true,
             bool outParam = false) {
    if (inParam && !outParam) {
        withParam->addInParameter<T>(str, inHelp, inDefValue, isMandatory);
    } else if (!inParam && outParam) {
        withParam->addOutParameter<T>(str, inHelp, inDefValue, isMandatory);
    } else if (inParam && outParam) {
        withParam->addInOutParameter<T>(str, inHelp, inDefValue, isMandatory);
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

struct WithParameter {

%Docstring
This class describes parameters for a plug-in.
These parameters can be of multiple types, and are used to generate a GUI 
that will be shown when the plug-in in invoked by the user.
%End

// ==================================================================================
	
    void addStringParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);
%Docstring
tlp.WithParameter.addStringParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End
			
%MethodCode
    addParameter<std::string>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addStringCollectionParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addStringCollectionParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

Adds a :class:`tlp.StringCollection` parameter to the plug-in.

:param name: the name of the :class:`tlp.StringCollection` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "entry1;entry2;...;entryN") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
:param inParam: defines if the parameter is an input one (can be read by the plugin)
:type inParam: boolean
:param outParam: defines if the parameter is an output one (can be written by the plugin and retrieved after its execution)
:type outParam: boolean
%End

%MethodCode
    addParameter<tlp::StringCollection>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End


// ==================================================================================

    void addIntegerParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);
			
%Docstring
tlp.WithParameter.addIntegerParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<int>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addFloatParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addFloatParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<double>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addBooleanParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addBooleanParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End
						
%MethodCode
    addParameter<bool>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End		

// ==================================================================================

    void addDoublePropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addDoublePropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::DoubleProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addDoubleVectorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addDoubleVectorPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::DoubleVectorProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addIntegerPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addIntegerPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::IntegerProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addIntegerVectorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addIntegerVectorPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::IntegerVectorProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addLayoutPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addLayoutPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::LayoutProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addCoordVectorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addCoordVectorPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::CoordVectorProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addSizePropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addSizePropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::SizeProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addSizeVectorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addSizeVectorPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::SizeVectorProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addColorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);
			
%Docstring
tlp.WithParameter.addColorPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::ColorProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addColorVectorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);
			
%Docstring
tlp.WithParameter.addColorVectorPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::ColorVectorProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addBooleanPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addBooleanPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::BooleanProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addBooleanVectorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addBooleanVectorPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::BooleanVectorProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addStringPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addStringPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::StringProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addStringVectorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addStringVectorPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::StringVectorProperty>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addColorParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addColorParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::Color>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

   void addColorScaleParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addColorScaleParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::ColorScale>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

// ==================================================================================

    void addPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
            bool isMandatory = true,
            bool inParam = true, bool outParam = false);

%Docstring
tlp.WithParameter.addPropertyParameter(name[, help = "", defaultValue = "", isMandatory = True, inParam = True, outParam = False])

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
%End			
			
%MethodCode
    addParameter<tlp::PropertyInterface*>(sipCpp, (*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3, a4, a5);
%End

};
};
