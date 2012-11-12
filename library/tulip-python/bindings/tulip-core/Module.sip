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
// | inspired from bindings by the Booggie project developpment team         |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

%Module tulip

// Hack to avoid overriding the tlp Python class (containing all the module classes as
// nested classes and declared in each module : tulip, tulipogl, tulipqt) in the current
// global namespace when importing the module the following way : from tulipogl import *
%PreInitialisationCode
    std::string tlpImportHook =
    "try:\n"
    "   _origimportfunc\n"
    "except NameError:\n"
    "   import sys\n"
#if PY_MAJOR_VERSION >= 3
    "   import builtins\n"
    "   _origimportfunc = builtins.__import__\n"
#else
    "   import __builtin__\n"
    "   _origimportfunc = __builtin__.__import__\n"
#endif
    "   def tlpimporthook(name, globals={}, locals={}, fromlist=[], level=-1):\n"
    "       fullname = name\n"
    "       m = _origimportfunc(name, globals, locals, fromlist, level)\n"
    "       if fullname == \"tulip\" or fullname == \"tulipogl\" or fullname == \"tulipqt\":\n"
    "           if fromlist and len(fromlist) > 0 and fromlist[0] == \"*\":\n"
    "               curTlpDict = None\n"
    "               if \"tlp\" in locals:\n"
    "                   curTlpDict = locals[\"tlp\"].__dict__\n"
    "                   if curTlpDict:\n"
    "                       m = type(fullname, (object,), dict(m.__dict__))\n"
    "                       m.tlp = type(\"tlp\", (object,), dict(m.tlp.__dict__))\n"
    "                       for k in curTlpDict.keys():\n"
    "                           if not k.startswith(\"_\"):\n"
    "                               setattr(m.tlp, k, curTlpDict[k])\n"
    "       return m\n"
#if PY_MAJOR_VERSION >= 3
    "   builtins.__import__ = tlpimporthook\n"
#else
    "   __builtin__.__import__ = tlpimporthook\n"
#endif
     ;

    PyRun_SimpleString(tlpImportHook.c_str());

    // Try to load plugins when importing the module from the Python shell
    if (tlp::TulipPluginsPath.empty()) {
        tlp::initTulipLib();
        tlp::PluginLibraryLoader::loadPlugins();
    }
%End


%ModuleHeaderCode
#include <tulip/Graph.h>
#include <tulip/PluginLister.h>
#include <tulip/PropertyAlgorithm.h>
#include <string>
#include <sstream>

template <typename T>
bool pluginExists(std::string pluginName) {
      return tlp::PluginLister::instance()->pluginExists<T>(pluginName);
}

extern int throwInvalidNodeException(const tlp::Graph *graph, tlp::node n);

extern int throwInvalidEdgeException(const tlp::Graph *graph, tlp::edge e);

extern void printErrorMessage(const std::string &errMsg);

extern void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td);

%End

%ModuleCode

int throwInvalidNodeException(const tlp::Graph *graph, tlp::node n) {
    std::ostringstream oss;
    std::string graphName;
    graph->getAttribute("name", graphName);
    oss << "Node with id " << n.id << " does not belong to graph \"" << graphName <<"\" (id " << graph->getId() <<")";
    PyErr_SetString(PyExc_Exception, oss.str().c_str());
    return -1;
}

int throwInvalidEdgeException(const tlp::Graph *graph, tlp::edge e) {
    std::ostringstream oss;
    std::string graphName;
    graph->getAttribute("name", graphName);
    oss << "Edge with id " << e.id << " does not belong to graph \"" << graphName <<"\" (id " << graph->getId() <<")";
    PyErr_SetString(PyExc_Exception, oss.str().c_str());
    return -1;
}


void printErrorMessage(const std::string &errMsg) {
    std::string pythonCode = "import sys\n"
                             "sys.stderr.write(\"";
    pythonCode += errMsg;
    pythonCode += "\\n\")";
    PyRun_SimpleString(pythonCode.c_str());
}

void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td) {
    PyObject *pyObj = sipGetPyObject(wrappedCppObject, td);
    if (pyObj) {
        sipTransferTo(pyObj, pyObj);
        sipSimpleWrapper *wrapper = reinterpret_cast<sipSimpleWrapper *>(pyObj);
        sipCommonDtor(wrapper);
    }
}

%End

%Import ../stl/Module.sip

%Include Iterator.sip
%Include MapIterator.sip
%Include Node.sip
%Include Edge.sip	
%Include Vector.sip
%Include Color.sip
%Include Coord.sip
%Include Size.sip
%Include Matrix.sip
%Include BoundingBox.sip
%Include StringCollection.sip
%Include ColorScale.sip
%Include Reflect.sip

%Include PluginContext.sip
%Include Plugin.sip

%Include Algorithm.sip
%Include PropertyAlgorithm.sip

%Include ImportModule.sip
%Include ExportModule.sip

%Include ObservableProperty.sip
%Include PropertyInterface.sip

%Include BooleanProperty.sip
%Include DoubleProperty.sip
%Include ColorProperty.sip
%Include StringProperty.sip
%Include SizeProperty.sip
%Include IntegerProperty.sip
%Include LayoutProperty.sip

%Include GraphProperty.sip

%Include WithParameter.sip
%Include WithDependency.sip
%Include PluginProgress.sip
%Include ObservableGraph.sip
%Include Graph.sip
%Include GraphMeasure.sip
%Include GraphTools.sip
%Include PluginLoader.sip
%Include PluginLoaderTxt.sip

%Include TlpTools.sip
%Include AcyclicTest.sip
%Include ConnectedTest.sip
%Include SimpleTest.sip
%Include TreeTest.sip
%Include OuterPlanarTest.sip
%Include BiconnectedTest.sip
%Include TriconnectedTest.sip
%Include PlanarityTest.sip

%Include DrawingTools.sip
%Include Observable.sip
%Include GraphEvent.sip
%Include Constants.sip
%Include Delaunay.sip



