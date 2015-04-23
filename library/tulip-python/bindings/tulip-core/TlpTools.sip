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
#include <tulip/PluginLibraryLoader.h>
#include <tulip/PluginLister.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/TlpTools.h>
%End

namespace tlp {

std::string TulipLibDir;
std::string TulipPluginsPath;
std::string TulipBitmapDir;
std::string TulipShareDir;

//=====================================================================

std::string getTulipRelease();
%Docstring
tlp.getTulipRelease()

Returns the current version number of Tulip in the form X.Y.Z,
X being the major version, Y the minor version, and Z the patch version.

:rtype: string
%End

%MethodCode
	sipRes = new std::string(TULIP_RELEASE);
%End

//=====================================================================

    void initTulipLib(const std::string &appDirPath = "");
%Docstring
tlp.initTulipLib()

Looks for the Tulip plugins directory and fills the Tulip path variables : tlp.TulipLibDir, tlp.TulipPluginsPath, tlp.TulipBitmapDir.
The plug-ins directory can be defined in different ways, given by order of prevalence:

    * the TLP_DIR environment variable, if it has a value
    * the appDirPath parameter, if it is not NULL
    * at that point, the Tulip paths will be retrieved from the path of the loaded Tulip shared library
      (you must dispose of a standard Tulip installation for that feature to work,
      meaning the relative path for the Tulip plugins folder to the Tulip libraries one must be ../lib/tulip/).
    * a fallback value of 'C:/Tulip/lib/' on windows, or '/usr/local/lib/' on Unix.
   
%End

%MethodCode
    const char *appDirPathStr = NULL;
    if ((*a0) != "") {
        appDirPathStr = (*a0).c_str();
    }
    tlp::initTulipLib(appDirPathStr);
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

%MethodCode
    if (a0) {
        tlp::PluginLibraryLoader::loadPlugins(a0);
    } else {
        tlp::PluginLoaderTxt pluginLoader;
        tlp::PluginLibraryLoader::loadPlugins(&pluginLoader);
    }
%End

//=====================================================================

void loadPluginsFromDir(const std::string &pluginsDir, tlp::PluginLoader *plug=0);
%Docstring
tlp.loadPluginsFromDir(pluginsDir)

.. versionadded:: 4.2

Loads C++ and Python plugins installed in a specific directory.
%End

%MethodCode
    std::string oldTlpPluginsPath = tlp::TulipPluginsPath;
    tlp::TulipPluginsPath = *a0;
    if (a1) {
        tlp::PluginLibraryLoader::loadPlugins(a1);
    } else {
        tlp::PluginLoaderTxt pluginLoader;
        tlp::PluginLibraryLoader::loadPlugins(&pluginLoader);
    }
    tlp::TulipPluginsPath = oldTlpPluginsPath;
    std::string importPythonPlugins = "loadTulipPythonPluginsFromDir(\"";
    importPythonPlugins += (*a0 + "\")");
    PyObject *pyRet = evalPythonStatement(importPythonPlugins);
    Py_XDECREF(pyRet);
%End

//=====================================================================

bool loadPlugin(const std::string &filename, tlp::PluginLoader *plug=0);
%Docstring
tlp.loadPlugin(filename)

Loads a C++ plugin or a Python plugin from a specific file (shared library or Python source file).
Returns :const:`True` if the plugin was correctly loaded.

:param filename: the path to the shared library containing the Tulip plugin (file extension : .so on linux, .dylib on mac, .dll on windows)
:type filename: string
:rtype: boolean
%End

%MethodCode
    std::string ext = a0->substr(a0->length()-3);
    if (ext != ".py") {
        if (a1) {
            sipRes = tlp::PluginLibraryLoader::loadPluginLibrary(*a0, a1);
        } else {
            tlp::PluginLoaderTxt pluginLoader;
            sipRes = tlp::PluginLibraryLoader::loadPluginLibrary(*a0, &pluginLoader);
        }
    } else {
        std::string importPythonPlugin = "loadTulipPythonPlugin(\"";
        importPythonPlugin += (*a0 + "\")");
        PyObject *pyRet = evalPythonStatement(importPythonPlugin);
        sipRes = (pyRet == Py_True);
        Py_XDECREF(pyRet);
    }
%End

//=====================================================================

void loadPluginsCheckDependencies(tlp::PluginLoader* loader=0);
%Docstring
tlp.loadPluginsCheckDependencies()

Checks the dependencies of all C++ plugins loaded so far. 
If a plugin does not have all its dependencies fulfilled,
it is unloaded and removed from the current plugins database.
%End

%MethodCode
    if (a0) {
        tlp::PluginLister::checkLoadedPluginsDependencies(a0);
    } else {
        tlp::PluginLoaderTxt pluginLoader;
        tlp::PluginLister::checkLoadedPluginsDependencies(&pluginLoader);
    }
%End

//=====================================================================

tlp::DataSet getDefaultPluginParameters(const std::string &pluginName, tlp::Graph *graph = 0);
%Docstring
tlp.getDefaultPluginParameters(pluginName[, graph = None])

Returns a dataset filled with default parameters (if any) for a given plugin (algorithm, import, property algorithm, ...).

:param pluginName: the name of the plugin on which to retrieve its default parameters
:type pluginName: string
:param graph: if provided, property parameters will be retrieved from the graph if they exist
:type graph: :class:`tlp.Graph`
:rtype: :class:`tlp.DataSet`
%End

%MethodCode
    if (tlp::PluginLister::pluginExists(*a0)) {
        tlp::DataSet result;
        const tlp::ParameterDescriptionList &parameters = tlp::PluginLister::getPluginParameters(*a0);
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

std::list<std::string> getAlgorithmPluginsList();
%Docstring
tlp.getAlgorithmPluginsList()

Return a list containing the names of the algorithm plugins (written in C++ or Python) loaded so far.
Algorithm plugins are objects implementing the tlp::Algorithm interface 
in C++ or the :class:`tlp.Algorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
        std::list<std::string> algoList = tlp::PluginLister::instance()->availablePlugins<tlp::Algorithm>();
        std::list<std::string> propAlgoList = tlp::PluginLister::instance()->availablePlugins<tlp::PropertyAlgorithm>();
        std::list<std::string> ret;
        for (std::list<std::string>::iterator it = algoList.begin() ; it != algoList.end() ; ++it) {
            if (std::find(propAlgoList.begin(), propAlgoList.end(), *it) == propAlgoList.end()) {
                ret.push_back(*it);
            }
        }
        sipRes = new std::list<std::string>(ret);
%End

//=====================================================================

std::list<std::string> getImportPluginsList();
%Docstring
tlp.getImportPluginsList()

Return a list containing the names of the import plugins (written in C++ or Python) loaded so far.
Import plugins are objects implementing the tlp::ImportModule interface 
in C++ or the :class:`tlp.ImportModule` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::ImportModule>());
%End

//=====================================================================

std::list<std::string> getExportPluginsList();
%Docstring
tlp.getExportPluginsList()

Return a list containing the names of the export plugins (written in C++ or Python) loaded so far.
Export plugins are objects implementing the tlp::ExportModule interface 
in C++ or the :class:`tlp.ExportModule` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::ExportModule>());
%End

//=====================================================================

std::list<std::string> getBooleanAlgorithmPluginsList();
%Docstring
tlp.getBooleanAlgorithmPluginsList()

Return a list containing the names of the boolean algorithm plugins (written in C++ or Python) loaded so far.
Boolean algorithm plugins are objects implementing the tlp::BooleanAlgorithm interface in C++ or 
the :class:`tlp.BooleanAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::BooleanAlgorithm>());
%End

//=====================================================================

std::list<std::string> getIntegerAlgorithmPluginsList();
%Docstring
tlp.getIntegerAlgorithmPluginsList()

Return a list containing the names of the integer algorithm plugins (written in C++ or Python) loaded so far.
Integer algorithm plugins are objects implementing the tlp::IntegerAlgorithm interface in C++ or 
the :class:`tlp.IntegerAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::IntegerAlgorithm>());
%End

//=====================================================================

std::list<std::string> getDoubleAlgorithmPluginsList();
%Docstring
tlp.getDoubleAlgorithmPluginsList()

Return a list containing the names of the double algorithm plugins (written in C++ or Python) loaded so far.
Double algorithm plugins are objects implementing the tlp::DoubleAlgorithm interface in C++ or 
the :class:`tlp.DoubleAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::DoubleAlgorithm>());
%End

//=====================================================================

std::list<std::string> getColorAlgorithmPluginsList();
%Docstring
tlp.getColorAlgorithmPluginsList()

Return a list containing the names of the color algorithm plugins (written in C++ or Python) loaded so far.
Color algorithm plugins are objects implementing the tlp::ColorAlgorithm interface in C++ or 
the :class:`tlp.ColorAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::ColorAlgorithm>());
%End

//=====================================================================

std::list<std::string> getLayoutAlgorithmPluginsList();
%Docstring
tlp.getLayoutAlgorithmPluginsList()

Return a list containing the names of the layout algorithm plugins (written in C++ or Python) loaded so far.
Layout algorithm plugins are objects implementing the tlp::LayoutAlgorithm interface in C++ or 
the :class:`tlp.LayoutAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::LayoutAlgorithm>());
%End

//=====================================================================

std::list<std::string> getSizeAlgorithmPluginsList();
%Docstring
tlp.getSizeAlgorithmPluginsList()

Return a list containing the names of the size algorithm plugins (written in C++ or Python) loaded so far.
Size algorithm plugins are objects implementing the tlp::SizeAlgorithm interface in C++ or 
the :class:`tlp.SizeAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::SizeAlgorithm>());
%End

//=====================================================================

std::list<std::string> getStringAlgorithmPluginsList();
%Docstring
tlp.getStringAlgorithmPluginsList()

Return a list containing the names of the string algorithm plugins (written in C++ or Python) loaded so far.
String algorithm plugins are objects implementing the tlp::StringAlgorithm interface in C++ or 
the :class:`tlp.StringAlgorithm` interface in Python.

:rtype: list of string
%End

%MethodCode
        sipRes = new std::list<std::string>(tlp::PluginLister::instance()->availablePlugins<tlp::StringAlgorithm>());
%End

//=====================================================================

unsigned int getSeedOfRandomSequence();
%Docstring
tlp.getSeedOfRandomSequence()

Return the value of the seed used for further initialization of a random sequence

:rtype: integer
%End

//=====================================================================

void setSeedOfRandomSequence(unsigned int seed);
%Docstring
tlp.setSeedOfRandomSequence(seed)

Give the value of the seed used for further initialization of a random sequence (with further calls to rand() or rand_r(...)).
Set seed to UINT_MAX if you need a random choice of the seed.

:param seed: the value of the seed
:type seed: integer
%End

//=====================================================================

void initRandomSequence();
%Docstring
tlp.initRandomSequence()

Initialize a random sequence with the seed previously set.
Further calls to rand() or rand_r(...) will return the elements of that sequence

%End

};
