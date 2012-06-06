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
    "       if fullname.startswith(\"tulip\"):\n"
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
    }
    tlp::loadPlugins();

%End


%ModuleHeaderCode
#include <tulip/Graph.h>
#include <tulip/TemplateFactory.h>
#include <tulip/Algorithm.h>
#include <string>
#include <sstream>

template <typename T>
std::vector<std::string> getPluginsList() {
    std::vector<std::string> ret;
    std::map< std::string, tlp::TemplateFactoryInterface* >::iterator it = tlp::TemplateFactoryInterface::allFactories->begin();
    for (; it != tlp::TemplateFactoryInterface::allFactories->end() ; ++it) {
        if (it->first == tlp::demangleTlpClassName(typeid(T).name())) {
            tlp::Iterator<std::string> *itS = it->second->availablePlugins();
            while (itS->hasNext()) {
                ret.push_back(itS->next());
            }
            delete itS;
        }
    }
    return ret;
}

extern bool pluginExists(std::string pluginName, std::string type);

extern int throwInvalidNodeException(const tlp::Graph *graph, tlp::node n);

extern int throwInvalidEdgeException(const tlp::Graph *graph, tlp::edge e);

extern void printErrorMessage(const std::string &errMsg);

extern std::vector<std::string> getAlgorithmPluginsListOfType(const std::string& type);

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

std::vector<std::string> getAlgorithmPluginsListOfType(const std::string& type) {
    std::vector<std::string> ret;
    tlp::Iterator<tlp::AlgorithmPlugin *> *itP = tlp::AlgorithmPlugin::factory->availablePluginObjects();
    while (itP->hasNext()) {
          tlp::AlgorithmPlugin *plugin = itP->next();
          if (type == plugin->getClassName())
             ret.push_back(plugin->getName());
    }
    delete itP;
    return ret;
}

bool pluginExists(std::string pluginName, std::string type) {
    if (type == "Import" || type == "Export") {
        std::string importFactoType = tlp::TemplateFactoryInterface::standardizeName(typeid(tlp::ImportModule).name());
        std::string exportFactoType = tlp::TemplateFactoryInterface::standardizeName(typeid(tlp::ExportModule).name());
        std::map< std::string, tlp::TemplateFactoryInterface* >::iterator it = tlp::TemplateFactoryInterface::allFactories->begin();
        for (; it != tlp::TemplateFactoryInterface::allFactories->end() ; ++it) {
            if ((it->first == importFactoType && type == "Import") || (it->first == exportFactoType && type == "Export"))
                return it->second->pluginExists(pluginName);
        }
        return false;
    } else {
        std::vector<std::string> algoList = getAlgorithmPluginsListOfType(type);
        return std::find(algoList.begin(), algoList.end(), pluginName) != algoList.end();
    }
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

%Include Algorithm.sip
%Include LayoutAlgorithm.sip
%Include IntegerAlgorithm.sip
%Include DoubleAlgorithm.sip
%Include BooleanAlgorithm.sip
%Include SizeAlgorithm.sip
%Include ColorAlgorithm.sip

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

%Include Plugin.sip
%Include WithParameter.sip
%Include WithDependency.sip
%Include PluginProgress.sip
%Include ObservableGraph.sip
%Include Graph.sip
%Include GraphMeasure.sip
%Include PluginLoader.sip

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



