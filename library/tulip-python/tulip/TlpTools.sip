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
#include <tulip/TemplateFactory.h>
%End

namespace tlp {

std::string TulipLibDir;
std::string TulipPluginsPath;
std::string TulipDocProfile;
std::string TulipUserHandBookIndex;
std::string TulipBitmapDir;

//=====================================================================

void initTulipLib(char* appDirPath=0);
%Docstring
tlp.initTulipLib()

Looks for the Tulip plugins directory and fills the Tulip path variables : tlp.TulipLibDir, tlp.TulipPluginsPath, tlp.TulipBitmapDir.
The plug-ins directory can be defined in different ways, given by order of prevalence:

   * the TLP_DIR environment variable, if it has a value
   * a fallback value of 'C:/Tulip/lib/' on windows, or '/usr/local/lib/' on unix
   
%End

//=====================================================================

void loadPlugins(tlp::PluginLoader *plug=0);
%Docstring
tlp.loadPlugins()

Loads C++ plugins installed in the directories listed in the tlp.TulipPluginsPath variable
(in the form "<path1>;<path2>" on windows and "<path1>:<path2>" on unix).
The tlp.TulipPluginsPath can be initialized with standard Tulip directories 
by calling the :func:`tlp.initTulipLib` function.
%End

//=====================================================================

bool loadPlugin(const std::string &filename, tlp::PluginLoader *plug=0);
%Docstring
tlp.loadPlugin(filename)

Loads a C++ plugin from a specific shared library.
Returns True if the plugin was correctly loaded.

:param filename: the path to the shared library containing the Tulip plugin (file extension : .so on linux, .dylib on mac, .dll on windows)
:type filename: string
:rtype: boolean
%End

//=====================================================================

void loadPluginsFromDir(std::string dir, std::string type = "Algorithm", tlp::PluginLoader *loader=0);
%Docstring
tlp.loadPluginsFromDir(dir)

Loads C++ plugins located in a specific directory.

:param dir: the path to the directory containing the Tulip plugins
:type dir: string
%End

//=====================================================================

void loadPluginsCheckDependencies(tlp::PluginLoader* loader=0);
%Docstring
tlp.loadPluginsCheckDependencies()

Checks the dependencies of all C++ plugins loaded so far. 
If a plugin does not have all its dependencies fulfilled,
it is unloaded and removed from the current plugins database.
%End

//=====================================================================

tlp::DataSet getDefaultPluginParameters(const std::string &pluginName, tlp::Graph *graph = 0);
%Docstring
tlp.getDefaultPluginParameters(pluginName[, graph = None])

Returns a dataset filled with default parameters (if any) for a given plugin (algorithm, import, property algorithm, ...).

:param pluginName: the name of the plugin on which to retrieve its default parameters
:type pluginName: string
:param graph: if provided, property parameters will be retrieved from the graph if they exists
:type graph: :class:`tlp.Graph`
:rtype: :class:`tlp.DataSet`
%End

%MethodCode
	tlp::TemplateFactoryInterface *selectedFactory = NULL;
	tlp::DataSet result;
	std::map< std::string, tlp::TemplateFactoryInterface* >::iterator it = tlp::TemplateFactoryInterface::allFactories->begin();
	for (; it != tlp::TemplateFactoryInterface::allFactories->end() ; ++it) {
		if (it->second->pluginExists(*a0)) {
			selectedFactory = it->second;
			break;
		}
	} 
	if (selectedFactory) {
		tlp::StructDef parameters = selectedFactory->getPluginParameters(*a0);
		parameters.buildDefaultDataSet(result, a1);
		sipRes = new tlp::DataSet(result);
	} else {
		sipIsErr = 1;
		std::string msg = "No plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

//=====================================================================

std::vector<std::string> getAlgorithmPluginsList();
%Docstring
tlp.getAlgorithmPluginsList()

Return a list containing the names of the algorithm plugins (written in C++ or Python) loaded so far.
Algorithm plugins are objects implementing the tlp::Algorithm interface 
in C++ or the :class:`tlp.Algorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::Algorithm>());
%End

//=====================================================================

std::vector<std::string> getImportPluginsList();
%Docstring
tlp.getImportPluginsList()

Return a list containing the names of the import plugins (written in C++ or Python) loaded so far.
Import plugins are objects implementing the tlp::ImportModule interface 
in C++ or the :class:`tlp.PythonImportModule` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::ImportModule>());
%End

//=====================================================================

std::vector<std::string> getExportPluginsList();
%Docstring
tlp.getExportPluginsList()

Return a list containing the names of the export plugins (written in C++ or Python) loaded so far.
Export plugins are objects implementing the tlp::ExportModule interface 
in C++ or the :class:`tlp.PythonExportModule` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::ExportModule>());
%End

//=====================================================================

std::vector<std::string> getBooleanAlgorithmPluginsList();
%Docstring
tlp.getBooleanAlgorithmPluginsList()

Return a list containing the names of the boolean algorithm plugins (written in C++ or Python) loaded so far.
Boolean algorithm plugins are objects implementing the tlp::BooleanAlgorithm interface in C++ or 
the :class:`tlp.BooleanAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::BooleanAlgorithm>());
%End

//=====================================================================

std::vector<std::string> getIntegerAlgorithmPluginsList();
%Docstring
tlp.getIntegerAlgorithmPluginsList()

Return a list containing the names of the integer algorithm plugins (written in C++ or Python) loaded so far.
Integer algorithm plugins are objects implementing the tlp::IntegerAlgorithm interface in C++ or 
the :class:`tlp.IntegerAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::IntegerAlgorithm>());
%End

//=====================================================================

std::vector<std::string> getDoubleAlgorithmPluginsList();
%Docstring
tlp.getDoubleAlgorithmPluginsList()

Return a list containing the names of the double algorithm plugins (written in C++ or Python) loaded so far.
Double algorithm plugins are objects implementing the tlp::DoubleAlgorithm interface in C++ or 
the :class:`tlp.DoubleAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::DoubleAlgorithm>());
%End

//=====================================================================

std::vector<std::string> getColorAlgorithmPluginsList();
%Docstring
tlp.getColorAlgorithmPluginsList()

Return a list containing the names of the color algorithm plugins (written in C++ or Python) loaded so far.
Color algorithm plugins are objects implementing the tlp::ColorAlgorithm interface in C++ or 
the :class:`tlp.ColorAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::ColorAlgorithm>());
%End

//=====================================================================

std::vector<std::string> getLayoutAlgorithmPluginsList();
%Docstring
tlp.getLayoutAlgorithmPluginsList()

Return a list containing the names of the layout algorithm plugins (written in C++ or Python) loaded so far.
Layout algorithm plugins are objects implementing the tlp::LayoutAlgorithm interface in C++ or 
the :class:`tlp.LayoutAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::LayoutAlgorithm>());
%End

//=====================================================================

std::vector<std::string> getSizeAlgorithmPluginsList();
%Docstring
tlp.getSizeAlgorithmPluginsList()

Return a list containing the names of the size algorithm plugins (written in C++ or Python) loaded so far.
Size algorithm plugins are objects implementing the tlp::SizeAlgorithm interface in C++ or 
the :class:`tlp.SizeAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::SizeAlgorithm>());
%End

//=====================================================================

std::vector<std::string> getStringAlgorithmPluginsList();
%Docstring
tlp.getStringAlgorithmPluginsList()

Return a list containing the names of the string algorithm plugins (written in C++ or Python) loaded so far.
String algorithm plugins are objects implementing the tlp::StringAlgorithm interface in C++ or 
the :class:`tlp.StringAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
	sipRes = new std::vector<std::string>(getPluginsList<tlp::StringAlgorithm>());
%End

};