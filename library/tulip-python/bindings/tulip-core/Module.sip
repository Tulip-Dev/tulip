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

%Module tulip

// Hack to avoid overriding the tlp Python class (containing all the module classes as
// nested classes and declared in each module : tulip, tulipogl, tulipqt) in the current
// global namespace when importing the module the following way : from tulipogl import *

// Plus set up type aliases for tlp.Coord and tlp.Size
%PreInitialisationCode
    // Try to load plugins when importing the module from the Python shell
    if (tlp::TulipPluginsPath.empty()) {
        tlp::initTulipLib();
        tlp::PluginLibraryLoader::loadPlugins();
    }
%End

%PostInitialisationCode
    // Set up some type aliases
    PyObject *moduleDict = PyModule_GetDict(sipModule);
    PyObject *tlpClass = PyDict_GetItemString(moduleDict, "tlp");
    PyObject *tlpVec3f = PyObject_GetAttrString(tlpClass, "Vec3f");
    PyObject_SetAttrString(tlpClass, "Coord", tlpVec3f);
    PyObject_SetAttrString(tlpClass, "Size", tlpVec3f);

    std::string pluginLoaderFuncs =
    "import os\n"
    "import sys\n"
    "import traceback\n"

    "def loadTulipPythonPlugin(pluginFilePath):\n"
    "        if not os.path.isfile(pluginFilePath) or not pluginFilePath.endswith(\".py\"):\n"
    "                return False\n"

    "        try:\n"
    "                pluginFile = open(pluginFilePath)\n"
    "                pluginFileContent = pluginFile.read()\n"
    "        except:\n"
    "                return False\n"

    "        if not \"tulipplugins.register\" in pluginFileContent:\n"
    "                return False\n"

    "        modulePath = os.path.dirname(pluginFilePath)\n"
    "        moduleName = os.path.basename(pluginFilePath)[:-3]\n"

    "        if not modulePath in sys.path:\n"
    "                sys.path.append(modulePath)\n"

    "        try:\n"
    "                mod = __import__(moduleName)\n"
    "        except ImportError:\n"
    "                sys.stdout.write(\"There was an error when trying to load the Tulip Python plugin from the file \" + pluginFilePath + \". See stack trace below.\\n\")\n"
    "                traceback.print_exc()\n"
    "                return False\n"

    "        return True\n"

    "def loadTulipPythonPluginsFromDir(pluginsDirPath):\n"
    "        if not os.path.exists(pluginsDirPath):\n"
    "                return False\n"

    "        files = os.listdir(pluginsDirPath)\n"

    "        for file in files:\n"
    "                loadTulipPythonPlugin(pluginsDirPath+\"/\"+file)\n"

    "        return True\n"
    ;
    PyRun_SimpleString(pluginLoaderFuncs.c_str());
%End

%ModuleHeaderCode
#include <tulip/Graph.h>
#include <tulip/PluginLister.h>
#include <tulip/PropertyAlgorithm.h>
#include <string>
#include <sstream>
#include <iostream>


template <typename T>
bool pluginExists(std::string pluginName) {
        if (typeid(T).name() == typeid(tlp::Algorithm).name()) {
            std::list<std::string> algoList = tlp::PluginLister::instance()->availablePlugins<tlp::Algorithm>();
            std::list<std::string> propAlgoList = tlp::PluginLister::instance()->availablePlugins<tlp::PropertyAlgorithm>();
            std::list<std::string> realAlgoList;
            for (std::list<std::string>::iterator it = algoList.begin() ; it != algoList.end() ; ++it) {
                if (std::find(propAlgoList.begin(), propAlgoList.end(), *it) == propAlgoList.end()) {
                    realAlgoList.push_back(*it);
                }
            }
            return std::find(realAlgoList.begin(), realAlgoList.end(), pluginName) != realAlgoList.end();
        } else {
            return tlp::PluginLister::instance()->pluginExists<T>(pluginName);
        }
}

template <typename PROP>
bool canGetProperty(const tlp::Graph *graph, const std::string &propName) {
    if (!graph->existProperty(propName)) {
        return true;
    }
    return dynamic_cast<PROP*>(graph->getProperty(propName)) != NULL;
}

extern int throwInvalidNodeException(const tlp::Graph *graph, tlp::node n);

extern int throwInvalidEdgeException(const tlp::Graph *graph, tlp::edge e);

extern int throwInvalidSgException(const tlp::Graph *graph, const tlp::Graph *sg);

extern int throwPropertyNameExistsException(const tlp::Graph *graph, const std::string &propertyName);

extern void printErrorMessage(const std::string &errMsg);

extern void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td);

extern void replaceAll(std::string &s, const std::string& replaceWhat, const std::string& replaceWithWhat);

extern bool isValidGraphSelection(const tlp::Graph *graph, tlp::BooleanProperty *selection);

extern PyObject* evalPythonStatement(const std::string &pythonStatement);

#if PY_MAJOR_VERSION >= 3
extern std::string convertPythonUnicodeObjectToStdString(PyObject *pyUnicodeObj);
#endif
%End

%ModuleCode

int throwInvalidNodeException(const tlp::Graph *graph, tlp::node n) {
    std::ostringstream oss;
    oss << "Node with id " << n.id << " does not belong to graph \"" << graph->getName() <<"\" (id " << graph->getId() <<")";
    PyErr_SetString(PyExc_Exception, oss.str().c_str());
    return -1;
}

int throwInvalidEdgeException(const tlp::Graph *graph, tlp::edge e) {
    std::ostringstream oss;
    oss << "Edge with id " << e.id << " does not belong to graph \"" << graph->getName() <<"\" (id " << graph->getId() <<")";
    PyErr_SetString(PyExc_Exception, oss.str().c_str());
    return -1;
}

int throwInvalidSgException(const tlp::Graph *graph, const tlp::Graph *sg) {
    std::ostringstream oss;
    oss << "Graph \"" << sg->getName() <<"\" (id " << sg->getId() <<") is not a sub-graph of graph \"" << graph->getName() <<"\" (id " << graph->getId() <<")";
    PyErr_SetString(PyExc_Exception, oss.str().c_str());
    return -1;
}

int throwPropertyNameExistsException(const tlp::Graph *graph, const std::string &propertyName) {
    tlp::PropertyInterface *prop = graph->getProperty(propertyName);
    std::ostringstream oss;
    oss << "A property named \"" << propertyName << "\" of type " << prop->getTypename() << " already exists in graph \"" << graph->getName() <<"\" (id " << graph->getId() <<")";
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

void replaceAll(std::string &s, const std::string& replaceWhat, const std::string& replaceWithWhat) {
  while(1) {
    const int pos = s.find(replaceWhat);
    if (pos==std::string::npos) break;
    s.replace(pos,replaceWhat.size(),replaceWithWhat);
  }
}

bool isValidGraphSelection(const tlp::Graph *graph, tlp::BooleanProperty *selection) {
    tlp::edge e;
    forEach(e, graph->getEdges()) {
        if (selection->getEdgeValue(e)) {
            if (!selection->getNodeValue(graph->source(e)) || !selection->getNodeValue(graph->target(e))) {
                return false;
            }
        }
    }
    return true;
}

#if PY_MAJOR_VERSION >= 3
std::string convertPythonUnicodeObjectToStdString(PyObject *pyUnicodeObj) {
    PyObject *utf8Str = PyUnicode_AsUTF8String(pyUnicodeObj);
    std::string ret(PyBytes_AsString(utf8Str));
    Py_DECREF(utf8Str);
    return ret;
}
#endif

PyObject* evalPythonStatement(const std::string &pythonStatement) {
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString("__main__");
#else
  PyObject *pName = PyString_FromString("__main__");
#endif
  PyObject *pMainModule = PyImport_Import(pName);
  Py_XDECREF(pName);
  PyObject *pMainDict = PyModule_GetDict(pMainModule);

  PyObject *ret = PyRun_String(pythonStatement.c_str(), Py_eval_input, pMainDict, pMainDict);

  if (PyErr_Occurred()) {
    PyErr_Print();
    PyErr_Clear();
  }

  return ret;
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

%Include PropertyInterface.sip


%Include BooleanProperty.sip
%Include DoubleProperty.sip
%Include ColorProperty.sip
%Include StringProperty.sip
%Include SizeProperty.sip
%Include IntegerProperty.sip
%Include LayoutProperty.sip
%Include NumericProperty.sip

%Include GraphProperty.sip

%Include WithParameter.sip
%Include WithDependency.sip
%Include PluginProgress.sip
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
%Include PropertyEvent.sip
%Include TulipViewSettings.sip
%Include Delaunay.sip
%Include ConvexHull.sip

// force generation of wrappers for specialized template classes
typedef std::vector<tlp::Graph*> vectorGraphType;
typedef std::list<tlp::Graph*> listGraphType;
typedef std::set<tlp::Graph*> setGraphType;

typedef std::vector<tlp::node> vectorNodeType;
typedef std::list<tlp::node> listNodeType;
typedef std::set<tlp::node> setNodeType;

typedef std::vector<tlp::edge> vectorEdgeType;
typedef std::list<tlp::edge> listEdgeType;
typedef std::set<tlp::edge> setEdgeType;

typedef std::vector<tlp::Color> vectorColorType;
typedef std::list<tlp::Color> listColorType;
typedef std::set<tlp::Color> setColorType;

typedef std::vector<tlp::Coord> vectorCoordType;
typedef std::list<tlp::Coord> listCoordType;
typedef std::set<tlp::Coord> setCoordType;

typedef std::vector<tlp::Size> vectorSizeType;
typedef std::list<tlp::Size> listSizeType;
typedef std::set<tlp::Size> setSizeType;

typedef std::vector<tlp::DataSet> vectorDataSetType;
typedef std::list<tlp::DataSet> listDataSetType;

typedef std::vector<tlp::ColorScale> vectorColorScaleType;
typedef std::list<tlp::ColorScale> listColorScaleType;

typedef std::vector<tlp::StringCollection> vectorStringCollectionType;
typedef std::list<tlp::StringCollection> listStringCollectionType;

typedef std::vector<tlp::BooleanProperty*> vectorBooleanPropertyType;
typedef std::list<tlp::BooleanProperty*> listBooleanPropertyType;

typedef std::vector<tlp::ColorProperty*> vectorColorPropertyType;
typedef std::list<tlp::ColorProperty*> listColorPropertyType;

typedef std::vector<tlp::DoubleProperty*> vectorDoublePropertyType;
typedef std::list<tlp::DoubleProperty*> listDoublePropertyType;

typedef std::vector<tlp::IntegerProperty*> vectorIntegerPropertyType;
typedef std::list<tlp::IntegerProperty*> listIntegerPropertyType;

typedef std::vector<tlp::LayoutProperty*> vectorLayoutPropertyType;
typedef std::list<tlp::LayoutProperty*> listLayoutPropertyType;

typedef std::vector<tlp::SizeProperty*> vectorSizePropertyType;
typedef std::list<tlp::SizeProperty*> listSizePropertyType;

typedef std::vector<tlp::StringProperty*> vectorStringPropertyType;
typedef std::list<tlp::StringProperty*> listStringPropertyType;

typedef std::vector<tlp::PropertyInterface*> vectorPropertyInterfaceType;
typedef std::list<tlp::PropertyInterface*> listPropertyInterfaceType;
