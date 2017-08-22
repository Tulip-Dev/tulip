/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/StableIterator.h>
#include <tulip/PropertyInterface.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/Algorithm.h>
#include <fstream>
namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;
typedef tlp::PropertyInterface* PropertyPointer;
typedef tlp::Iterator<tlp::PropertyPointer> IteratorProperty;
}
typedef std::pair<tlp::node, tlp::node> pairNodeNode;

inline void releasePropertyWrapper(tlp::PropertyInterface *prop) {
  releaseSIPWrapper(prop, sipFindType("tlp::PropertyInterface"));
}

inline void releaseGraphWrapper(tlp::Graph *graph) {
  releaseSIPWrapper(graph, sipFindType("tlp::Graph"));
  tlp::PropertyInterface *prop = NULL;
  forEach(prop, graph->getLocalObjectProperties()) {
    releasePropertyWrapper(prop);
  }
}

inline void releaseGraphHierarchyWrappers(tlp::Graph *graph) {
  tlp::Graph *sg = NULL;
  forEach(sg, graph->getSubGraphs()) {
    releaseGraphHierarchyWrappers(sg);
  }
  releaseGraphWrapper(graph);
}

extern tlp::DataSet *prepareAlgorithmParameters(const std::string &algoName, tlp::Graph *graph=NULL, tlp::DataSet *inputParams=NULL, PyObject *inputParamsWrapper=NULL);
extern void updateWrappedDataSetAfterAlgorithmCall(tlp::DataSet *algorithmOutputDataset, PyObject *inputDataSetWrapper);

template<class AlgorithmType, class PropertyType>
bool callGraphPropertyAlgorithm(tlp::Graph *graph, const std::string &algoName, PropertyType *result,
                                tlp::DataSet *inputDataSet, PyObject *inputDataSetWrapper,
                                std::string &errMsg, int &sipIsErr, const std::string &propertyType) {
  bool ret = false;
  if (pluginExists<AlgorithmType>(algoName)) {
    tlp::DataSet *params = prepareAlgorithmParameters(algoName, graph, inputDataSet, inputDataSetWrapper);
    if (params) {
      PropertyType tmp(graph);
      tmp = *result;
      ret = graph->applyPropertyAlgorithm(algoName, &tmp, errMsg, NULL, params);
      *result = tmp;
      updateWrappedDataSetAfterAlgorithmCall(params, inputDataSetWrapper);
      delete params;
    } else {
      sipIsErr = 1;
    }
  } else {
    sipIsErr = 1;
    std::string msg = "No Tulip " + propertyType + " algorithm plugin named ";
    msg += algoName;
    msg += ".";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
  return ret;
}

tlp::DataSet *getDataSetFilledWithPropertiesValues(tlp::Graph *graph, unsigned int eltId, bool nodeValues);

bool setPropertiesValuesFromDataSet(tlp::Graph *graph, const tlp::DataSet *dataSet, unsigned int eltId, bool nodeValues);

%End

%ModuleCode

tlp::DataSet *prepareAlgorithmParameters(const std::string &algoName, tlp::Graph *graph, tlp::DataSet *inputParams, PyObject *inputParamsWrapper) {
  tlp::DataSet algoParams = ::getDefaultPluginParameters(algoName, graph);
  tlp::DataSet *params = NULL;
  if (inputParamsWrapper && PyDict_Check(inputParamsWrapper)) {
    params = convertPyDictToTlpDataSet(inputParamsWrapper, &algoParams, algoName);
  } else {
    params = new tlp::DataSet(algoParams);
    if (inputParams) {
      std::pair<std::string, tlp::DataType*> entry;
      forEach(entry, inputParams->getValues()) {
        params->setData(entry.first, entry.second);
      }
    }
  }
  // if some file parameters are defined without the special prefix provided, ensure
  // the prefixed versions are defined and set their values before calling Tulip algorithm
  if (params) {
    std::vector<std::string> filesPrefixes;
    filesPrefixes.push_back("file::");
    filesPrefixes.push_back("anyfile::");
    filesPrefixes.push_back("dir::");
    std::pair<std::string, tlp::DataType*> entry;
    stableForEach(entry, params->getValues()) {
      for (size_t i = 0 ; i < filesPrefixes.size() ; ++i) {
        std::string fileParamTlpName = filesPrefixes[i] + entry.first;
        if (algoParams.exist(fileParamTlpName)) {
          std::string fileParamTlp;
          std::string fileParam;
          params->get(fileParamTlpName, fileParamTlp);
          params->get(entry.first, fileParam);
          if (fileParamTlp.empty() && !fileParam.empty()) {
            params->set(fileParamTlpName, fileParam);
          }
          if (!fileParamTlp.empty() && fileParam.empty()) {
            params->set(entry.first, fileParamTlp);
          }
        }
      }
    }
  }
  return params;
}

void updateWrappedDataSetAfterAlgorithmCall(tlp::DataSet *algorithmOutputDataset, PyObject *inputDataSetWrapper) {
  if (inputDataSetWrapper && PyDict_Check(inputDataSetWrapper)) {
    convertTlpDataSetToPyDict(*algorithmOutputDataset, inputDataSetWrapper);
  } else if (inputDataSetWrapper) {
    tlp::DataSet *wrappedDs = static_cast<tlp::DataSet*>(sipGetAddress(reinterpret_cast<sipSimpleWrapper *>(inputDataSetWrapper)));
    std::pair<std::string, tlp::DataType*> entry;
    forEach(entry, algorithmOutputDataset->getValues()) {
      wrappedDs->setData(entry.first, entry.second);
    }
  }
}

tlp::DataSet *getDataSetFilledWithPropertiesValues(tlp::Graph *graph, unsigned int eltId, bool nodeValues) {
  tlp::DataSet *ret = new tlp::DataSet();
  tlp::PropertyInterface *prop = NULL;
  forEach(prop, graph->getObjectProperties()) {
    tlp::BooleanProperty *booleanProp = dynamic_cast<tlp::BooleanProperty*>(prop);
    tlp::ColorProperty *colorProp = dynamic_cast<tlp::ColorProperty*>(prop);
    tlp::DoubleProperty *doubleProp = dynamic_cast<tlp::DoubleProperty*>(prop);
    tlp::IntegerProperty *integerProp = dynamic_cast<tlp::IntegerProperty*>(prop);
    tlp::LayoutProperty *layoutProp = dynamic_cast<tlp::LayoutProperty*>(prop);
    tlp::SizeProperty *sizeProp = dynamic_cast<tlp::SizeProperty*>(prop);
    tlp::StringProperty *stringProp = dynamic_cast<tlp::StringProperty*>(prop);
    tlp::BooleanVectorProperty *booleanVecProp = dynamic_cast<tlp::BooleanVectorProperty*>(prop);
    tlp::ColorVectorProperty *colorVecProp = dynamic_cast<tlp::ColorVectorProperty*>(prop);
    tlp::DoubleVectorProperty *doubleVecProp = dynamic_cast<tlp::DoubleVectorProperty*>(prop);
    tlp::IntegerVectorProperty *integerVecProp = dynamic_cast<tlp::IntegerVectorProperty*>(prop);
    tlp::CoordVectorProperty *coordVecProp = dynamic_cast<tlp::CoordVectorProperty*>(prop);
    tlp::SizeVectorProperty *sizeVecProp = dynamic_cast<tlp::SizeVectorProperty*>(prop);
    tlp::StringVectorProperty *stringVecProp = dynamic_cast<tlp::StringVectorProperty*>(prop);

    if (booleanProp) {
      ret->set(prop->getName(), nodeValues ? booleanProp->getNodeValue(tlp::node(eltId)) : booleanProp->getEdgeValue(tlp::edge(eltId)));
    } else if (colorProp) {
      ret->set(prop->getName(), nodeValues ? colorProp->getNodeValue(tlp::node(eltId)) : colorProp->getEdgeValue(tlp::edge(eltId)));
    } else if (doubleProp) {
      ret->set(prop->getName(), nodeValues ? doubleProp->getNodeValue(tlp::node(eltId)) : doubleProp->getEdgeValue(tlp::edge(eltId)));
    } else if (integerProp) {
      ret->set(prop->getName(), nodeValues ? integerProp->getNodeValue(tlp::node(eltId)) : integerProp->getEdgeValue(tlp::edge(eltId)));
    } else if (layoutProp) {
      if (nodeValues) {
        ret->set(prop->getName(), layoutProp->getNodeValue(tlp::node(eltId)));
      } else {
        ret->set(prop->getName(), layoutProp->getEdgeValue(tlp::edge(eltId)));
      }
    } else if (sizeProp) {
      ret->set(prop->getName(), nodeValues ? sizeProp->getNodeValue(tlp::node(eltId)) : sizeProp->getEdgeValue(tlp::edge(eltId)));
    } else if (stringProp) {
      ret->set(prop->getName(), nodeValues ? stringProp->getNodeValue(tlp::node(eltId)) : stringProp->getEdgeValue(tlp::edge(eltId)));
    } else if (booleanVecProp) {
      ret->set(prop->getName(), nodeValues ? booleanVecProp->getNodeValue(tlp::node(eltId)) : booleanVecProp->getEdgeValue(tlp::edge(eltId)));
    } else if (colorVecProp) {
      ret->set(prop->getName(), nodeValues ? colorVecProp->getNodeValue(tlp::node(eltId)) : colorVecProp->getEdgeValue(tlp::edge(eltId)));
    } else if (doubleVecProp) {
      ret->set(prop->getName(), nodeValues ? doubleVecProp->getNodeValue(tlp::node(eltId)) : doubleVecProp->getEdgeValue(tlp::edge(eltId)));
    } else if (integerVecProp) {
      ret->set(prop->getName(), nodeValues ? integerVecProp->getNodeValue(tlp::node(eltId)) : integerVecProp->getEdgeValue(tlp::edge(eltId)));
    } else if (coordVecProp) {
      ret->set(prop->getName(), nodeValues ? coordVecProp->getNodeValue(tlp::node(eltId)) : coordVecProp->getEdgeValue(tlp::edge(eltId)));
    } else if (sizeVecProp) {
      ret->set(prop->getName(), nodeValues ? sizeVecProp->getNodeValue(tlp::node(eltId)) : sizeVecProp->getEdgeValue(tlp::edge(eltId)));
    } else if (stringVecProp) {
      ret->set(prop->getName(), nodeValues ? stringVecProp->getNodeValue(tlp::node(eltId)) : stringVecProp->getEdgeValue(tlp::edge(eltId)));
    }
  }
  return ret;
}

bool setPropertiesValuesFromDataSet(tlp::Graph *graph, const tlp::DataSet *dataSet, unsigned int eltId, bool nodeValues) {
  PyObject *graphWrapper = sipGetPyObject(graph, sipFindType("tlp::Graph"));
#if PY_MAJOR_VERSION >= 3
  PyObject *pSetItem = PyUnicode_FromString("__setitem__");
  PyObject *pGetItem = PyUnicode_FromString("__getitem__");
#else
  PyObject *pSetItem = PyString_FromString("__setitem__");
  PyObject *pGetItem = PyString_FromString("__getitem__");
#endif

  bool ok = true;

  tlp::Observable::holdObservers();
  std::pair<std::string, tlp::DataType*> entry;
  forEach(entry, dataSet->getValues()) {

#if PY_MAJOR_VERSION >= 3
    PyObject *pPropName = PyUnicode_FromString(entry.first.c_str());
#else
    PyObject *pPropName = PyString_FromString(entry.first.c_str());
#endif
    PyObject *pElement = NULL;
    if (nodeValues) {
      pElement = sipConvertFromNewType(new tlp::node(eltId), sipFindType("tlp::node"), Py_None);
    } else {
      pElement = sipConvertFromNewType(new tlp::edge(eltId), sipFindType("tlp::edge"), Py_None);
    }

    PyObject *propProxy = PyObject_CallMethodObjArgs(graphWrapper, pGetItem, pPropName, NULL);

    PyObject *ret = PyObject_CallMethodObjArgs(propProxy, pSetItem, pElement, getPyObjectFromDataType(entry.second), NULL);

    if (!ret) {
      ok = false;
    }

    Py_XDECREF(pPropName);
    Py_XDECREF(pElement);
    Py_XDECREF(propProxy);
    Py_XDECREF(ret);

    if (!ok) {
      break;
    }
  }
  tlp::Observable::unholdObservers();
  Py_XDECREF(pGetItem);
  Py_XDECREF(pSetItem);
  return ok;
}

%End

typedef std::pair<tlp::node, tlp::node> pairNodeNode;

namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;
typedef tlp::PropertyInterface* PropertyPointer;
typedef tlp::Iterator<tlp::PropertyPointer> IteratorProperty;

enum ElementType {NODE=0, EDGE};

class Graph : tlp::Observable /Abstract/ {
%TypeHeaderCode
#include <tulip/Graph.h>
%End

%TypeCode
#include <tulip/PythonCppTypesConverter.h>
%End

%Docstring
This is the main Tulip class. It enables to :

  * create a directed graph
  * create and manipulate a sub-graphs hierarchy
  * get semantic iterators on graph elements
  * create and retrieve properties of different types to get/set data on the graph

This class can not be instanced directly. Use :func:`tlp.newGraph()` to create a new
empty graph.
%End

public:

  Graph();

  ~Graph();
%MethodCode
  releaseGraphHierarchyWrappers(sipCpp);
%End

//===========================================================================================

  bool applyAlgorithm(const std::string &algorithm, std::string &errorMessage /Out/, tlp::DataSet *dataSet /GetWrapper/ = NULL) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyAlgorithm(algoName, params=None)

.. versionadded:: 3.7

Applies an algorithm plugin, identified by its name.
Algorithm plugins are objects implementing the tlp::Algorithm interface 
in C++ or the :class:`tlp.Algorithm` interface in Python.

Parameters can be transmit to the algorithm using a Python dictionnary
filled with parameters values where keys are of type string (parameters names) .
In some cases, algorithms
can also use this dictionnary in order to return some external information.
To determine a plugin's parameters, you can either:

  * refer to its :ref:`documentation <algorithmpluginsdoc>`
  * call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: The name of the algorithm to apply.
:type algoName: string
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: (boolean, string)
%End

%MethodCode
  if (pluginExists<tlp::Algorithm>(*a0)) {
    tlp::DataSet *params = prepareAlgorithmParameters(*a0, sipCpp, a2, a2Wrapper);
    if (params) {
      sipRes = sipCpp->applyAlgorithm(*a0, *a1, params);
      updateWrappedDataSetAfterAlgorithmCall(params, a2Wrapper);
      delete params;
    } else {
      sipIsErr = 1;
    }
  } else {
    sipIsErr = 1;
    std::string msg = "No Tulip algorithm plugin named ";
    msg += *a0;
    msg += ".";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
%End

//===========================================================================================

  void clear();
%Docstring
tlp.Graph.clear()

Remove all nodes, edges and sub-graphs from the graph .
%End

//===========================================================================================

  tlp::Graph *addSubGraph(tlp::BooleanProperty *selection=0, const std::string& name = "unnamed");
%Docstring
tlp.Graph.addSubGraph(selection=None, name="unnamed")

Creates and returns a new sub-graph. The elements of the new sub-graph are those 
selected in the selection. If there is no selection an empty sub-graph is returned.

:param selection: a Boolean property whose selected elements will be added to the sub-graph
:type selection: :class:`tlp.BooleanProperty`
:param name: The name of the newly created sub-graph. Defaults to "unnamed".
:type name: string
:rtype: :class:`tlp.Graph`
%End

%MethodCode
  if (!a0 || isValidGraphSelection(sipCpp, a0)) {
    sipRes = sipCpp->addSubGraph(a0, *a1);
  } else {
    sipIsErr = -1;
    std::ostringstream oss;
    oss << "The selection provided as parameter to the tlp.Graph.addSubGraph([selection, name]) method is not valid." << std::endl;
    oss << "It means that some edges extremities are not in the selection." << std::endl;
    PyErr_SetString(PyExc_Exception, oss.str().c_str());
  }
%End


//===========================================================================================

  tlp::Graph *addSubGraph(const std::string& name);
%Docstring
tlp.Graph.addSubGraph(name)

.. versionadded:: 3.7

Creates and returns a new named sub-graph of this graph.

:param name: The name of the newly created sub-graph.
:type name: string
:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

  tlp::Graph* addCloneSubGraph(const std::string& name = "unnamed", bool addSibling = false, bool addSiblingProperties = false);
%Docstring
tlp.Graph.addCloneSubGraph(name="unnamed", addSibling=False, addSiblinProperties=False)

.. versionadded:: 4.5

Creates and returns a sub-graph of this graph (or the parent of this graph) that contains all its elements.

:param name: The name of the newly created sub-graph. Defaults to "unnamed".
:type name: string
:param addSibling: if true a sibling of this graph will be created; a sub-graph of this graph if not. Defaults to false
:type addSibling: bool
:param addSiblingProperties: if true all the local properties of this graph will be copied into its clone sibling. Defaults to false
:type addSiblingProperties: bool
:rtype: :class:`tlp.Graph`
%End

//=========================================================================================== 

  tlp::Graph *inducedSubGraph(const std::set<tlp::node>& nodeSet, tlp::Graph *parentSubGraph = NULL, const std::string &name = "unnamed");
%Docstring
tlp.Graph.inducedSubGraph(nodeSet, parentSubGraph=None, name="unnamed")

.. deprecated:: 5.0

Creates and returns a new sub-graph of the graph induced by a set of nodes.
The sub-graph contains all the nodes from the set and all the existing edges
between two nodes in the set including self-loops.

:param nodeSet: the set of nodes from which to build the induced sub-graph
:type nodeSet: set of :class:`tlp.node`
:param parentSubGraph: If provided, is used as parent graph for the newly created sub-graph instead of the graph this method is called on.
:type parentSubGraph: :class:`tlp.Graph`
:param name: The name of the newly created subgraph
:type name: string
:rtype: :class:`tlp.Graph` 
%End

//===========================================================================================

  tlp::Graph *inducedSubGraph(const std::vector<tlp::node>& nodes, tlp::Graph *parentSubGraph = NULL, const std::string &name = "unnamed");
%Docstring
tlp.Graph.inducedSubGraph(nodes, parentSubGraph=None, name="unnamed")

.. versionadded:: 5.0

Creates and returns a new sub-graph of the graph induced by a set of nodes.
The sub-graph contains all the nodes from the set and all the existing edges
between two nodes in the set including self-loops.

:param nodes: the set of nodes from which to build the induced sub-graph
:type nodes: list of :class:`tlp.node`
:param parentSubGraph: If provided, is used as parent graph for the newly created sub-graph instead of the graph this method is called on.
:type parentSubGraph: :class:`tlp.Graph`
:param name: The name of the newly created subgraph
:type name: string
:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

  tlp::Graph *inducedSubGraph(tlp::BooleanProperty *selection, tlp::Graph *parentSubGraph = NULL, const std::string &name = "unnamed");
%Docstring
tlp.Graph.inducedSubGraph(selection, parentSubGraph=None, name="unnamed")

.. versionadded:: 4.10

Creates and returns a new sub-graph of the graph induced by a selection of nodes and edges.
The sub-graph contains all the nodes of the set induced by the selected nodes but also the source and target nodes from
the selected edges and all the existing edges between two nodes in the set including self-loops.

:param selection: a selection of nodes and edges
:type selection: :class:`tlp.BooleanProperty`
:param parentSubGraph: If provided, is used as parent graph for the newly created sub-graph instead of the graph this method is called on.
:type parentSubGraph: :class:`tlp.Graph`
:param name: The name of the newly created subgraph
:type name: string
:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

  void delSubGraph(tlp::Graph *subgraph);
%Docstring
tlp.Graph.delSubGraph(subgraph)

Delete a sub-graph of this graph. The sub-graph's sub-graphs become sub-graphs of the graph.

:param subgraph: the sub-graph to remove
:type subgraph: :class:`tlp.Graph`
%End

%MethodCode
  if (!a0) {
    PyErr_SetString(PyExc_TypeError, "Graph.delSubGraph(): argument 1 has unexpected type 'NoneType'");
    sipIsErr = 1;
  } else {
    if (a0->getSuperGraph() != sipCpp) {
      sipIsErr = throwInvalidSgException(sipCpp, a0);
    } else {
      // the sub-graph will be deleted after the call to tlp::Graph::delSubGraph(tlp::Graph *)
      // we need to release the SIP wrapper associated to the C++ pointer otherwise the following
      // Python code raises a segmentation fault :
      // sg = graph.addSubGraph()
      // graph.delSubGraph(sg)
      // print sg # or any operation on sg
      // An exception is now thrown claiming that the underlying C++ object has been deleted
      releaseGraphWrapper(a0);
      sipCpp->delSubGraph(a0);
    }
  }
%End

//===========================================================================================

  void delAllSubGraphs(tlp::Graph *subgraph);
%Docstring
tlp.Graph.delAllSubGraphs(subgraph)

Delete a sub-graph of this graph and all its sub-graphs.

:param subgraph: the sub-graph to remove
:type subgraph: :class:`tlp.Graph` 
%End

%MethodCode
  if (!a0) {
    PyErr_SetString(PyExc_TypeError, "Graph.delAllSubGraphs(): argument 1 has unexpected type 'NoneType'");
    sipIsErr = 1;
  } else {
    if (a0->getSuperGraph() != sipCpp) {
      sipIsErr = throwInvalidSgException(sipCpp, a0);
    } else {
      // the sub-graphs will be deleted after the call to tlp::Graph::delAllSubGraphs(tlp::Graph *)
      // we need to release the possible SIP wrappers associated to the C++ pointers otherwise the following
      // Python code raises a segmentation fault :
      // sg = graph.addSubGraph()
      // sg2 = sg.addSubGraph()
      // graph.delAllSubGraphs(sg2)
      // print sg2 # or any operation on sg2
      // An exception is now thrown claiming that the underlying C++ object has been deleted
      releaseGraphHierarchyWrappers(a0);
      sipCpp->delAllSubGraphs(a0);
    }
  }
%End

//===========================================================================================

  tlp::Graph* getSuperGraph() const;
%Docstring
tlp.Graph.getSuperGraph()

Returns the parent of the graph, if it has no parent (is the root graph), it returns itself.

:rtype: :class:`tlp.Graph` 
%End

//===========================================================================================

  tlp::Graph* getRoot() const;
%Docstring
tlp.Graph.getRoot()

Returns the root graph of the graph hierarchy.

:rtype: :class:`tlp.Graph` 
%End

//===========================================================================================

  void setSuperGraph(tlp::Graph *superGraph);
%Docstring
tlp.Graph.setSuperGraph(superGraph)

Sets the parent of the graph (use very carefully). Standard users should never use this method.

:param superGraph: the new parent for the graph in the hierarchy.
:type superGraph: :class:`tlp.Graph`
%End

//===========================================================================================

  tlp::IteratorGraph *getSubGraphs() const /TransferBack, TypeHint="Iterator[tlp.Graph]"/;
%Docstring
tlp.Graph.getSubGraphs()

Returns an iterator on all the sub-graphs of the graph.

:rtype: a Tulip iterator on :class:`tlp.Graph` objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<tlp::Graph *>(sipCpp->getSubGraphs());
%End

//===========================================================================================

  bool isSubGraph(tlp::Graph* graph) const;
%Docstring
tlp.Graph.isSubGraph(graph)

Returns :const:`True` if the graph argument is a direct sub-graph of the graph.

:param graph: a graph
:type graph: :class:`tlp.Graph`
:rtype: boolean 
%End

//===========================================================================================

  bool isDescendantGraph(tlp::Graph* graph) const;
%Docstring
tlp.Graph.isDescendantGraph(graph)

Returns :const:`True` if the graph argument is a descendant of this graph.

:param graph: a graph
:type graph: :class:`tlp.Graph`
:rtype: boolean 
%End

//===========================================================================================

  tlp::Graph* getSubGraph(unsigned int id) const;
%Docstring
tlp.Graph.getSubGraph(id)

Returns the sub-graph with the corresponding id or :const:`None` if there is no sub-graph with that id.

:param id: a graph id
:type id: integer
:rtype: :class:`tlp.Graph` or :const:`None` 
%End

//===========================================================================================

  tlp::Graph* getSubGraph(const std::string &name) const;
%Docstring
tlp.Graph.getSubGraph(name)

.. versionadded:: 3.7

Returns the sub-graph with the corresponding name or :const:`None` if there is no sub-graph with that name.

:param name: the name of the sub-graph to return
:type name: string
:rtype: :class:`tlp.Graph` or :const:`None` 
%End

//===========================================================================================

  tlp::Graph* getDescendantGraph(unsigned int id) const;
%Docstring
tlp.Graph.getDescendantGraph(id)

Returns the descendant graph with the corresponding id or :const:`None` if there is no such descendant graph.

:param id: a graph id
:type id: integer
:rtype: :class:`tlp.Graph` or :const:`None` 
%End

//===========================================================================================

    tlp::Graph* getDescendantGraph(const std::string &name) const;
%Docstring
tlp.Graph.getDescendantGraph(name)

.. versionadded:: 3.7

Returns the descendant graph with the corresponding name or :const:`None` if there is no such descendant graph.

:param name: the name of the descendant to return
:type name: string
:rtype: :class:`tlp.Graph` or :const:`None`
%End

//===========================================================================================

  tlp::Graph *getNthSubGraph(unsigned int n) const;
%Docstring
tlp.Graph.getNthSubGraph(n)

.. versionadded:: 3.7

Returns the nth sub-graph or :const:`None` if there is no such sub-graph.
Since order cannot be ensured in every implementation, this method should be equivalent to::

  i = 0
  for sg in graph.getSubGraphs():
    if i++ == n:
      return sg
  return None    

:param n: the index of the sub-graph to return
:type n: integer
:rtype: :class:`tlp.Graph` or :const:`None`
%End

//===========================================================================================

  unsigned  int numberOfSubGraphs() const;
%Docstring
tlp.Graph.numberOfSubGraphs()

.. versionadded:: 3.7

Returns the number of direct sub-graphs.

:rtype: integer
%End  
  
//===========================================================================================  
  
  unsigned int numberOfDescendantGraphs() const;
%Docstring
tlp.Graph.numberOfDescendantGraphs()

.. versionadded:: 3.7

Returns the number of descendant graphs.

:rtype: integer
%End

//===========================================================================================

  tlp::IteratorGraph *getDescendantGraphs() const /TransferBack, TypeHint="Iterator[tlp.Graph]"/;
%Docstring
tlp.Graph.getDescendantGraphs()

Returns an iterator over all the descendant graphs of the graph.

:rtype: a Tulip iterator on :class:`tlp.Graph` objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<tlp::Graph *>(sipCpp->getDescendantGraphs());
%End

//===========================================================================================

  tlp::node addNode(tlp::DataSet *propertiesValues = NULL);
%Docstring
tlp.Graph.addNode(propertiesValues=None)

Adds a new node in the graph and returns it. This node is also added in all 
the graph's ancestors to maintain the sub-graph relation between graphs.
Since Tulip 5.0, it is also possible to set graph properties values for the newly added node
through the use of a dictionnary (see :meth:`tlp.Graph.setNodePropertiesValues` for more details).

:param propertiesValues: an optionnal dictionnary containing graph properties values for the newly added node
:type propertiesValues: a dictionnary
:rtype: :class:`tlp.node`
%End

%MethodCode
  sipRes = new tlp::node(sipCpp->addNode());
  if (a0) {
    sipIsErr = setPropertiesValuesFromDataSet(sipCpp, a0, sipRes->id, true) ? 0 : 1;
    if (sipIsErr) {
      sipCpp->delNode(*sipRes);
    }
  }
%End

//===========================================================================================

  void addNodes(unsigned int nbNodes, std::vector<tlp::node>& addedNodes /Out/);
%Docstring
tlp.Graph.addNodes(nbNodes)

.. versionadded:: 3.7

Adds new nodes in the graph and returns them in a list.
The new nodes are also added in all the graph ancestors to maintain the sub-graph relation between graphs.

:param nbNodes: the number of nodes to add in the graph
:type nbNodes: integer
:rtype: list of :class:`tlp.node`

.. warning:: the addedNodes vector is cleared before adding nodes.

%End

//===========================================================================================

  void addNode(const tlp::node node);
%Docstring
tlp.Graph.addNode(node)

Adds an existing node in the graph. This node is also added in all the graph ancestors 
to maintain the sub-graph relation between graphs.

:param node: an existing node to add to the graph
:type node: :class:`tlp.node`
:throws: an exception if the node does not belong to the root graph

.. warning:: The node must be an element of the graph hierarchy, thus it must be an element of the root graph.

.. warning:: One can't add an existing node to the root graph.

%End

%MethodCode
  if (sipCpp->getRoot()->isElement(*a0)) {
    sipCpp->addNode(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp->getRoot(), *a0);
  }
%End

//===========================================================================================

  void addNodes(tlp::Iterator<tlp::node>* nodes /TypeHint="Iterator[tlp.node]"/);
%Docstring
tlp.Graph.addNodes(itNodes)

.. versionadded:: 3.7

Adds existing nodes in the graph. The nodes are also added in all
the graph ancestors to maintain the sub-graph relation between graphs.

:param itNodes: an iterator on nodes to add
:type itNodes: a Tulip iterator on :class:`tlp.node` objects
:throws: an exception if one of the node to add is not an element of the root graph.

.. warning:: The added nodes must be elements of the graph hierarchy, thus they must be elements of the root graph.

.. warning:: One can't add existing nodes to the root graph.

%End

%MethodCode
  tlp::StableIterator<tlp::node> sItNodes(a0);
  tlp::node n;
  while (sItNodes.hasNext()) {
    n = sItNodes.next();
    if (!sipCpp->getRoot()->isElement(n)) {
      sipIsErr = throwInvalidNodeException(sipCpp->getRoot(), n);
      break;
    }
  }
  if (sipIsErr == 0) {
    sItNodes.restart();
    sipCpp->addNodes(&sItNodes);
  }
%End

//===========================================================================================

  void addNodes(const std::vector<tlp::node>& nodes);
%Docstring
tlp.Graph.addNodes(nodes)

.. versionadded:: 4.5

Adds existing nodes in the graph. The nodes are also added in all
the graph ancestors to maintain the sub-graph relation between graphs.

:param nodes: a vector of nodes to add
:type nodes: a list of :class:`tlp.node` objects
:throws: an exception if one of the node to add is not an element of the root graph.

.. warning:: The added nodes must be elements of the graph hierarchy, thus they must be elements of the root graph.

.. warning:: One can't add existing nodes to the root graph.

%End

%MethodCode
  for (size_t i = 0 ; i < a0->size() ; ++i) {
    if (!sipCpp->getRoot()->isElement((*a0)[i])) {
      sipIsErr = throwInvalidNodeException(sipCpp->getRoot(), (*a0)[i]);
      break;
    }
  }
  if (sipIsErr == 0) {
    sipCpp->addNodes(*a0);
  }
%End

//===========================================================================================

  void delNode(const tlp::node node, bool deleteInAllGraphs = false);
%Docstring
tlp.Graph.delNode(node, deleteInAllGraphs=False)

Deletes a node in the graph. This node is also removed in all the sub-graphs of the graph 
to maintain the sub-graph relation between graphs. 

:param node: the node to delete
:type node: :class:`tlp.node` 
:param deleteInAllGraphs: if :const:`True`, remove the node in all the hierarchy of graphs
:type deleteInAllGraphs: boolean
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipCpp->delNode(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  void delNodes(tlp::Iterator<tlp::node>* itN /Transfer, TypeHint="Iterator[tlp.node]"/, bool deleteInAllGraphs = false);
%Docstring
tlp.Graph.delNodes(itNodes, deleteInAllGraphs=False)

.. versionadded:: 3.7

Deletes nodes in the graph. These nodes are also removed in
the sub-graphs hierarchy of the current graph to maintain
the sub-graph relation between graphs.

:param itNodes: an iterator on the nodes to add
:type itNodes: a Tulip iterator on :class:`tlp.node` objects
:param deleteInAllGraphs: if :const:`True`, these nodes are  deleted in the whole hierarchy of graphs.
:type deleteInAllGraphs: boolean
:throws: an exception if one of the node to delete is not an element of the graph.

%End

%MethodCode
  tlp::StableIterator<tlp::node> sItNodes(a0);
  tlp::node n;
  while (sItNodes.hasNext()) {
    n = sItNodes.next();
    if (!sipCpp->isElement(n)) {
      sipIsErr = throwInvalidNodeException(sipCpp, n);
      break;
    }
  }
  if (sipIsErr == 0) {
    sItNodes.restart();
    sipCpp->delNodes(&sItNodes, a1);
  }
%End

//===========================================================================================

  void delNodes(const std::vector<tlp::node>& nodes, bool deleteInAllGraphs = false);
%Docstring
tlp.Graph.delNodes(nodes, deleteInAllGraphs=False)

.. versionadded:: 4.5

Deletes nodes in the graph. These nodes are also removed in
the sub-graphs hierarchy of the current graph to maintain
the sub-graph relation between graphs.

:param nodes: a vector of nodes to add
:type nodes: a list of :class:`tlp.node` objects
:type itNodes: a Tulip iterator on :class:`tlp.node` objects
:param deleteInAllGraphs: if :const:`True`, these nodes are  deleted in the whole hierarchy of graphs.
:type deleteInAllGraphs: boolean
:throws: an exception if one of the node to delete is not an element of the graph.

%End

%MethodCode
  for (size_t i = 0 ; i < a0->size() ; ++i) {
    if (!sipCpp->isElement((*a0)[i])) {
      sipIsErr = throwInvalidNodeException(sipCpp->getRoot(), (*a0)[i]);
      break;
    }
  }
  if (sipIsErr == 0) {
    sipCpp->delNodes(*a0, a1);
  }
%End

//===========================================================================================

  tlp::edge addEdge(const tlp::node src, const tlp::node tgt, tlp::DataSet *propertiesValues = NULL);
%Docstring
tlp.Graph.addEdge(src, tgt, propertiesValues=None)

Adds a new edge in the graph and returns it. This edge is also added in all 
the graph's ancestors to maintain the sub-graph relation between graphs.
Since Tulip 5.0, it is also possible to set graph properties values for the newly added edge
through the use of a dictionnary (see :meth:`tlp.Graph.setEdgePropertiesValues` for more details).

:param src: the source node of the new edge
:param tgt: the target node of the new edge
:type src: :class:`tlp.node`
:type tgt: :class:`tlp.node`
:param propertiesValues: an optionnal dictionnary containing graph properties values for the newly added edge
:type propertiesValues: a dictionnary
:rtype: :class:`tlp.edge`
:throws: an exception if the provided source or target node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->isElement(*a1)) {
      sipRes = new tlp::edge(sipCpp->addEdge(*a0, *a1));
      if (a2) {
        sipIsErr = setPropertiesValuesFromDataSet(sipCpp, a2, sipRes->id, false) ? 0 : 1;
        if (sipIsErr) {
          sipCpp->delEdge(*sipRes);
        }
      }
    } else {
      sipIsErr = throwInvalidNodeException(sipCpp, *a1);
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  void addEdges(const std::vector<pairNodeNode>& edges, std::vector<tlp::edge>& addedEdges /Out/);
%Docstring
tlp.Graph.addEdges(edges)

.. versionadded:: 3.7

Adds new edges in the graph and returns them in a list.
The new edges are also added in all the graph ancestors to maintain the sub-graph relation between graphs.

:param edges: the list of edges to add described by a pair of nodes
:type edges: list of tuples containing two :class:`tlp.node` objects
:rtype: list of :class:`tlp.edge`
:throws: an exception if one of the provided nodes to link is not an element of the graph.
%End

%MethodCode
  for (size_t i = 0 ; i < a0->size() ; ++i) {
    if (!sipCpp->isElement((*a0)[i].first)) {
      sipIsErr = throwInvalidNodeException(sipCpp, (*a0)[i].first);
      break;
    }
    if (!sipCpp->isElement((*a0)[i].second)) {
      sipIsErr = throwInvalidNodeException(sipCpp, (*a0)[i].second);
      break;
    }
  }
  if (sipIsErr == 0) {
    sipCpp->addEdges(*a0, *a1);
  }
%End

//===========================================================================================

  void addEdge(const tlp::edge edge);
%Docstring
tlp.Graph.addEdge(edge)

Adds an existing edge in the graph. This edge is also added in all 
the graph's ancestors to maintain the sub-graph relation between graphs. 

:param edge: an existing edge to add to the graph
:type edge: :class:`tlp.edge`
:throws: an exception if the edge does not belong to the root graph of if the source and target node of the edge are not elements of the graph

.. warning:: The edge must be an element of the graph hierarchy, thus it must be an element of the root graph.

.. warning:: One can't add an existing edge to the root graph.

%End

%MethodCode
  if (sipCpp->getRoot()->isElement(*a0)) {
    if (sipCpp->isElement(sipCpp->getRoot()->source(*a0))) {
      if (sipCpp->isElement(sipCpp->getRoot()->target(*a0))) {
        sipCpp->addEdge(*a0);
      } else {
        std::ostringstream oss;
        oss << "Error : edge " << a0->id << " can not be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
            << "because the graph does not contain its target node.";
        printErrorMessage(oss.str());
        sipIsErr = throwInvalidNodeException(sipCpp, sipCpp->getRoot()->target(*a0));
      }
    } else {
      std::ostringstream oss;
      oss << "Error : edge " << a0->id << " can not be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
          << "because the graph does not contain its source node.";
      printErrorMessage(oss.str());
      sipIsErr = throwInvalidNodeException(sipCpp, sipCpp->getRoot()->source(*a0));
    }
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp->getRoot(), *a0);
  }
%End

//===========================================================================================

  void addEdges(tlp::Iterator<tlp::edge>* edges /TypeHint="Iterator[tlp.edge]"/);
%Docstring
tlp.Graph.addEdges(itEdges)

.. versionadded:: 3.7

Adds existing edges in the graph. The edges are also added in all
the graph ancestors to maintain the sub-graph relation between graphs.

:param itEdges: an iterator on the edges to add
:type itEdges: a Tulip iterator on :class:`tlp.edge` objects
:throws: an exception if one of the edge to add is not an element of the root graph or if one of the edge extremities is not an element of the graph.

.. warning:: The added edges must be elements of the graph hierarchy, thus they must be elements of the root graph.

.. warning : One cannot add existing edges to the root graph.
%End

%MethodCode
  tlp::StableIterator<tlp::edge> sItEdges(a0);
  tlp::edge e;
  while (sItEdges.hasNext()) {
    e = sItEdges.next();
    if (!sipCpp->getRoot()->isElement(e)) {
      sipIsErr = throwInvalidEdgeException(sipCpp->getRoot(), e);
      break;
    } else {
      if (!sipCpp->isElement(sipCpp->getRoot()->source(e))) {
        std::ostringstream oss;
        oss << "Error : edge " << e.id << " cannot be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
            << "because the graph does not contain its source node.";
        printErrorMessage(oss.str());
        sipIsErr = throwInvalidNodeException(sipCpp, sipCpp->getRoot()->source(e));
        break;
      }
      if (!sipCpp->isElement(sipCpp->getRoot()->target(e))) {
        std::ostringstream oss;
        oss << "Error : edge " << e.id << " cannot be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
            << "because the graph does not contain its target node.";
        printErrorMessage(oss.str());
        sipIsErr = throwInvalidNodeException(sipCpp, sipCpp->getRoot()->target(e));
        break;
      }
    }
  }
  if (sipIsErr == 0) {
    sItEdges.restart();
    sipCpp->addEdges(&sItEdges);
  }
%End
//===========================================================================================

  void addEdges(const std::vector<tlp::edge>& list_edges);
%Docstring
tlp.Graph.addEdges(list_edges)

.. versionadded:: 4.5

Adds existing edges in the graph. The edges are also added in all
the graph ancestors to maintain the sub-graph relation between graphs.

:param list_edges: a list of edges to add
:type list_edges: a list of :class:`tlp.edge` objects
:throws: an exception if one of the edge to add is not an element of the root graph or if one of the edge extremities is not an element of the graph.

.. warning:: The added edges must be elements of the graph hierarchy, thus they must be elements of the root graph.

.. warning:: It is not possible to add existing edges to the root graph.

%End

%MethodCode
  for (size_t i = 0 ; i < a0->size() ; ++i) {
    tlp::edge e = (*a0)[i];
    if (!sipCpp->getRoot()->isElement(e)) {
      sipIsErr = throwInvalidEdgeException(sipCpp->getRoot(), e);
      break;
    } else {
      if (!sipCpp->isElement(sipCpp->getRoot()->source(e))) {
          std::ostringstream oss;
          oss << "Error : edge " << e.id << " cannot be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
              << "because the graph does not contain its source node.";
          printErrorMessage(oss.str());
          sipIsErr = throwInvalidNodeException(sipCpp, sipCpp->getRoot()->source(e));
          break;
      }
      if (!sipCpp->isElement(sipCpp->getRoot()->target(e))) {
          std::ostringstream oss;
          oss << "Error : edge " << e.id << " cannot be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
              << "because the graph does not contain its target node.";
          printErrorMessage(oss.str());
          sipIsErr = throwInvalidNodeException(sipCpp, sipCpp->getRoot()->target(e));
          break;
      }
    }
  }
  if (sipIsErr == 0) {
    sipCpp->addEdges(*a0);
  }
%End

//===========================================================================================

  void delEdge(const tlp::edge edge, bool deleteInAllGraphs = false);
%Docstring
tlp.Graph.delEdge(edge, deleteInAllGraphs=False)

Deletes an edge in the graph. This edge is also removed in all the sub-graphs 
of the graph to maintain the sub-graph relation between graphs. The ordering of edges is preserved. 

:param edge: the edge to delete
:type edge: :class:`tlp.edge` 
:param deleteInAllGraphs: if :const:`True`, remove the edge in all the hierarchy of graphs
:type deleteInAllGraphs: boolean
:throws: an exception if the edge does not belong to the graph
%End 
 
%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipCpp->delEdge(*a0);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  void delEdges(tlp::Iterator<tlp::edge>* itE /Transfer, TypeHint="Iterator[tlp.edge]"/, bool deleteInAllGraphs = false)=0;
%Docstring
tlp.Graph.delEdges(itEdges, deleteInAllGraphs=False)

.. versionadded:: 3.7

Deletes edges in the graph. These edges are also removed in
the sub-graphs hierarchy of the current graph to maintain
the sub-graph relation between graphs.
The ordering of the remaining edges is preserved.

:param itEdges: an iterator on the edges to delete
:type itEdges: a Tulip iterator on :class:`tlp.edge` objects
:param deleteInAllGraphs: If :const:`True`, the edges are deleted in the whole graph hierarchy.
:param deleteInAllGraphs: Boolean
:throws: an exception if one of the edge to delete is not an element of the graph
%End

%MethodCode
  tlp::StableIterator<tlp::edge> sItEdges(a0);
  tlp::edge e;
  while (sItEdges.hasNext()) {
    e = sItEdges.next();
    if (!sipCpp->isElement(e)) {
      sipIsErr = throwInvalidEdgeException(sipCpp, e);
      break;
    }
  }
  if (sipIsErr == 0) {
    sItEdges.restart();
    sipCpp->delEdges(&sItEdges, a1);
  }
%End

//===========================================================================================

  void delEdges(const std::vector<tlp::edge>& edges, bool deleteInAllGraphs = false);
%Docstring
tlp.Graph.delEdges(edges, deleteInAllGraphs=False)

.. versionadded:: 4.5

Deletes edges in the graph. These edges are also removed in
the sub-graphs hierarchy of the current graph to maintain
the sub-graph relation between graphs.

:param edges: a list of edges to delete
:type edges: a list of :class:`tlp.edge` objects
:type itEdges: a Tulip iterator on :class:`tlp.edge` objects
:param deleteInAllGraphs: if :const:`True`, these edges are deleted in the whole graph hierarchy.
:type deleteInAllGraphs: Boolean
:throws: an exception if one of the edge to delete is not an element of the graph
%End

%MethodCode
  for (size_t i = 0 ; i < a0->size() ; ++i) {
    if (!sipCpp->isElement((*a0)[i])) {
      sipIsErr = throwInvalidEdgeException(sipCpp, (*a0)[i]);
      break;
    }
  }
  if (sipIsErr == 0) {
    sipCpp->delEdges(*a0, a1);
  }
%End

//===========================================================================================

  void setEdgeOrder(const tlp::node node, const std::vector<tlp::edge> & edges);
%Docstring
tlp.Graph.setEdgeOrder(node, edges)

Sets the order of the edges around a node. This operation ensures that adjacent edges 
of a node will be ordered as they are in the list of edges given in parameter.

:param node: the node on which to set edges ordering
:type node: :class:`tlp.node`
:param edges: the list of edges adjacent to the node
:type edges: list of :class:`tlp.edge`
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipCpp->setEdgeOrder(*a0, *a1);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  void swapEdgeOrder(const tlp::node node, const tlp::edge edge1, const tlp::edge edge2);
%Docstring
tlp.Graph.swapEdgeOrder(node, edge1, edge2)

Swaps two edges in the adjacency list of a node.

:param node: the node on which swapping two edges in its adjacency list
:type node: :class:`tlp.node`
:param edge1: an edge adjacent to the node
:param edge2: another edge adjacent to the node
:type edge1: :class:`tlp.edge`
:type edge2: :class:`tlp.edge`
:throws: an exception if the node or one of the edges does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->isElement(*a1)) {
      if (sipCpp->isElement(*a2)) {
        sipCpp->swapEdgeOrder(*a0, *a1, *a2);
      } else {
        sipIsErr = throwInvalidEdgeException(sipCpp, *a2);
      }
    } else {
      sipIsErr = throwInvalidEdgeException(sipCpp, *a1);
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

void setSource(const tlp::edge edge, const tlp::node src);
%Docstring
tlp.Graph.setSource(edge, src)

Sets the source of an existing edge.

:param edge: the edge on which to change the source node
:type edge: :class:`tlp.edge`
:param src: the new source node of the edge
:type src: :class:`tlp.node`
:throws: an exception if the edge or the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->isElement(*a1)) {
      sipCpp->setSource(*a0, *a1);
    } else {
      sipIsErr = throwInvalidNodeException(sipCpp, *a1);
    }
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End
  
//===========================================================================================  
  
void setTarget(const tlp::edge edge, const tlp::node tgt);
%Docstring
tlp.Graph.setTarget(edge, tgt)

Sets the target of an existing edge.

:param edge: the edge on which to change the target node
:type edge: :class:`tlp.edge`
:param tgt: the new target node of the edge
:type tgt: :class:`tlp.node`
:throws: an exception if the edge or node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->isElement(*a1)) {
      sipCpp->setTarget(*a0, *a1);
    } else {
      sipIsErr = throwInvalidNodeException(sipCpp, *a1);
    }
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

  
//===========================================================================================  
  
void setEnds(const tlp::edge edge, const tlp::node src, const tlp::node tgt);
%Docstring
tlp.Graph.setEnds(edge, src, tgt)

Sets both the source and target of an existing edge. 

:param edge: the edge on which to change the source and target nodes
:type edge: :class:`tlp.edge`
:param src: the new source node of the edge
:type src: :class:`tlp.node`
:param tgt: the new target node of the edge
:type tgt: :class:`tlp.node`
:throws: an exception if the edge or one of the nodes does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->isElement(*a1)) {
      if (sipCpp->isElement(*a2)) {
        sipCpp->setEnds(*a0, *a1, *a2);
      } else {
        sipIsErr = throwInvalidNodeException(sipCpp, *a2);
      }
    } else {
      sipIsErr = throwInvalidNodeException(sipCpp, *a1);
    }
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  void reverse(const tlp::edge edge);
%Docstring
tlp.Graph.reverse(edge)

Reverses the direction of an edge, the source becomes the target 
and the target becomes the source. 

:param edge: the edge on which to revert the direction.
:type edge: :class:`tlp.edge` 
:throws: an exception if the edge does not belong to the graph

.. warning:: The ordering is global to the entire graph hierarchy. Thus, by changing the ordering in a graph you change the ordering in the hierarchy.

%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipCpp->reverse(*a0);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::node getOneNode() const;
%Docstring
tlp.Graph.getOneNode()

Returns the first node in the graph.

:rtype: :class:`tlp.node`  
%End

//===========================================================================================

  tlp::node getRandomNode() const;
%Docstring
tlp.Graph.getRandomNode()

.. versionadded:: 4.8

Returns a random node in the graph.

:rtype: :class:`tlp.node`
%End

//===========================================================================================

  tlp::node getSource() const;
%Docstring
tlp.Graph.getSource()

.. versionadded:: 3.7

Finds the first node whose input degree equals 0.
Returns the found node or an invalid one if there is no source.

:rtype: :class:`tlp.node`
%End

//===========================================================================================

tlp::node getSink() const;
%Docstring
tlp.Graph.getSink()

.. versionadded:: 5.0

Finds the first node whose output degree equals 0.
Returns the found node or an invalid one if there is no sink.

:rtype: :class:`tlp.node`
%End


//===========================================================================================

  const std::vector<tlp::node>& nodes() const /NoCopy/;
%Docstring
tlp.Graph.nodes()

.. versionadded:: 5.0

Returns the list of nodes belonging to the graph.

:rtype: list of :class:`tlp.node`
%End
//===========================================================================================

  unsigned int nodePos(const tlp::node node) const;
%Docstring
tlp.Graph.nodePos(node)

.. versionadded:: 5.0

Returns the position of a node in the list of nodes belonging to the graph
that can be accessed through the :meth:`tlp.Graph.nodes` method.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: integer
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = sipCpp->nodePos(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End
//===========================================================================================

  tlp::Iterator<tlp::node>* getNodes() const /TransferBack, TypeHint="Iterator[tlp.node]"/;
%Docstring
tlp.Graph.getNodes()

Returns an iterator on the nodes.

:rtype: a Tulip iterator on :class:`tlp.node` objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getNodes());
%End

//===========================================================================================

  tlp::node getInNode(const tlp::node node, unsigned int i) const;
%Docstring
tlp.Graph.getInNode(node, i)

Returns the ith predecessor of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:param i: the position in the predecessor nodes list (warning : first index is 1 not 0)
:type i: integer
:rtype: :class:`tlp.node`
:throws: an exception if the node does not belong to the graph or if the requested index is lesser than the number of predecessor nodes
%End

 
%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (a1 == 0) {
      PyErr_SetString(PyExc_Exception, "Error : first index for tlp.Graph.getInNode is 1 not 0");
      sipIsErr = -1;
    } else if (a1 <= sipCpp->indeg(*a0)) {
      sipRes = new tlp::node(sipCpp->getInNode(*a0, a1));
    } else {
      std::ostringstream oss;
      std::string graphName;
      sipCpp->getAttribute("name", graphName);
      oss << "node with id " << a0->id << " belonging to graph \"" << graphName << "\" (id " << sipCpp->getId() << ") has " << sipCpp->indeg(*a0) << " predecessor nodes and the requested index is " << a1; 
      PyErr_SetString(PyExc_Exception, oss.str().c_str());
      sipIsErr = -1;
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::Iterator<tlp::node>* getInNodes(const tlp::node node) const /TransferBack, TypeHint="Iterator[tlp.node]"/;
%Docstring
tlp.Graph.getInNodes(node)

Return an iterator on the predecessors of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: a Tulip iterator on :class:`tlp.node` objects
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getInNodes(*a0));
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::node getOutNode(const tlp::node node, unsigned int i) const;
%Docstring
tlp.Graph.getOutNode(node, i)

Returns the ith successor of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:param i: the position in the successor nodes list (warning : first index is 1 not 0)
:type i: integer
:rtype: :class:`tlp.node`
:throws: an exception if the node does not belong to the graph or if the requested index is lesser than the number of successor nodes
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (a1 == 0) {
      PyErr_SetString(PyExc_Exception, "Error : first index for tlp.Graph.getOutNode is 1 not 0");
      sipIsErr = -1;
    }else if (a1 <= sipCpp->outdeg(*a0)) {
      sipRes = new tlp::node(sipCpp->getOutNode(*a0, a1));
    } else {
      std::ostringstream oss;
      std::string graphName;
      sipCpp->getAttribute("name", graphName);
      oss << "node with id " << a0->id << " belonging to graph \"" << graphName << "\" (id " << sipCpp->getId() << ") has " << sipCpp->outdeg(*a0) << " successor nodes and the requested index is " << a1; 
      PyErr_SetString(PyExc_Exception, oss.str().c_str());
      sipIsErr = -1;
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }

%End

//===========================================================================================

  tlp::Iterator<tlp::node>* getOutNodes(const tlp::node node) const /TransferBack, TypeHint="Iterator[tlp.node]"/;
%Docstring
tlp.Graph.getOutNodes(node)

Returns an iterator on the successors of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: a Tulip iterator on :class:`tlp.node` objects
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getOutNodes(*a0));
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::Iterator<tlp::node>* getInOutNodes(const tlp::node node) const /TransferBack, TypeHint="Iterator[tlp.node]"/;
%Docstring
tlp.Graph.getInOutNodes(node)

Returns an iterator on the neighbours of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: a Tulip iterator on :class:`tlp.node` objects
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getInOutNodes(*a0));
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::Iterator<tlp::node>* bfs(const tlp::node root = tlp::node()) const /TransferBack, TypeHint="Iterator[tlp.node]"/;
%Docstring
tlp.Graph.bfs(root=tlp.node())

.. versionadded:: 4.2

Returns an iterator performing a breadth-first search on the graph.

:param root: The node from whom to start the BFS. If not provided, the root node will be assigned to a source node in the graph (node with input degree equals to 0). If there is no source node in the graph, a random node will be picked.
:type root: :class:`tlp.node`
:rtype: a Tulip iterator on :class:`tlp.node` objects in the BFS order.
:throws: an exception if the provided root node does not belong to the graph.
%End

%MethodCode
  if (!a0->isValid() || sipCpp->isElement(*a0)) {
    // tlp::Graph::bfs returns a StableIterator
    sipRes = sipCpp->bfs(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::Iterator<tlp::node>* dfs(const tlp::node root = tlp::node()) const /TransferBack, TypeHint="Iterator[tlp.node]"/;
%Docstring
tlp.Graph.dfs(root=tlp.node())

.. versionadded:: 4.2

Returns an iterator performing a depth-first search on the graph.

:param root: The node from whom to start the DFS. If not provided, the root node will be assigned to a source node in the graph (node with input degree equals to 0). If there is no source node in the graph, a random node will be picked.
:type root: :class:`tlp.node`
:rtype: a Tulip iterator on :class:`tlp.node` objects in the DFS order.
:throws: an exception if the provided root node does not belong to the graph.
%End

%MethodCode
  if (!a0->isValid() || sipCpp->isElement(*a0)) {
    // tlp::Graph::dfs returns a StableIterator
    sipRes = sipCpp->dfs(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::Graph* getNodeMetaInfo(const tlp::node node) const;
%Docstring
tlp.Graph.getNodeMetaInfo(node)

Returns the underlying graph of a meta node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: :class:`tlp.Graph`
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = sipCpp->getNodeMetaInfo(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  const std::vector<tlp::edge>& edges() const /NoCopy/;
%Docstring
tlp.Graph.edges()

.. versionadded:: 5.0

Returns the list of edges belonging to the graph.

:rtype: list of :class:`tlp.node`

%End
//===========================================================================================

  unsigned int edgePos(const tlp::edge edge) const;
%Docstring
tlp.Graph.edgePos(edge)

.. versionadded:: 5.0

Returns the position of an edge in the list of edges belonging to the graph
that can be accessed through the :meth:`tlp.Graph.edges` method.

:param edge: an existing edge of the graph
:type node: :class:`tlp.edge`
:rtype: integer
:throws: an exception if the edge does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = sipCpp->edgePos(*a0);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End
//===========================================================================================

  tlp::Iterator<tlp::edge>* getEdges() const /TransferBack, TypeHint="Iterator[tlp.edge]"/;
%Docstring
tlp.Graph.getEdges()

Returns an iterator on the edges.

:rtype: a Tulip iterator on :class:`tlp.edge` objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getEdges());
%End

//===========================================================================================

  tlp::edge getOneEdge() const;
%Docstring
tlp.Graph.getOneEdge()

Returns the first edge in the graph.

:rtype: :class:`tlp.edge`  
%End

//===========================================================================================

  tlp::edge getRandomEdge() const;
%Docstring
tlp.Graph.getRandomEdge()

.. versionadded:: 4.8

Returns a random edge in the graph.

:rtype: :class:`tlp.edge`
%End

//===========================================================================================

  tlp::Iterator<tlp::edge>* getOutEdges(const tlp::node node) const /TransferBack, TypeHint="Iterator[tlp.edge]"/;
%Docstring
tlp.Graph.getOutEdges(node)

Returns a Tulip iterator on the outgoing edges of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: a Tulip iterator on :class:`tlp.edge` objects
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getOutEdges(*a0));
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::Iterator<tlp::edge>* getInOutEdges(const tlp::node node) const /TransferBack, TypeHint="Iterator[tlp.edge]"/;
%Docstring
tlp.Graph.getInOutEdges(node)

Returns an iterator on the incoming and outgoing edges of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: a Tulip iterator on :class:`tlp.edge` objects
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getInOutEdges(*a0));
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::Iterator<tlp::edge>* getInEdges(const tlp::node node) const /TransferBack, TypeHint="Iterator[tlp.edge]"/;
%Docstring
tlp.Graph.getInEdges(node)

Returns an iterator on the incoming edges of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: a Tulip iterator on :class:`tlp.edge` objects
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getInEdges(*a0));
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::Iterator<tlp::edge>* getEdgeMetaInfo(const tlp::edge edge) const /TransferBack, TypeHint="Iterator[tlp.edge]"/;
%Docstring
tlp.Graph.getEdgeMetaInfo(edge)

Returns an iterator on the underlying edges of a meta edge.

:param edge: an existing edge of the graph
:type edge: :class:`tlp.edge`
:rtype: a Tulip iterator on :class:`tlp.edge` objects
:throws: an exception if the edge does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getEdgeMetaInfo(*a0));
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  const std::vector<tlp::edge>& allEdges(const tlp::node node) const;
%Docstring
tlp.Graph.allEdges(node)

.. versionadded:: 5.0

Returns the list of all edges adjacent to a given node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: list of :class:`tlp.edge`
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new std::vector<tlp::edge>();
    const std::vector<tlp::edge> &edges = sipCpp->allEdges(*a0);
    sipRes->reserve(edges.size());
    for (size_t i = 0 ; i < edges.size() ; ++i) {
      if (sipCpp->isElement(edges[i])) {
        sipRes->push_back(edges[i]);
      }
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  unsigned int getId() const;
%Docstring
tlp.Graph.getId()

Returns the graph's id. This id is unique.

:rtype: integer
%End

//===========================================================================================

  unsigned int numberOfNodes() const;
%Docstring
tlp.Graph.numberOfNodes()

Returns the number of nodes in the graph.

:rtype: integer
%End

//===========================================================================================

  unsigned int numberOfEdges() const;
%Docstring
tlp.Graph.numberOfEdges()

Returns the number of edges in the graph.

:rtype: integer
%End

//===========================================================================================

  unsigned int deg(const tlp::node node) const;
%Docstring
tlp.Graph.deg(node)

Returns the degree of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: integer
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = sipCpp->deg(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  unsigned int indeg(const tlp::node node) const;
%Docstring
tlp.Graph.indeg(node)

Returns the incoming degree of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: integer
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = sipCpp->indeg(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  unsigned int outdeg(const tlp::node node) const;
%Docstring
tlp.Graph.outdeg(node)

Returns the outgoing degree of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: integer
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = sipCpp->outdeg(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::node source(const tlp::edge edge) const;
%Docstring
tlp.Graph.source(edge)

Returns the source of the edge.

:param edge: an existing edge of the graph
:type edge: :class:`tlp.edge`
:rtype: :class:`tlp.node`
:throws: an exception if the edge does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::node(sipCpp->source(*a0));
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::node target(const tlp::edge edge) const;
%Docstring
tlp.Graph.target(edge)

Returns the target of the edge.

:param edge: an existing edge of the graph
:type edge: :class:`tlp.edge`
:rtype: :class:`tlp.node`
:throws: an exception if the edge does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new tlp::node(sipCpp->target(*a0));
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  const std::pair<tlp::node, tlp::node>& ends(const tlp::edge edge) const;
%Docstring
tlp.Graph.ends(edge)

Returns a tuple containing the two end nodes of an edge

:param edge: an existing edge of the graph
:type node: :class:`tlp.edge`
:rtype: (:class:`tlp.node`, :class:`tlp.node`) 
:throws: an exception if the edge does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = new std::pair<tlp::node, tlp::node>(sipCpp->ends(*a0));
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::node opposite(const tlp::edge edge, const tlp::node node) const;
%Docstring
tlp.Graph.opposite(edge, node)

Returns the opposite node of the edge for the given node.

:param edge: an existing edge of the graph
:type edge: :class:`tlp.edge`
:rtype: :class:`tlp.node`
:throws: an exception if the edge does not belong to the graph or if the given node is not linked by the edge
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->source(*a0) == *a1 || sipCpp->target(*a0) == *a1) {  
      sipRes = new tlp::node(sipCpp->opposite(*a0, *a1));
    } else {
      std::ostringstream oss;
      oss << "node with id " << a1->id << " is not linked by the edge with id " << a0->id,
      PyErr_SetString(PyExc_Exception, oss.str().c_str());
      sipIsErr = -1;
    }
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  bool isElement(const tlp::node node) const;
%Docstring
tlp.Graph.isElement(node)

Returns :const:`True` if the node is an element of the graph.

:param node: a node
:type node: :class:`tlp.node`
:rtype: boolean
%End

//===========================================================================================

  bool isElement(const tlp::edge edge) const;
%Docstring
tlp.Graph.isElement(edge)

Returns :const:`True` if the edge is an element of the graph.

:param edge: an edge
:type edge: :class:`tlp.edge`
:rtype: boolean
%End

//===========================================================================================

  bool isMetaNode(const tlp::node node) const;
%Docstring
tlp.Graph.isMetaNode(node)

Returns :const:`True` if the node is a meta-node.

:param node: an existing node of the graph
:type node: :class:`tlp.edge`
:rtype: boolean
:throws: an exception if the node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = sipCpp->isMetaNode(*a0);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  bool isMetaEdge(const tlp::edge edge) const;
%Docstring
tlp.Graph.isMetaEdge(edge)

Returns :const:`True` if the edge is a meta-edge.

:param edge: an existing edge of the graph
:type node: :class:`tlp.edge`
:rtype: boolean
:throws: an exception if the edge does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = sipCpp->isMetaEdge(*a0);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::edge existEdge(const tlp::node node1, const tlp::node node2, bool directed=true) const;
%Docstring
tlp.Graph.existEdge(node1, node2, directed=True)

Returns the first edge found between two nodes.
If no edge is found, returns an invalid edge.

:param node1: an existing node of the graph
:type node1: :class:`tlp.node`
:param node2: an existing node of the graph
:type node2: :class:`tlp.node`
:param directed: indicates if the direction of the edge (from source to target) must be taken in to account
:type directed: boolean
:rtype: :class:`tlp.edge`
:throws: an exception if one of the nodes does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->isElement(*a1)) {
      sipRes = new tlp::edge(sipCpp->existEdge(*a0, *a1, a2));
    } else {
      sipIsErr = throwInvalidNodeException(sipCpp, *a1);
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  bool hasEdge(const tlp::node node1, const tlp::node node2, bool directed=true) const;
%Docstring
tlp.Graph.hasEdge(node1, node2, directed=True)

.. versionadded:: 4.2

Returns true if it exists an edge between two nodes.
If no edge is found, returns false.

:param node1: an existing node of the graph
:type node1: :class:`tlp.node`
:param node2: an existing node of the graph
:type node2: :class:`tlp.node`
:param directed: indicates if the direction of the edge (from source to target) must be taken in to account
:type directed: boolean
:rtype: boolean
:throws: an exception if one of the nodes does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->isElement(*a1)) {
      sipRes = sipCpp->hasEdge(*a0, *a1, a2);
    } else {
      sipIsErr = throwInvalidNodeException(sipCpp, *a1);
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  std::vector<tlp::edge> getEdges(const tlp::node node1, const tlp::node node2, bool directed=true) const;
%Docstring
tlp.Graph.getEdges(node1, node2, directed=True)

.. versionadded:: 4.2

Returns the edges found between two nodes.
If no edge is found, returns edges is empty.

:param node1: an existing node of the graph
:type node1: :class:`tlp.node`
:param node2: an existing node of the graph
:type node2: :class:`tlp.node`
:param directed: indicates if the direction of the edge (from source to target) must be taken in to account
:type directed: boolean
:rtype: boolean
:throws: an exception if one of the nodes does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    if (sipCpp->isElement(*a1)) {
      sipRes = new std::vector<tlp::edge>(sipCpp->getEdges(*a0, *a1, a2));
    } else {
      sipIsErr = throwInvalidNodeException(sipCpp, *a1);
    }
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::BooleanProperty *getBooleanProperty(const std::string &name);
%Docstring
tlp.Graph.getBooleanProperty(name)

Returns the boolean property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the boolean property to return or to create
:type name: string
:rtype: :class:`tlp.BooleanProperty`
%End

%MethodCode
  if (canGetProperty<tlp::BooleanProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::BooleanProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::BooleanProperty *getLocalBooleanProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalBooleanProperty(name)

Returns the boolean property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the boolean property to return or to create
:type name: string
:rtype: :class:`tlp.BooleanProperty`
%End

%MethodCode
  if (canGetProperty<tlp::BooleanProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::BooleanProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::BooleanVectorProperty *getBooleanVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getBooleanVectorProperty(name)

Returns the boolean vector property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the boolean vector property to return or to create
:type name: string
:rtype: :class:`tlp.BooleanVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::BooleanVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::BooleanVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::BooleanVectorProperty *getLocalBooleanVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalBooleanVectorProperty(name)

Returns the boolean vector property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the boolean vector property to return or to create
:type name: string
:rtype: :class:`tlp.BooleanVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::BooleanVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::BooleanVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::LayoutProperty *getLayoutProperty(const std::string &name);
%Docstring
tlp.Graph.getLayoutProperty(name)

Returns the layout property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the layout property to return or to create
:type name: string
:rtype: :class:`tlp.LayoutProperty`
%End

%MethodCode
  if (canGetProperty<tlp::LayoutProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::LayoutProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::LayoutProperty *getLocalLayoutProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalLayoutProperty(name)

Returns the layout property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the layout property to return or to create
:type name: string
:rtype: :class:`tlp.LayoutProperty`
%End

%MethodCode
  if (canGetProperty<tlp::LayoutProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::LayoutProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::CoordVectorProperty *getCoordVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getCoordVectorProperty(name)

Returns the coord vector property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the coord vector property to return or to create
:type name: string
:rtype: :class:`tlp.CoordVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::CoordVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::CoordVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::CoordVectorProperty *getLocalCoordVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalCoordVectorProperty(name)

Returns the coord vector property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the coord vector property to return or to create
:type name: string
:rtype: :class:`tlp.CoordVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::CoordVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::CoordVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::ColorProperty *getColorProperty(const std::string &name);
%Docstring
tlp.Graph.getColorProperty(name)

Returns the color property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the color property to return or to create
:type name: string
:rtype: :class:`tlp.ColorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::ColorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::ColorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::ColorProperty *getLocalColorProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalColorProperty(name)

Returns the color property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the color property to return or to create
:type name: string
:rtype: :class:`tlp.ColorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::ColorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::ColorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::ColorVectorProperty *getColorVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getColorVectorProperty(name)

Returns the color vector property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the color vector property to return or to create
:type name: string
:rtype: :class:`tlp.ColorVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::ColorVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::ColorVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::ColorVectorProperty *getLocalColorVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalColorVectorProperty(name)

Returns the color vector property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the color vector property to return or to create
:type name: string
:rtype: :class:`tlp.ColorVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::ColorVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::ColorVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::DoubleProperty *getDoubleProperty(const std::string &name);
%Docstring
tlp.Graph.getDoubleProperty(name)

Returns the double property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the double property to return or to create
:type name: string
:rtype: :class:`tlp.DoubleProperty`
%End

%MethodCode
  if (canGetProperty<tlp::DoubleProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::DoubleProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::DoubleProperty *getLocalDoubleProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalDoubleProperty(name)

Returns the double property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the double property to return or to create
:type name: string
:rtype: :class:`tlp.DoubleProperty`
%End

%MethodCode
  if (canGetProperty<tlp::DoubleProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::DoubleProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
}
%End

//===========================================================================================

  tlp::DoubleVectorProperty *getDoubleVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getDoubleVectorProperty(name)

Returns the double vector property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the double vector property to return or to create
:type name: string
:rtype: :class:`tlp.DoubleVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::DoubleVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::DoubleVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::DoubleVectorProperty *getLocalDoubleVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalDoubleVectorProperty(name)

Returns the double vector property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the double vector property to return or to create
:type name: string
:rtype: :class:`tlp.DoubleVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::DoubleVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::DoubleVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::StringProperty *getStringProperty(const std::string &name);
%Docstring
tlp.Graph.getStringProperty(name)

Returns the string property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the string property to return or to create
:type name: string
:rtype: :class:`tlp.StringProperty`
%End

%MethodCode
  if (canGetProperty<tlp::StringProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::StringProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::StringProperty *getLocalStringProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalStringProperty(name)

Returns the string property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the string property to return or to create
:type name: string
:rtype: :class:`tlp.StringProperty`
%End

%MethodCode
  if (canGetProperty<tlp::StringProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::StringProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::StringVectorProperty *getStringVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getStringVectorProperty(name)

Returns the string vector property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the string vector property to return or to create
:type name: string
:rtype: :class:`tlp.StringVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::StringVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::StringVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::StringVectorProperty *getLocalStringVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalStringVectorProperty(name)

Returns the string vector property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the string vector property to return or to create
:type name: string
:rtype: :class:`tlp.StringVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::StringVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::StringVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::SizeProperty *getSizeProperty(const std::string &name);
%Docstring
tlp.Graph.getSizeProperty(name)

Returns the size property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the size property to return or to create
:type name: string
:rtype: :class:`tlp.SizeProperty`
%End

%MethodCode
  if (canGetProperty<tlp::SizeProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::SizeProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::SizeProperty *getLocalSizeProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalSizeProperty(name)

Returns the size property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the size property to return or to create
:type name: string
:rtype: :class:`tlp.SizeProperty`
%End

%MethodCode
  if (canGetProperty<tlp::SizeProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::SizeProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::SizeVectorProperty *getSizeVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getSizeVectorProperty(name)

Returns the size vector property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the size vector property to return or to create
:type name: string
:rtype: :class:`tlp.SizeVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::SizeVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::SizeVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::SizeVectorProperty *getLocalSizeVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalSizeVectorProperty(name)

Returns the size vector property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the size vector property to return or to create
:type name: string
:rtype: :class:`tlp.SizeVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::SizeVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::SizeVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::IntegerProperty *getIntegerProperty(const std::string &name);
%Docstring
tlp.Graph.getIntegerProperty(name)

Returns the integer property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the integer property to return or to create
:type name: string
:rtype: :class:`tlp.IntegerProperty`
%End

%MethodCode
  if (canGetProperty<tlp::IntegerProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::IntegerProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::IntegerProperty *getLocalIntegerProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalIntegerProperty(name)

Returns the integer property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the integer property to return or to create
:type name: string
:rtype: :class:`tlp.IntegerProperty`
%End

%MethodCode
  if (canGetProperty<tlp::IntegerProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::IntegerProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::IntegerVectorProperty *getIntegerVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getIntegerVectorProperty(name)

Returns the integer vector property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the integer vector property to return or to create
:type name: string
:rtype: :class:`tlp.IntegerVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::IntegerVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::IntegerVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::IntegerVectorProperty *getLocalIntegerVectorProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalIntegerVectorProperty(name)

Returns the integer vector property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the integer vector property to return or to create
:type name: string
:rtype: :class:`tlp.IntegerVectorProperty`
%End

%MethodCode
  if (canGetProperty<tlp::IntegerVectorProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::IntegerVectorProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::GraphProperty *getGraphProperty(const std::string &name);
%Docstring
tlp.Graph.getGraphProperty(name)

Returns the meta-graph property associated to name in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the meta-graph property to return or to create
:type name: string
:rtype: :class:`tlp.GraphProperty`
%End

%MethodCode
  if (canGetProperty<tlp::GraphProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getProperty<tlp::GraphProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::GraphProperty *getLocalGraphProperty(const std::string &name);
%Docstring
tlp.Graph.getLocalGraphProperty(name)

Returns the meta-graph property associated to name in the graph properties pool.
If the property is not registered in the pool, it creates a new one and returns it. 
Using of :const:`del` on that property will cause a segmentation violation (use :meth:`tlp.Graph.delLocalProperty` instead). 

:param name: the name of the meta-graph property to return or to create
:type name: string
:rtype: :class:`tlp.GraphProperty`
%End

%MethodCode
  if (canGetProperty<tlp::GraphProperty>(sipCpp, *a0)) {
    sipRes = sipCpp->getLocalProperty<tlp::GraphProperty>(*a0);
  } else {
    sipIsErr = throwPropertyNameExistsException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::PropertyInterface* getProperty(const std::string& name);
%Docstring
tlp.Graph.getProperty(name)

Returns the property associated to name in the graph properties pool.
The returned property is referenced by its base class :class:`tlp.PropertyInterface` meaning getting and setting values
can only be done via the use of strings. To get the property correctly typed, use the methods described above. 
If the property does not exist it returns :const:`None`. 

:param name: the name of the property to return
:type name: string
:rtype: :class:`tlp.PropertyInterface`
%End
 

//===========================================================================================

  bool existProperty(const std::string& name);
%Docstring
tlp.Graph.existProperty(name)

Returns :const:`True` if a property of that name exists in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.

:param name: the name of the property
:type name: string
:rtype: boolean
%End

//===========================================================================================

  bool existLocalProperty(const std::string& name);
%Docstring
tlp.Graph.existLocalProperty(name)

Returns :const:`True` if a property of that name exists in the graph properties pool.

:param name: the name of the property
:type name: string
:rtype: boolean
%End

//===========================================================================================

  void delLocalProperty(const std::string& name);
%Docstring
tlp.Graph.delLocalProperty(name)

Removes and deletes the property associated to name in the graph properties pool.

:param name: the name of the property to delete
:type name: string
%End

%MethodCode
  if (sipCpp->existLocalProperty(*a0)) {
    tlp::PropertyInterface *prop = sipCpp->getProperty(*a0);
    releasePropertyWrapper(prop);
    sipCpp->delLocalProperty(*a0);
  } else {
    sipIsErr = -1;
    std::string msg = "No local graph property named  ";
    msg += *a0;
    msg += ".";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
%End

//===========================================================================================

  tlp::Iterator<std::string>* getLocalProperties() /TransferBack, TypeHint="Iterator[str]"/;
%Docstring
tlp.Graph.getLocalProperties()

Returns an iterator on the names of the properties local to the graph.

:rtype: a Tulip iterator on string objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<std::string>(sipCpp->getLocalProperties());
%End

//===========================================================================================

  tlp::Iterator<std::string>* getInheritedProperties() /TransferBack, TypeHint="Iterator[str]"/;
%Docstring
tlp.Graph.getInheritedProperties()

Returns an iterator on the names of the properties inherited from the graph's ancestors.

:rtype: a Tulip iterator on string objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<std::string>(sipCpp->getInheritedProperties());
%End

//===========================================================================================

  tlp::Iterator<std::string>* getProperties() /TransferBack, TypeHint="Iterator[str]"/;
%Docstring
tlp.Graph.getInheritedProperties()

Returns an iterator on the names of all the properties attached to the graph.

:rtype: a Tulip iterator on string objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<std::string>(sipCpp->getProperties());
%End

//===========================================================================================

  tlp::IteratorProperty* getObjectProperties() /TransferBack, TypeHint="Iterator[tlp.PropertyInterface]"/;
%Docstring
tlp.Graph.getObjectProperties()

.. versionadded:: 4.8

Returns an iterator over the properties attached to the graph,
whether they are local or inherited.

:rtype: a Tulip iterator on :class:`tlp.PropertyInterface` objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<tlp::PropertyInterface*>(sipCpp->getObjectProperties());
%End

//===========================================================================================

  tlp::IteratorProperty* getLocalObjectProperties() /TransferBack, TypeHint="Iterator[tlp.PropertyInterface]"/;
%Docstring
tlp.Graph.getLocalObjectProperties()

.. versionadded:: 4.8

Returns an iterator over the local properties attached to the graph.

:rtype: a Tulip iterator on :class:`tlp.PropertyInterface` objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<tlp::PropertyInterface*>(sipCpp->getLocalObjectProperties());
%End

//===========================================================================================

  tlp::IteratorProperty* getInheritedObjectProperties() /TransferBack, TypeHint="Iterator[tlp.PropertyInterface]"/;
%Docstring
tlp.Graph.getInheritedObjectProperties()

.. versionadded:: 4.8

Returns an iterator over the inherited properties attached to the graph.

:rtype: a Tulip iterator on :class:`tlp.PropertyInterface` objects
%End

%MethodCode
  sipRes = new tlp::StableIterator<tlp::PropertyInterface*>(sipCpp->getInheritedObjectProperties());
%End

//===========================================================================================

  bool applyIntegerAlgorithm(const std::string &algoName, tlp::IntegerProperty* result, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyIntegerAlgorithm(algoName, result, params=None)

.. versionadded:: 3.8

Computes an integer property on the current graph 
using an external named integer algorithm (plugin).
Integer algorithm plugins are objects
implementing the tlp::IntegerAlgorithm interface in C++ or 
the :class:`tlp.IntegerAlgorithm` interface in Python.
The list of currently loaded integer algorithm plugins can be
retrieved through the :func:`tlp.getIntegerAlgorithmPluginsList` function.
The computed values will be stored in result. 

Parameters can be transmit to the algorithm using a Python dictionnary
filled with parameters values where keys are of type string (parameters names) .
In some cases, algorithms
can also use this dictionnary in order to return some external information
(not stored in result).
To determine a plugin's parameters, you can either:
    
  * refer to its documentation
  * call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the integer algorithm to call
:type algoName: string
:param result: an integer property in which result of the algorithm will be stored
:type result: :class:`tlp.IntegerProperty`
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: (boolean, string)
:throws: an exception if the requested integer algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::IntegerAlgorithm>(sipCpp, *a0, a1, a3, a3Wrapper, *a2, sipIsErr, "integer");
%End

//===========================================================================================

  bool applyDoubleAlgorithm(const std::string &algoName, tlp::DoubleProperty* result, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyDoubleAlgorithm(algoName, result, params=None)

.. versionadded:: 3.8

Computes a double property on the current graph 
using an external named double algorithm (plugin).
Double algorithm plugins are objects
implementing the tlp::DoubleAlgorithm interface in C++ or 
the :class:`tlp.DoubleAlgorithm` interface in Python.
The list of currently loaded double algorithm plugins can be
retrieved through the :func:`tlp.getDoubleAlgorithmPluginsList` function.
The computed values will be stored in result. 

Parameters can be transmit to the algorithm using a Python dictionnary
filled with parameters values where keys are of type string (parameters names) .
In some cases, algorithms
can also use this dictionnary in order to return some external information
(not stored in result).
To determine a plugin's parameters, you can either:
    
  * refer to its :ref:`documentation <doublepluginsdoc>`
  * call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the double algorithm to call
:type algoName: string
:param result: a double property in which result of the algorithm will be stored
:type result: :class:`tlp.DoubleProperty`
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: (boolean, string)
:throws: an exception if the requested double algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::DoubleAlgorithm>(sipCpp, *a0, a1, a3, a3Wrapper, *a2, sipIsErr, "double");
%End

//===========================================================================================

  bool applyDoubleAlgorithm(const std::string &algoName, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyDoubleAlgorithm(algoName, params=None)

.. versionadded:: 4.8

Convenient method that computes a double property on the current graph and stores the result
in the default double property "viewMetric".
This is equivalent to call::

  graph.applyDoubleAlgorithm(algoName, graph.getDoubleProperty("viewMetric"), params)

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::DoubleAlgorithm>(sipCpp, *a0, sipCpp->getProperty<tlp::DoubleProperty>("viewMetric"), a2, a2Wrapper, *a1, sipIsErr, "double");
%End

//===========================================================================================

  bool applyLayoutAlgorithm(const std::string &algoName, tlp::LayoutProperty* result, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyLayoutAlgorithm(algoName, result, params=None)

.. versionadded:: 3.8

Computes a layout property on the current graph 
using an external named layout algorithm (plugin).
Layout algorithm plugins are objects
implementing the tlp::LayoutAlgorithm interface in C++ or 
the :class:`tlp.LayoutAlgorithm` interface in Python.
The list of currently loaded layout algorithm plugins can be
retrieved through the :func:`tlp.getLayoutAlgorithmPluginsList` function.
The computed values will be stored in result. 

Parameters can be transmit to the algorithm using a Python dictionnary
filled with parameters values where keys are of type string (parameters names) .
In some cases, algorithms
can also use this dictionnary in order to return some external information
(not stored in result).
To determine a plugin's parameters, you can either:
    
  * refer to its :ref:`documentation <layoutpluginsdoc>`
  * call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the layout algorithm to call
:type algoName: string
:param result: a layout property in which result of the algorithm will be stored
:type result: :class:`tlp.LayoutProperty`
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: (boolean, string)
:throws: an exception if the requested layout algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::LayoutAlgorithm>(sipCpp, *a0, a1, a3, a3Wrapper, *a2, sipIsErr, "layout");
%End

//===========================================================================================

  bool applyLayoutAlgorithm(const std::string &algoName, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyLayoutAlgorithm(algoName, params=None)

.. versionadded:: 4.8

Convenient method that computes a layout property on the current graph and stores the result
in the default layout property "viewLayout".
This is equivalent to call::

  graph.applyLayoutAlgorithm(algoName, graph.getLayoutProperty("viewLayout"), params)

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::LayoutAlgorithm>(sipCpp, *a0, sipCpp->getProperty<tlp::LayoutProperty>("viewLayout"), a2, a2Wrapper, *a1, sipIsErr, "layout");
%End

//===========================================================================================

  bool applyBooleanAlgorithm(const std::string &algoName, tlp::BooleanProperty* result, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyBooleanAlgorithm(algoName, result, params=None)

.. versionadded:: 3.8

Computes a boolean property on the current graph 
using an external named boolean algorithm (plugin).
Boolean algorithm plugins are objects
implementing the tlp::BooleanAlgorithm interface in C++ or 
the :class:`tlp.BooleanAlgorithm` interface in Python.
The list of currently loaded boolean algorithm plugins can be
retrieved through the :func:`tlp.getBooleanAlgorithmPluginsList` function.
The computed values will be stored in result. 

Parameters can be transmit to the algorithm using a Python dictionnary
filled with parameters values where keys are of type string (parameters names) .
In some cases, algorithms
can also use this dictionnary in order to return some external information
(not stored in result).
To determine a plugin's parameters, you can either:
    
  * refer to its :ref:`documentation <booleanpluginsdoc>`
  * call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the boolean algorithm to call
:type algoName: string
:param result: a boolean property in which result of the algorithm will be stored
:type result: :class:`tlp.BooleanProperty`
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: (boolean, string)
:throws: an exception if the requested boolean algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::BooleanAlgorithm>(sipCpp, *a0, a1, a3, a3Wrapper, *a2, sipIsErr, "boolean");
%End

//===========================================================================================

  bool applyBooleanAlgorithm(const std::string &algoName, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyBooleanAlgorithm(algoName, params=None)

.. versionadded:: 4.8

Convenient method that computes a boolean property on the current graph and stores the result
in the default boolean property "viewSelection".
This is equivalent to call::

  graph.applyBooleanAlgorithm(algoName, graph.getBooleanProperty("viewSelection"), params)

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::BooleanAlgorithm>(sipCpp, *a0, sipCpp->getProperty<tlp::BooleanProperty>("viewSelection"), a2, a2Wrapper, *a1, sipIsErr, "boolean");
%End

//===========================================================================================

  bool applySizeAlgorithm(const std::string &algoName, tlp::SizeProperty* result, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applySizeAlgorithm(algoName, result, params=None)

.. versionadded:: 3.8

Computes a size property on the current graph 
using an external named size algorithm (plugin).
Size algorithm plugins are objects
implementing the tlp::SizeAlgorithm interface in C++ or 
the :class:`tlp.SizeAlgorithm` interface in Python.
The list of currently loaded size algorithm plugins can be
retrieved through the :func:`tlp.getSizeAlgorithmPluginsList` function.
The computed values will be stored in result. 

Parameters can be transmit to the algorithm using a Python dictionnary
filled with parameters values where keys are of type string (parameters names) .
In some cases, algorithms
can also use this dictionnary in order to return some external information
(not stored in result).
To determine a plugin's parameters, you can either:
    
  * refer to its :ref:`documentation <sizepluginsdoc>`
  * call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the size algorithm to call
:type algoName: string
:param result: a size property in which result of the algorithm will be stored
:type result: :class:`tlp.SizeProperty`
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: (boolean, string)
:throws: an exception if the requested size algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::SizeAlgorithm>(sipCpp, *a0, a1, a3, a3Wrapper, *a2, sipIsErr, "size");
%End

//===========================================================================================

  bool applySizeAlgorithm(const std::string &algoName, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applySizeAlgorithm(algoName, params=None)

.. versionadded:: 4.8

Convenient method that computes a size property on the current graph and stores the result
in the default size property "viewSize".
This is equivalent to call::

  graph.applySizeAlgorithm(algoName, graph.getSizeProperty("viewSize"), params)

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::SizeAlgorithm>(sipCpp, *a0, sipCpp->getProperty<tlp::SizeProperty>("viewSize"), a2, a2Wrapper, *a1, sipIsErr, "size");
%End

//===========================================================================================

  bool applyColorAlgorithm(const std::string &algoName, tlp::ColorProperty* result, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyColorAlgorithm(algoName, result, params=None)

.. versionadded:: 3.8

Computes a color property on the current graph 
using an external named color algorithm (plugin).
Color algorithm plugins are objects
implementing the tlp::ColorAlgorithm interface in C++ or 
the :class:`tlp.ColorAlgorithm` interface in Python.
The list of currently loaded color algorithm plugins can be
retrieved through the :func:`tlp.getColorAlgorithmPluginsList` function.
The computed values will be stored in result. 

Parameters can be transmit to the algorithm using a Python dictionnary
filled with parameters values where keys are of type string (parameters names) .
In some cases, algorithms
can also use this dictionnary in order to return some external information
(not stored in result).
To determine a plugin's parameters, you can either:
    
  * refer to its :ref:`documentation <colorpluginsdoc>`
  * call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the color algorithm to call
:type algoName: string
:param result: a color property in which result of the algorithm will be stored
:type result: :class:`tlp.ColorProperty`
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: (boolean, string)
:throws: an exception if the requested color algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::ColorAlgorithm>(sipCpp, *a0, a1, a3, a3Wrapper, *a2, sipIsErr, "color");
%End

//===========================================================================================

  bool applyColorAlgorithm(const std::string &algoName, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyColorAlgorithm(algoName, params=None)

.. versionadded:: 4.8

Convenient method that computes a color property on the current graph and stores the result
in the default color property "viewColor".
This is equivalent to call::

  graph.applyColorAlgorithm(algoName, graph.getColorProperty("viewColor"), params)

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::ColorAlgorithm>(sipCpp, *a0, sipCpp->getProperty<tlp::ColorProperty>("viewColor"), a2, a2Wrapper, *a1, sipIsErr, "color");
%End

//===========================================================================================

  bool applyStringAlgorithm(const std::string &algoName, tlp::StringProperty* result, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyStringAlgorithm(algoName, result, params=None)

.. versionadded:: 3.8

Computes a string property on the current graph 
using an external named string algorithm (plugin).
String algorithm plugins are objects
implementing the tlp::StringAlgorithm interface in C++ or 
the :class:`tlp.StringAlgorithm` interface in Python.
The list of currently loaded string algorithm plugins can be
retrieved through the :func:`tlp.getStringAlgorithmPluginsList` function.
The computed values will be stored in result. 
Parameters can be transmit to the algorithm using a Python dictionnary
filled with parameters values where keys are of type string (parameters names) .
In some cases, algorithms
can also use this dictionnary in order to return some external information
(not stored in result).
To determine a plugin's parameters, you can either:
    
  * refer to its :ref:`documentation <stringpluginsdoc>`
  * call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the string algorithm to call
:type algoName: string
:param result: a string property in which result of the algorithm will be stored
:type result: :class:`tlp.StringProperty`
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: (boolean, string)
:throws: an exception if the requested string algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::StringAlgorithm>(sipCpp, *a0, a1, a3, a3Wrapper, *a2, sipIsErr, "string");
%End

//===========================================================================================

  bool applyStringAlgorithm(const std::string &algoName, std::string &msg /Out/, tlp::DataSet *data /GetWrapper/ =0) /TypeHint="Tuple[bool, str]"/;
%Docstring
tlp.Graph.applyStringAlgorithm(algoName, params=None)

.. versionadded:: 4.8

Convenient method that computes a string property on the current graph and stores the result
in the default string property "viewLabel".
This is equivalent to call::

  graph.applyStringAlgorithm(algoName, graph.getStringProperty("viewLabel"), params)

%End

%MethodCode
  sipRes = callGraphPropertyAlgorithm<tlp::StringAlgorithm>(sipCpp, *a0, sipCpp->getProperty<tlp::StringProperty>("viewLabel"), a2, a2Wrapper, *a1, sipIsErr, "string");
%End

 //===========================================================================================
  void push(bool unpopAllowed = true, std::vector<tlp::PropertyInterface*>* propertiesToPreserveOnPop= NULL);
%Docstring
tlp.Graph.push(unpopAllowed=True)

Marks the state of the current root graph in the hierarchy.
The next updates will be recorded in order to be undone at the
next call of the :meth:`tlp.Graph.pop` method. Be careful that all 
the updates are undone except those who affect the ordering of edges.

:param unpopAllowed: If set to :const:`False`, the next updates could not be replayed after undone. If some previously undonevupdates exist they could no longer be replayed.
:type unpopAllowed: boolean
%End

//===========================================================================================

  void pop(bool unpopAllowed = true);
%Docstring
tlp.Graph.pop(unpopAllowed=True)

Restores a previously marked state of the current root graph
in the hierarchy. The restored state does not remain marked.

:param unpopAllowed: If set to :const:`False`, the undone updates could not be replayed.
:type unpopAllowed: boolean
%End

//===========================================================================================

  void unpop();
%Docstring
tlp.Graph.unpop()

Marks again the current state of the root graph hierarchy
and replays the last updates previously undone.
%End

//===========================================================================================

  bool canPop();
%Docstring
tlp.Graph.canPop()

Returns :const:`True` if a previously marked state can be restored.

:rtype: boolean
%End

//===========================================================================================

  bool canUnpop();
%Docstring
tlp.Graph.canUnpop()

Returns :const:`True` if some previously undone updates can be replayed.

:rtype: boolean
%End

//===========================================================================================

  bool canPopThenUnpop();
%Docstring
tlp.Graph.canUnpop()

Returns :const:`True` if the current state updates can be undone then replayed.

:rtype: boolean
%End

//===========================================================================================

  void popIfNoUpdates();
%Docstring
tlp.Graph.popIfNoUpdates()

.. versionadded:: 5.0

Remove last pushed state if no updates have been recorded.

:rtype: boolean
%End

//===========================================================================================

  tlp::node createMetaNode(const std::set<tlp::node> &nodeSet, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(nodeSet, multiEdges=True, delAllEdge=True)

.. deprecated:: 5.0

Closes a set of existing nodes into a metanode and returns it.
Edges from nodes in the set to other nodes are replaced with
edges from the metanode to the other nodes.

:param nodeSet: a set of existing nodes
:type nodeSet: set of :class:`tlp.node`
:param multiEdges: indicates if a meta edge will be created for each underlying edge
:type multiEdges: boolean
:param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
:type delAllEdge: boolean
:rtype: :class:`tlp.node`
:throws: an exception if one of the provided nodes does not belong to the graph.

 .. warning:: This method will fail when called on the root graph.

%End

%MethodCode
  std::set<tlp::node>::const_iterator it = a0->begin();
  for (; it != a0->end() ; ++it) {
    if (!sipCpp->isElement(*it)) {
      sipIsErr = throwInvalidNodeException(sipCpp, *it);
      break;
    }
  }
  if (sipIsErr != -1) {
    sipRes = new tlp::node(sipCpp->createMetaNode(*a0, a1, a2));
  }
%End

//===========================================================================================

  tlp::node createMetaNode(const std::vector<tlp::node> &nodes, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(nodes, multiEdges=True, delAllEdge=True)

.. versionadded:: 5.0

Closes a set of existing nodes into a metanode and returns it.
Edges from nodes in the set to other nodes are replaced with
edges from the metanode to the other nodes.

:param nodes: a list of existing nodes
:type nodes: list of :class:`tlp.node`
:param multiEdges: indicates if a meta edge will be created for each underlying edge
:type multiEdges: boolean
:param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
:type delAllEdge: boolean
:rtype: :class:`tlp.node`
:throws: an exception if one of the provided nodes does not belong to the graph.

 .. warning:: This method will fail when called on the root graph.

%End

%MethodCode
 std::vector<tlp::node>::const_iterator it = a0->begin();
 for (; it != a0->end() ; ++it) {
   if (!sipCpp->isElement(*it)) {
     sipIsErr = throwInvalidNodeException(sipCpp, *it);
     break;
   }
 }
 if (sipIsErr != -1) {
   sipRes = new tlp::node(sipCpp->createMetaNode(*a0, a1, a2));
 }
%End

//===========================================================================================

  tlp::node createMetaNode(tlp::Graph* subgraph, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(subgraph, multiEdges=True, delAllEdge=True)

Closes a sub-graph into a metanode and returns it. Edges from nodes
in the sub-graph to nodes outside the sub-graph are replaced with
edges from the metanode to the nodes outside the sub-graph.

The provided sub-graph can not be a descendant of the current graph in the graph hierarchy.
When creating a metanode, all nodes that are closed in it are first deleted
from the graph. These nodes are also deleted in the sub-graphs of the graph and
in all descendant sub-graphs. So if the provided sub-graph is a descendant of
the graph, all its nodes and edges will be deleted and the meta-node will
point to an empty sub-graph.

:param subgraph: an existing sub-graph
:type subgraph: :class:`tlp.Graph`
:param multiEdges: indicates if a meta edge will be created for each underlying edge
:type multiEdges: boolean
:param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
:type delAllEdge: boolean
:rtype: :class:`tlp.node`
:throws: an exception if the provided sub-graph is a descandent of the graph in the graph hierarchy or if one of the element in the sub-graph does not belong to the graph.

.. warning:: This method will fail when called on the root graph.

%End

%MethodCode
  if (sipCpp->isDescendantGraph(a0)) {
    sipIsErr = -1;
    std::ostringstream oss;
    oss << "Can not create a metanode from subgraph ";
    oss << "\"" << a0->getName() << "\" (id " << a0->getId() << ")";
    oss << " because it is a descendant of graph ";
    oss << "\"" << sipCpp->getName() << "\" (id " << sipCpp->getId() << ")";
    PyErr_SetString(PyExc_Exception, oss.str().c_str());
  } else {
    tlp::node n;
    forEach(n, a0->getNodes()) {
      if (!sipCpp->isElement(n)) {
        sipIsErr = throwInvalidNodeException(sipCpp, n);
        break;
      }
    }
    if (sipIsErr != -1) {
      tlp::edge e;
      forEach(e, a0->getEdges()) {
        if (!sipCpp->isElement(e)) {
          sipIsErr = throwInvalidEdgeException(sipCpp, e);
          break;
        }
      }
    }
    if (sipIsErr != -1) {
      sipRes = new tlp::node(sipCpp->createMetaNode(a0, a1, a2));
    }
  }
%End

//===========================================================================================;

  void createMetaNodes(tlp::IteratorGraph *itGraph /TypeHint="Iterator[tlp.Graph]"/, tlp::Graph *quotientGraph, std::vector<tlp::node>& metaNodes /Out/);
%Docstring
tlp.Graph.createMetaNodes(itGraph, quotientGraph)

Populates a quotient graph with one meta node for each iterated graph.
Returns a list of created meta-nodes.

:param itGraph: a graph iterator, (typically a sub-graph iterator)
:type itGraph: a Tulip iterator on :class:`tlp.Graph` objects
:param quotientGraph: the graph that will contain the meta nodes
:type quotientGraph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.node`
%End

//===========================================================================================

  void openMetaNode(tlp::node metaNode, bool updateProperties = true);
%Docstring
tlp.Graph.openMetaNode(metaNode, updateProperties=True)

Opens a metanode and replaces all edges between that
meta node and other nodes in the graph.

:param metaNode: the meta-node to open
:type metaNode: :class:`tlp.node`
:param updateProperties: If :const:`True`, open meta node will update inner nodes layout, color, size, etc...
:type updateProperties: boolean

.. warning:: This method will fail when called on the root graph.

%End

//===========================================================================================

  SIP_PYOBJECT __getitem__(const std::string &attributeName) const;
%MethodCode
  if (sipCpp->existProperty(*a0)) {
    sipRes = sipConvertFromType(sipCpp->getProperty(*a0), sipFindType("tlp::PropertyInterface"), NULL);
  } else {
    sipRes = sipConvertFromNewType(new tlp::PropertyProxy(sipCpp, *a0), sipFindType("tlp::PropertyProxy"), NULL);
  }
%End

//===========================================================================================

  std::string getName() const;
%Docstring
tlp.Graph.getName()

Returns the name of the graph.

:rtype: string
%End

//===========================================================================================

  void setName(const std::string &name);
%Docstring
tlp.Graph.setName(name)

Sets the name of the graph.

:param name: the new name of the graph
:type name: string
%End

//===========================================================================================

  void setAttribute(const std::string &name, SIP_PYOBJECT val);
%Docstring
tlp.Graph.setAttribute(name, val)

Sets an attribute of the graph.

:param name: the name of the attribute to set
:type name: string
:param val: the value of the attribute
:param type: object
%End

%MethodCode
  tlp::DataType* dataType = sipCpp->getAttribute(*a0);
  ValueSetter valSetter(sipCpp, *a0);
  if (!setCppValueFromPyObject(a1, valSetter, dataType)) {
    sipIsErr = -1;
    std::string msg = "Object of type ";
    msg += std::string(a1->ob_type->tp_name);
    msg += " can not be store as graph attribute.";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
  delete dataType;
%End 

//===========================================================================================

  SIP_PYOBJECT getAttribute(const std::string& name);
%Docstring
tlp.Graph.getAttribute(name)

Returns a reference on an attribute of the graph or :const:`None` if it does not exist.

:param name: the name of the attribute to return.
:type name: string
:rtype: object
:throws: an exception is the attribute does not exist
%End

%MethodCode
  sipRes = NULL;
  if (sipCpp->existAttribute(*a0)) {
    tlp::DataType* dataType = sipCpp->getAttribute(*a0);
    sipRes = getPyObjectFromDataType(dataType);
    delete dataType;
  }

  if (!sipRes) {
    std::ostringstream oss;
    oss << "Graph attribute named \"" << *a0 << "\" does not exist.";
    sipIsErr = -1;
    PyErr_SetString(PyExc_AttributeError, oss.str().c_str());
  }
%End  

//===========================================================================================

  void removeAttribute(const std::string& name);
%Docstring
tlp.Graph.removeAttribute(name)

.. versionadded:: 4.4

Removes an attribute of the graph.

:param name: the name of the attribute to return.
:type name: string
:rtype: object
:throws: an exception is the attribute does not exist
%End

%MethodCode
  if (sipCpp->existAttribute(*a0)) {
    sipCpp->removeAttribute(*a0);
  } else {
    std::ostringstream oss;
    oss << "Graph attribute named \"" << *a0 << "\" does not exist.";
    sipIsErr = -1;
    PyErr_SetString(PyExc_AttributeError, oss.str().c_str());
  }
%End

//===========================================================================================

  bool attributeExist(const std::string &name);
%Docstring
tlp.Graph.attributeExist(name)

.. versionadded:: 4.4

.. deprecated:: 4.10

Checks if an attribute with a specific name exists in the graph.
That method is now deprecated, use :meth:`tlp.Graph.existAttribute` instead.

:param name: the name of the attribute to test existence.
:type name: string
:rtype: boolean
%End

//===========================================================================================

  bool existAttribute(const std::string &name);
%Docstring
tlp.Graph.existAttribute(name)

.. versionadded:: 4.10

Checks if an attribute with a specific name exists in the graph.

:param name: the name of the attribute to test existence.
:type name: string
:rtype: boolean
%End

//===========================================================================================

  const tlp::DataSet & getAttributes() const;
%Docstring
tlp.Graph.getAttributes()

.. versionadded:: 4.4

Returns all the attributes of the graph.

:rtype: a dictionnary with string keys filled with graph attributes values
%End

//===========================================================================================

  tlp::DataSet getNodePropertiesValues(const tlp::node n);
%Docstring
tlp.Graph.getNodePropertiesValues(node)

.. versionadded:: 4.8.1

Returns a dictionnary containing the attached graph properties values for a given node.
The keys of that dictionnary are of string type and corresponds to the graph properties names
while the values are typed according to those of the properties (bool, int, float, str, :class:`tlp.Color`,
:class:`tlp.Coord`, :class:`tlp.Size`, list of those types).

:param node: a node of the graph
:type node: :class:`tlp.node`
:rtype: a dictionnary filled with the node properties values
:throws: an exception if the provided node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = getDataSetFilledWithPropertiesValues(sipCpp, a0->id, true);
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  tlp::DataSet getEdgePropertiesValues(const tlp::edge e);
%Docstring
tlp.Graph.getEdgePropertiesValues(edge)

.. versionadded:: 4.8.1

Returns a dictionnary containing the attached graph properties values for a given edge.
The keys of that dictionnary are of string type and corresponds to the graph properties names
while the values are typed according to those of the properties (bool, int, float, str, :class:`tlp.Color`,
:class:`tlp.Coord`, :class:`tlp.Size`, list of those types).

:param edge: an edge of the graph
:type edge: :class:`tlp.edge`
:rtype: a dictionnary filled with the edge properties values
 :throws: an exception if the provided node does not belong to the graph
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipRes = getDataSetFilledWithPropertiesValues(sipCpp, a0->id, false);
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  void setNodePropertiesValues(const tlp::node n, const tlp::DataSet &dataSet);
%Docstring
tlp.Graph.setNodePropertiesValues(node, propertiesValues)

.. versionadded:: 4.8.1

Sets the values of multiple graph properties for a given node through the use of a dictionnary.
The keys of that dictionnary must be of string type and correspond to graph properties names (possibly not created yet)
while the values must be typed according to those of the available Tulip properties (bool, int, float, str, :class:`tlp.Color`,
:class:`tlp.Coord`, :class:`tlp.Size`, list of those types).

:param node: a node of the graph
:type node: :class:`tlp.node`
:param propertiesValues: the properties values for the node
:type propertiesValues: a dictionnary
:throws: an exception if the provided node does not belong to the graph, if one of the string key of the dictionnary does not correspond to an existing property name of if there is a type error for a property value
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipIsErr = setPropertiesValuesFromDataSet(sipCpp, a1, a0->id, true) ? 0 : 1;
  } else {
    sipIsErr = throwInvalidNodeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  void setEdgePropertiesValues(const tlp::edge e, const tlp::DataSet &dataSet);
%Docstring
tlp.Graph.setEdgePropertiesValues(edge, propertiesValues)

.. versionadded:: 4.8.1

Sets the values of multiple graph properties for a given edge through the use of a dictionnary.
The keys of that dictionnary must be of string type and correspond to graph properties names (possibly not created yet)
while the values must be typed according to those of the available Tulip properties (bool, int, float, str, :class:`tlp.Color`,
:class:`tlp.Coord`, :class:`tlp.Size`, list of those types).

:param edge: an edge of the graph
:type edge: :class:`tlp.edge`
:param propertiesValues: the properties values for the edge
:type propertiesValues: a dictionnary
:throws: an exception if the provided edge does not belong to the graph, if one of the string key of the dictionnary does not correspond to an existing property name of if there is a type error for a property value
%End

%MethodCode
  if (sipCpp->isElement(*a0)) {
    sipIsErr = setPropertiesValuesFromDataSet(sipCpp, a1, a0->id, false) ? 0 : 1;
  } else {
    sipIsErr = throwInvalidEdgeException(sipCpp, *a0);
  }
%End

//===========================================================================================

  std::string __repr__() const;
%MethodCode
  std::string graphName;
  sipCpp->getAttribute("name", graphName);
  std::ostringstream oss;
  oss << "<graph \"" << graphName << "\" (id " << sipCpp->getId() << ") >";
  sipRes = new std::string(oss.str());
%End

//===========================================================================================

  long __hash__() const;
%MethodCode
  return static_cast<long>(sipCpp->getId());
%End

};

};

namespace tlp {

//===========================================================================================

tlp::Graph *newGraph() /TransferBack/;
%Docstring
tlp.newGraph()

Creates and returns a new empty graph.

:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

tlp::Graph *loadGraph(const std::string &filename, tlp::Graph *newGraph);
%Docstring
tlp.loadGraph(filename, graph=None)

Loads a graph in the TLP format from a file (extension can be .tlp, .tlp.gz, or .tlpz).
Returns the graph (a new graph if none is provided) or :const:`None` if the import fails.

:param filename: the path to the tlp file
:type filename: string
:param graph: if provided, import new graph elements in that already existing graph
:type newGraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Graph`
%End
%MethodCode
  tlp::DataSet dataSet;
  dataSet.set<std::string>("file::filename", *a0);
  sipRes = tlp::importGraph("TLP Import", dataSet, 0, a1);
%End

//===========================================================================================

tlp::Graph *loadGraph(const std::string &filename) /TransferBack/;
%Docstring
tlp.loadGraph(fileName)

Loads a graph serialized in a file trough one the available Tulip import plugins based on its extension.

Since Tulip 4.8, the selection of the import plugin is based on the provided filename extension.
The import will fail if the selected import plugin is not loaded.
The graph file formats that can currently be imported are : TLP (.tlp, .tlp.gz, .tlpz), TLP Binary (.tlpb, .tlpb.gz, .tlpbz), TLP JSON (.json),
Gephi (.gexf), Pajek (.net, .paj), GML (.gml), Graphviz (.dot) and UCINET (.txt).

Before Tulip 4.8 and as a fallback, this function uses the "TLP Import" import plugin
(always loaded as it is linked into the tulip-core library).

Returns a new graph or :const:`None` if the import fails.

:param filename: the path to the file in one of the supported formats to parse.
:type filename: string
:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

bool saveGraph(tlp::Graph *graph, const std::string &filename);
%Docstring
tlp.saveGraph(graph, filename)

Serializes the corresponding graph and all its sub-graphs (depending on the format) to a file
through the available Tulip export plugins.

Since Tulip 4.8, the selection of the export plugin is based on the provided filename extension.
The export will fail if the selected export plugin is not loaded.
The file formats the graph can be exported to are: TLP (.tlp, .tlp.gz, .tlpz), TLP Binary (.tlpb, .tlpb.gz, .tlpbz),
TLP JSON (.json), GML (.gml), CSV (.csv), SVG (.svg, .svgz)

Before Tulip 4.8 and as a fallback, this function uses the "TLP Export" export plugin
(always loaded as it is linked into the tulip-core library).

Returns whether the export was successfull or not.

:param graph: the graph to export
:type graph: :class:`tlp.Graph`
:param filename: the path to the destination file
:type filename: string
:rtype: boolean
%End

//===========================================================================================

bool saveGraph(tlp::Graph *graph);
%Docstring
tlp.saveGraph(graph)

.. versionadded:: 4.5

Saves a graph to the same file from which it has been loaded (if the ouput format is supported).
Returns :const:`True` if the graph has been successfully saved.

:param graph: the graph to export
:type graph: :class:`tlp.Graph`
:rtype: boolean
:throws: an exception if there is no file attached to the graph
%End

%MethodCode
  bool ok = false;
  std::string tlpFilePath;
  if (a0->existAttribute("file")) {
    a0->getAttribute("file", tlpFilePath);
    ok = true;
  }
  if (ok) {
    sipRes = tlp::saveGraph(a0, tlpFilePath);
  } else {
    sipIsErr = -1;
    std::string msg = "No file attached to the graph \"";
    msg += a0->getName();
    msg += "\".";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
%End

//===========================================================================================

tlp::Graph *importGraph(const std::string &importPluginName) /TransferBack/;
%MethodCode
  if (pluginExists<tlp::ImportModule>(*a0)) {
    tlp::DataSet *params = prepareAlgorithmParameters(*a0);
    sipRes = tlp::importGraph(*a0, *params, NULL);
  } else {
    sipIsErr = -1;
    std::string msg = "No Tulip import plugin named  ";
    msg += *a0;
    msg += ".";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
%End

tlp::Graph *importGraph(const std::string &importPluginName, tlp::DataSet dataSet /GetWrapper/ ) /TransferBack/;
%Docstring
tlp.importGraph(importPluginName, params={})

Imports a graph using a Tulip :ref:`import plugin <importpluginsdoc>`.
Returns a new graph or None if the import fails.

:param importPluginName: the name of the Tulip import plugin
:type importPluginName: string
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:param newGraph: if provided, add imported graph elements in that graph
:type newGraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Graph`
:throws: an exception if the requested import plugin is not registered in the plugins database. 
%End

%MethodCode
  if (pluginExists<tlp::ImportModule>(*a0)) {
    tlp::DataSet *params = prepareAlgorithmParameters(*a0, NULL, a1, a1Wrapper);
    if (params) {
      sipRes = tlp::importGraph(*a0, *params, NULL);
      updateWrappedDataSetAfterAlgorithmCall(params, a1Wrapper);
      delete params;
    } else {
      sipIsErr = -1;
    }
  } else {
    sipIsErr = -1;
    std::string msg = "No Tulip import plugin named  ";
    msg += *a0;
    msg += ".";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
%End

//===========================================================================================

tlp::Graph *importGraph(const std::string &importPluginName, tlp::DataSet &dataSet /GetWrapper/, tlp::Graph *newGraph=NULL);
%Docstring
tlp.importGraph(importPluginName, params, newGraph=None)

Imports a graph using a Tulip :ref:`import plugin <importpluginsdoc>`.
Import plugins are objects implementing the tlp::ImportModule interface
in C++ or the :class:`tlp.ImportModule` interface in Python.
The list of currently loaded import plugins can be
retrieved through the :func:`tlp.getImportPluginsList` function.
Returns a new graph or None if the import fails.

:param importPluginName: the name of the Tulip import plugin
:type importPluginName: string
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:param newGraph: if provided, add imported graph elements in that graph
:type newGraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Graph`
:throws: an exception if the requested import plugin is not registered in the plugins database.

%End

%MethodCode
  if (pluginExists<tlp::ImportModule>(*a0)) {
    tlp::DataSet *params = prepareAlgorithmParameters(*a0, a2, a1, a1Wrapper);
    if (params) {
      sipRes = tlp::importGraph(*a0, *params, 0, a2);
      updateWrappedDataSetAfterAlgorithmCall(params, a1Wrapper);
      delete params;
    } else {
      sipIsErr = 1;
    }
  } else {
    sipIsErr = -1;
    std::string msg = "No Tulip import plugin named  ";
    msg += *a0;
    msg += ".";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
%End

//===========================================================================================

bool exportGraph(const std::string &exportPluginName, tlp::Graph *graph, const std::string &outputFilePath, tlp::DataSet dataSet /GetWrapper/ = tlp::DataSet());
%Docstring
    tlp.exportGraph(exportPluginName, graph, outputFilePath, params={})

Exports a graph to a file using a Tulip :ref:`export plugin <exportpluginsdoc>`.

:param graph: the graph to export
:type graph: :class:`tlp.Graph`
:param outputFilePath: the path of the file the export plugin will write to
:type outputFilePath: string
:param exportPluginName: the name of the Tulip export plugin to execute
:type exportPluginName: string
:param params: The parameters to the algorithm.
:type params: a dictionnary with string keys (parameters names) and parameters values
:rtype: boolean
:throws: an exception if the requested export plugin is not registered in the plugins database. 

%End

%MethodCode
  if (pluginExists<tlp::ExportModule>(*a0)) {
    std::ostream *ofs = tlp::getOutputFileStream((*a2).c_str());
    if (ofs->good()) {
      tlp::DataSet *params = prepareAlgorithmParameters(*a0, a1, a3, a3Wrapper);
      if (params) {
        sipRes = tlp::exportGraph(a1, *ofs, *a0, *params);
        updateWrappedDataSetAfterAlgorithmCall(params, a3Wrapper);
        delete params;
      } else {
        sipIsErr = -1;
      }
    } else {
      std::string msg = "Export error : The specified file path (";
      msg += *a2;
      msg += ") is not valid.";  
      printErrorMessage(msg);
      sipRes = false;
    }
    delete ofs;
  } else {
    sipIsErr = -1;
    std::string msg = "No Tulip export plugin named  ";
    msg += *a0;
    msg += ".";
    PyErr_SetString(PyExc_Exception, msg.c_str());
  }
%End

//===========================================================================================

void copyToGraph(tlp::Graph *outGraph, tlp::Graph *inGraph, tlp::BooleanProperty* inSelection=0, tlp::BooleanProperty* outSelection=0);
%Docstring
tlp.copyToGraph(outGraph, inGraph, inSelection=None, outSelection=None)

Appends the selected part of a graph (properties, nodes and edges) into another one.
If no selection is done, the whole input graph graph is appended.
The output selection is used to select the appended nodes & edges

:param outGraph: the graph on which to append elements
:type outGraph: :class:`tlp.Graph`
:param inGraph: the graph to append
:type inGraph: :class:`tlp.Graph`
:param inSelection: boolean property attached to inGraph, only selected elements will be appended if provided
:type inSelection: :class:`tlp.BooleanProperty`
:param outSelection: boolean property attached to outGraph, appended nodes and edges will be selected if provided 
:type outSelection: :class:`tlp.BooleanProperty`

.. warning:: The input selection is extended to all selected edge ends.

%End

//===========================================================================================

void removeFromGraph(tlp::Graph *inGraph, tlp::BooleanProperty *inSelection = 0);
%Docstring
tlp.removeFromGraph(inGraph, inSelection=None)

Removes the selected part of a graph (properties values, nodes and edges).
If no selection is done, the whole graph is reseted to default value.

:param inGraph: the graph on which to remove elements
:type inGraph: :class:`tlp.Graph`
:param inSelection: boolean property attached to inGraph, only selected elements will be removed if provided
:type inSelection: :class:`tlp.BooleanProperty`

.. warning:: The selection is extended to all selected edge ends.

%End

//===========================================================================================
tlp::IteratorGraph *getRootGraphs() /TypeHint="Iterator[tlp.Graph]"/;
%Docstring
tlp.getRootGraphs()

Returns an iterator on all the currently existing root graphs.

:rtype: a Tulip iterator on :class:`tlp.Graph` objects
%End

};

