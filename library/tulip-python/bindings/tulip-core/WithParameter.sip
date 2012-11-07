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
			bool isMandatory = true);
%Docstring
tlp.WithParameter.addStringParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a string parameter to the plug-in.

:param name: the name of the string parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End
			
%MethodCode
    sipCpp->addParameter<std::string>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addStringCollectionParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addStringCollectionParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.StringCollection` parameter to the plug-in.

:param name: the name of the :class:`tlp.StringCollection` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "entry1;entry2;...;entryN") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End

%MethodCode
    sipCpp->addParameter<tlp::StringCollection>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End


// ==================================================================================

    void addIntegerParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);
			
%Docstring
tlp.WithParameter.addIntegerParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds an integer parameter to the plug-in.

:param name: the name of the integer parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "[0-9]+") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<int>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addFloatParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addFloatParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a floating point number parameter to the plug-in.

:param name: the name of the floating point number parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "[0-9]*[.][0-9]+") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<double>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addBooleanParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addBooleanParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a boolean parameter to the plug-in.

:param name: the name of the boolean parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value ("true" or "false") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End
						
%MethodCode
    sipCpp->addParameter<bool>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End		

// ==================================================================================

    void addDoublePropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addDoublePropertyParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.DoubleProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.DoubleProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::DoubleProperty>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addIntegerPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addIntegerPropertyParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.IntegerProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.IntegerProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::IntegerProperty>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addLayoutPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addLayoutPropertyParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.LayoutProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.LayoutProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::LayoutProperty>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addSizePropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addSizePropertyParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.SizeProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.SizeProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::SizeProperty>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addColorPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);
			
%Docstring
tlp.WithParameter.addColorPropertyParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.ColorProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.ColorProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::ColorProperty>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addBooleanPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addBooleanPropertyParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.BooleanProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.BooleanProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::BooleanProperty>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addStringPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addStringPropertyParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.StringProperty` parameter to the plug-in.

:param name: the name of the :class:`tlp.StringProperty` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::StringProperty>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addColorParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addColorParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.Color` parameter to the plug-in.

:param name: the name of the :class:`tlp.Color` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "(R, G, B, A)", e.g. for red : "(255,0,0,255)") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::Color>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

   void addColorScaleParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addColorScaleParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.ColorScale` parameter to the plug-in.

:param name: the name of the :class:`tlp.ColorScale` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "((R, G, B, A), ..., (R, G, B, A))", e.g. gradient from black to white : "((0,0,0,255), (255,255,255,255))") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::ColorScale>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

// ==================================================================================

    void addPropertyParameter(const std::string & str,
            const std::string & inHelp = "",
            const std::string & inDefValue = "",
			bool isMandatory = true);

%Docstring
tlp.WithParameter.addPropertyParameter(name[, help = None, defaultValue = None, isMandatory = True])
			
Adds a :class:`tlp.PropertyInterface` parameter to the plug-in.

:param name: the name of the :class:`tlp.PropertyInterface` parameter to add
:type name: string
:param help: a description of the parameter, that will be displayed to the user
:type help: string
:param defaultValue: the default value (in the form "<property_name>") the parameter should take, to be the initial value in the GUI
:type defaultValue: string
:param isMandatory: specify whether this parameter requires a value or not
:type isMandatory: boolean 
%End			
			
%MethodCode
    sipCpp->addParameter<tlp::PropertyInterface*>((*a0).c_str(), (*a1).c_str(), (*a2).c_str(), a3);
%End

};
};
