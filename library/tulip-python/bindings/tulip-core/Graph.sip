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
};
typedef std::pair<tlp::node, tlp::node> pairNodeNode;

extern void releaseGraphWrapper(tlp::Graph *graph);
extern void releaseGraphHierarchyWrappers(tlp::Graph *graph);
%End

%ModuleCode
void releaseGraphWrapper(tlp::Graph *graph) {
    releaseSIPWrapper(graph, sipFindType("tlp::Graph"));
}

void releaseGraphHierarchyWrappers(tlp::Graph *graph) {
    tlp::Graph *sg = NULL;
    forEach(sg, graph->getSubGraphs()) {
        releaseGraphHierarchyWrappers(sg);
    }
    releaseGraphWrapper(graph);
}
%End

typedef std::pair<tlp::node, tlp::node> pairNodeNode;

namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;

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
	
//===========================================================================================	
	
    bool applyAlgorithm(const std::string &algorithm, std::string &errorMessage /Out/, tlp::DataSet *dataSet=NULL);
%Docstring
    tlp.Graph.applyAlgorithm(algoName[, dataSet])

.. versionadded:: 3.7
	
Applies an algorithm plugin, identified by its name.
Algorithm plugins are objects implementing the tlp::Algorithm interface 
in C++ or the :class:`tlp.Algorithm` interface in Python.
Parameters can be transmit to the algorithm using a :class:`tlp.DataSet`.
To determine a plugin's parameters, you can either:

	* refer to its documentation
	* call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: The name of the algorithm to apply.
:type algoName: string
:param dataSet: The parameters to the algorithm. Defaults to :const:`None`.
:type dataSet: :class:`tlp.DataSet`
:rtype: (boolean, string)
%End  	

%MethodCode
    if (pluginExists<tlp::Algorithm>(*a0)) {
        sipRes = sipCpp->applyAlgorithm(*a0, *a1, a2);
    } else {
        sipIsErr = -1;
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

Remove all nodes, edges and subgraphs from the graph .
%End

//===========================================================================================

	tlp::Graph *addSubGraph(tlp::BooleanProperty *selection=0, const std::string& name = "unnamed");
%Docstring
tlp.Graph.addSubGraph([selection = None, name = "unnamed"])

Creates and returns a new sub-graph. The elements of the new sub-graph are those 
selected in the selection. If there is no selection an empty sub-graph is returned.

:param selection: a Boolean property whose selected elements will be added to the sub-graph
:type selection: :class:`tlp.BooleanProperty`
:param name: The name of the newly created subgraph. Defaults to "unnamed".
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

:param name: The name of the newly created subgraph.
:type name: string
:rtype: :class:`tlp.Graph`
%End	

//===========================================================================================

 	tlp::Graph* addCloneSubGraph(const std::string& name = "unnamed", bool addSibling = false);
%Docstring
tlp.Graph.addCloneSubGraph(name = "unnamed", addSibling = false)

.. versionadded:: 4.5

Creates and returns a subgraph of this graph (or the parent of this graph) that contains all its elements.

:param name: The name of the newly created subgraph. Defaults to "unnamed".
:type name: string
:param addSibling: if true a sibling of this graph will be created; a subgraph of this graph if not. Defaults to false
:type addSibling: bool
:rtype: :class:`tlp.Graph`
%End 	
 
//=========================================================================================== 

	tlp::Graph *inducedSubGraph(const std::set<tlp::node>& nodeSet);
%Docstring
tlp.Graph.inducedSubGraph(nodeSet)

Creates and returns a new sub-graph of the graph induced by a set of nodes. 
The sub-graph contains all the nodes of the set and all the existing edges 
between two nodes of the set including self-loops.

:param nodeSet: the set of nodes from which to build the induced sub-graph
:type nodeSet: list or set of :class:`tlp.node`
:rtype: :class:`tlp.Graph` 
%End

// For code compatibility with Tulip-Python 3.X
   tlp::Graph *inducedSubGraph(const std::vector<tlp::node>& nodeSet);
%MethodCode
    sipRes = sipCpp->inducedSubGraph(std::set<tlp::node>(a0->begin(), a0->end()));
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

	tlp::Graph* getSuperGraph() const ;
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
	
	tlp::IteratorGraph *getSubGraphs() const /TransferBack/;
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

Returns the descendant graph with the corresponding id or :const:`None` if there is no descendant with that id. 

:param id: a graph id
:type id: integer
:rtype: :class:`tlp.Graph` or :const:`None` 
%End

//===========================================================================================

    tlp::Graph* getDescendantGraph(const std::string &name) const;
%Docstring
tlp.Graph.getDescendantGraph(name)

.. versionadded:: 3.7

Returns the descendant with the corresponding name or :const:`None` if there is no descendant with that name.

:param name: the name of the descendant to return
:type name: string
:rtype: :class:`tlp.Graph` or :const:`None`
%End

//===========================================================================================
	
	tlp::Graph *getNthSubGraph(unsigned int n) const;
%Docstring
tlp.Graph.getNthSubGraph(n)

.. versionadded:: 3.7

Returns the nth subgraph or :const:`None` if there is no such sub-graph.
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
	
	tlp::IteratorGraph *getDescendantGraphs() const /TransferBack/;
%Docstring
tlp.Graph.getDescendantGraphs()

Returns an iterator over all the descendant graphs of the graph.

:rtype: a Tulip iterator on :class:`tlp.Graph` objects
%End
	
%MethodCode
	sipRes = new tlp::StableIterator<tlp::Graph *>(sipCpp->getDescendantGraphs());
%End		

//===========================================================================================

	tlp::node addNode();
%Docstring
tlp.Graph.addNode()

Adds a new node in the graph and returns it. This node is also added in all 
the graph's ancestors to maintain the sub-graph relation between graphs.

:rtype: :class:`tlp.node`
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

        void addNodes(tlp::Iterator<tlp::node>* nodes /Transfer/);
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
tlp.Graph.delNode(node[, deleteInAllGraphs = False])

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

        void delNodes(tlp::Iterator<tlp::node>* itN /Transfer/, bool deleteInAllGraphs = false);
%Docstring
tlp.Graph.delNodes(itNodes[, deleteInAllGraphs=False])

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
tlp.Graph.delNodes(nodes[, deleteInAllGraphs=False])

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

	tlp::edge addEdge(const tlp::node src, const tlp::node tgt);
%Docstring
tlp.Graph.addEdge(src, tgt)

Adds a new edge in the graph and returns it. This edge is also added in all 
the graph's ancestors to maintain the sub-graph relation between graphs.

:param src: the source node of the new edge
:param tgt: the target node of the new edge
:type src: :class:`tlp.node`
:type tgt: :class:`tlp.node`
:rtype: :class:`tlp.edge`
:throws: an exception if the provided source or target node does not belong to the graph
%End	
	
%MethodCode
	if (sipCpp->isElement(*a0)) {
		if (sipCpp->isElement(*a1)) {
			sipRes = new tlp::edge(sipCpp->addEdge(*a0, *a1));
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
:throws: an exception if of the provided nodes to link is not an element of the graph.
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

        void addEdges(tlp::Iterator<tlp::edge>* edges /Transfer/);
%Docstring
tlp.Graph.addEdges(itEdges)

.. versionadded:: 3.7

Adds existing edges in the graph. The edges are also added in all
the graph ancestors to maintain the sub-graph relation between graphs.

:param itEdges: an iterator on the edges to add
:type itEdges: a Tulip iterator on :class:`tlp.edge` objects
:throws: an exception if one of the edge to add is not an element of the root graph or if one of the edges extremities is not an element of the graph.

.. warning:: The added edges must be elements of the graph hierarchy, thus they must be elements of the root graph.

.. warning : One can't add existing edges to the root graph.
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
                oss << "Error : edge " << e.id << " can not be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
                    << "because the graph does not contain its source node.";
                printErrorMessage(oss.str());
                sipIsErr = throwInvalidNodeException(sipCpp, sipCpp->getRoot()->source(e));
                break;
            }
            if (!sipCpp->isElement(sipCpp->getRoot()->target(e))) {
                std::ostringstream oss;
                oss << "Error : edge " << e.id << " can not be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
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

        void addEdges(const std::vector<tlp::edge>& edges);
%Docstring
tlp.Graph.addEdges(edges)

.. versionadded:: 4.5

Adds existing edges in the graph. The edges are also added in all
the graph ancestors to maintain the sub-graph relation between graphs.

:param edges: a vector of edges to add
:type edges: a list of :class:`tlp.edge` objects
:throws: an exception if one of the edge to add is not an element of the root graph or if one of the edges extremities is not an element of the graph.

.. warning:: The added edges must be elements of the graph hierarchy, thus they must be elements of the root graph.

.. warning:: One can't add existing edges to the root graph.

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
                oss << "Error : edge " << e.id << " can not be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
                    << "because the graph does not contain its source node.";
                printErrorMessage(oss.str());
                sipIsErr = throwInvalidNodeException(sipCpp, sipCpp->getRoot()->source(e));
                break;
            }
            if (!sipCpp->isElement(sipCpp->getRoot()->target(e))) {
                std::ostringstream oss;
                oss << "Error : edge " << e.id << " can not be added in graph \\\"" << sipCpp->getName() << "\\\" (id " << sipCpp->getId() << ") "
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
tlp.Graph.delEdge(edge[, deleteInAllGraphs = False])

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

        void delEdges(tlp::Iterator<tlp::edge>* itE /Transfer/, bool deleteInAllGraphs = false)=0;
%Docstring
tlp.Graph.delEdges(itEdges[, deleteInAllGraphs=False])

.. versionadded:: 3.7

Deletes edges in the graph. These edges are also removed in
the sub-graphs hierarchy of the current graph to maintain
the sub-graph relation between graphs.
The ordering of remaining edges is preserved.

:param itEdges: an iterator on the edges to delete
:type itEdges: a Tulip iterator on :class:`tlp.edge` objects
:param deleteInAllGraphs: If :const:`True`, the edges are deleted in the whole hierarchy of graphs.
:param deleteInAllGraphs: boolean
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
tlp.Graph.delEdges(edges[, deleteInAllGraphs=False])

.. versionadded:: 4.5

Deletes edges in the graph. These edges are also removed in
the sub-graphs hierarchy of the current graph to maintain
the sub-graph relation between graphs.

:param edges: a vector of edges to add
:type edges: a list of :class:`tlp.edge` objects
:type itEdges: a Tulip iterator on :class:`tlp.edge` objects
:param deleteInAllGraphs: if :const:`True`, these edges are  deleted in the whole hierarchy of graphs.
:type deleteInAllGraphs: boolean
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

	tlp::node getOneNode() const ;
%Docstring
tlp.Graph.getOneNode()

Returns an existing node of the graph.

:rtype: :class:`tlp.node`  
%End

//===========================================================================================

	tlp::node getSource() const ;
%Docstring
tlp.Graph.getSource()

.. versionadded:: 3.7

Finds the first node whose input degree equals 0.
Returns the found node (invalid if there is no source);

:rtype: :class:`tlp.node`
%End

//===========================================================================================

	tlp::Iterator<tlp::node>* getNodes() const /TransferBack/;
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

	tlp::Iterator<tlp::node>* getInNodes(const tlp::node node) const /TransferBack/;
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

	tlp::node getOutNode(const tlp::node node, unsigned int i) const ;
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

	tlp::Iterator<tlp::node>* getOutNodes(const tlp::node node) const /TransferBack/;
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

	tlp::Iterator<tlp::node>* getInOutNodes(const tlp::node node) const /TransferBack/;
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

    tlp::Iterator<tlp::node>* bfs(const tlp::node root = tlp::node()) const /TransferBack/;
%Docstring
tlp.Graph.bfs([root])

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

    tlp::Iterator<tlp::node>* dfs(const tlp::node root = tlp::node()) const /TransferBack/;
%Docstring
tlp.Graph.dfs([root])

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

	tlp::Iterator<tlp::edge>* getEdges() const /TransferBack/;
%Docstring
tlp.Graph.getEdges()

Returns an iterator on the edges.

:rtype: a Tulip iterator on :class:`tlp.edge` objects
%End
	
%MethodCode
	sipRes = new tlp::StableIterator<tlp::edge>(sipCpp->getEdges());
%End		

//===========================================================================================

	tlp::edge getOneEdge() const ;
%Docstring
tlp.Graph.getOneEdge()

Returns an existing edge of the graph.

:rtype: :class:`tlp.edge`  
%End

//===========================================================================================

	tlp::Iterator<tlp::edge>* getOutEdges(const tlp::node node) const /TransferBack/;
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

	tlp::Iterator<tlp::edge>* getInOutEdges(const tlp::node node) const /TransferBack/;
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

	tlp::Iterator<tlp::edge>* getInEdges(const tlp::node node) const /TransferBack/;
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

	tlp::Iterator<tlp::edge>* getEdgeMetaInfo(const tlp::edge edge) const /TransferBack/;
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

	unsigned int getId() const;
%Docstring
tlp.Graph.getId()

Returns the graph's id. This id is unique.

:rtype: integer
%End	

//===========================================================================================

	unsigned int numberOfNodes() const ;
%Docstring
tlp.Graph.numberOfNodes()

Returns the number of nodes in the graph.

:rtype: integer
%End

//===========================================================================================

	unsigned int numberOfEdges() const ;
%Docstring
tlp.Graph.numberOfEdges()

Returns the number of edges in the graph.

:rtype: integer
%End


//===========================================================================================

	unsigned int deg(const tlp::node node) const ;
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

	unsigned int indeg(const tlp::node node) const ;
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

	unsigned int outdeg(const tlp::node node) const ;
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

	tlp::node source(const tlp::edge edge) const ;
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

	tlp::node target(const tlp::edge edge) const ;
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

	tlp::node opposite(const tlp::edge edge, const tlp::node node) const ;
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

	bool isElement(const tlp::node node) const ;
%Docstring
tlp.Graph.isElement(node)

Returns :const:`True` if the node is an element of the graph.

:param node: a node
:type node: :class:`tlp.node`
:rtype: boolean
%End

//===========================================================================================

	bool isElement(const tlp::edge edge) const ;
%Docstring
tlp.Graph.isElement(edge)

Returns :const:`True` if the edge is an element of the graph.

:param edge: an edge
:type edge: :class:`tlp.edge`
:rtype: boolean
%End

//===========================================================================================

	bool isMetaNode(const tlp::node node) const ;
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

	bool isMetaEdge(const tlp::edge edge) const ;
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

	tlp::edge existEdge(const tlp::node node1, const tlp::node node2, bool directed=true) const ;
%Docstring
tlp.Graph.existEdge(node1, node2[, directed=True])

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

	bool hasEdge(const tlp::node node1, const tlp::node node2, bool directed=true) const ;
%Docstring
tlp.Graph.hasEdge(node1, node2[, directed=True])

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

	std::vector<tlp::edge> getEdges(const tlp::node node1, const tlp::node node2, bool directed=true) const ;
%Docstring
tlp.Graph.getEdges(node1, node2[, directed=True])

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

:param name: the name of the boolean vector property to return or for n in graph.getNodes():
          print nto create
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
        releaseSIPWrapper(prop, sipFindType("tlp::PropertyInterface"));
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

	tlp::Iterator<std::string>* getLocalProperties() /TransferBack/;
%Docstring
tlp.Graph.getLocalProperties()

Returns an iterator on the names of the properties local to the graph.

:rtype: a Tulip iterator on string objects
%End	
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getLocalProperties());
%End		

//===========================================================================================

	tlp::Iterator<std::string>* getInheritedProperties() /TransferBack/;
%Docstring
tlp.Graph.getInheritedProperties()

Returns an iterator on the names of the properties inherited from the graph's ancestors.

:rtype: a Tulip iterator on string objects
%End
	
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getInheritedProperties());
%End		

//===========================================================================================

	tlp::Iterator<std::string>* getProperties() /TransferBack/;
%Docstring
tlp.Graph.getInheritedProperties()

Returns an iterator on the names of all the properties attached to the graph.

:rtype: a Tulip iterator on string objects
%End
	
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getProperties());
%End			
	
//===========================================================================================	
	
	bool applyIntegerAlgorithm(const std::string &algoName, tlp::IntegerProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.applyIntegerAlgorithm(algoName, result[, dataSet = None])

.. versionadded:: 3.8

Computes an integer property on the current graph 
using an external named integer algorithm (plugin).
Integer algorithm plugins are objects
implementing the tlp::IntegerAlgorithm interface in C++ or 
the :class:`tlp.IntegerAlgorithm` interface in Python.
The list of currently loaded integer algorithm plugins can be
retrieved through the :func:`tlp.getIntegerAlgorithmPluginsList` function.
The computed values will be stored in result. 
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
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
:param dataSet: a data set containing some parameters to transmit to the algorithm
:type dataSet: :class:`tlp.DataSet`
:rtype: (boolean, string)
:throws: an exception if the requested integer algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call integer algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).

%End	

%MethodCode
	if (pluginExists<tlp::IntegerAlgorithm>(*a0)) {
		tlp::IntegerProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = -1;
		std::string msg = "No Tulip integer algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}
%End

//===========================================================================================

	bool applyDoubleAlgorithm(const std::string &algoName, tlp::DoubleProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.applyDoubleAlgorithm(algoName, result[, dataSet = None])

.. versionadded:: 3.8

Computes a double property on the current graph 
using an external named double algorithm (plugin).
Double algorithm plugins are objects
implementing the tlp::DoubleAlgorithm interface in C++ or 
the :class:`tlp.DoubleAlgorithm` interface in Python.
The list of currently loaded double algorithm plugins can be
retrieved through the :func:`tlp.getDoubleAlgorithmPluginsList` function.
The computed values will be stored in result. 
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
To determine a plugin's parameters, you can either:
    
	* refer to its documentation
	* call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the double algorithm to call
:type algoName: string
:param result: a double property in which result of the algorithm will be stored
:type result: :class:`tlp.DoublrProperty`
:param dataSet: a data set containing some parameters to transmit to the algorithm
:type dataSet: :class:`tlp.DataSet`
:rtype: (boolean, string)
:throws: an exception if the requested double algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call double algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End
	
%MethodCode
	if (pluginExists<tlp::DoubleAlgorithm>(*a0)) {
		tlp::DoubleProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = -1;
		std::string msg = "No Tulip double algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

//===========================================================================================

	bool applyLayoutAlgorithm(const std::string &algoName, tlp::LayoutProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.applyLayoutAlgorithm(algoName, result[, dataSet = None])

.. versionadded:: 3.8

Computes a layout property on the current graph 
using an external named layout algorithm (plugin).
Layout algorithm plugins are objects
implementing the tlp::LayoutAlgorithm interface in C++ or 
the :class:`tlp.LayoutAlgorithm` interface in Python.
The list of currently loaded layout algorithm plugins can be
retrieved through the :func:`tlp.getLayoutAlgorithmPluginsList` function.
The computed values will be stored in result. 
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
To determine a plugin's parameters, you can either:
    
	* refer to its documentation
	* call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the layout algorithm to call
:type algoName: string
:param result: a layout property in which result of the algorithm will be stored
:type result: :class:`tlp.LayoutProperty`
:param dataSet: a data set containing some parameters to transmit to the algorithm
:type dataSet: :class:`tlp.DataSet`
:rtype: (boolean, string)
:throws: an exception if the requested layout algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call layout algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End
	
%MethodCode
	if (pluginExists<tlp::LayoutAlgorithm>(*a0)) {
		tlp::LayoutProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = -1;
		std::string msg = "No Tulip layout algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

//===========================================================================================

	bool applyBooleanAlgorithm(const std::string &algoName, tlp::BooleanProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.applyBooleanAlgorithm(algoName, result[, dataSet = None])

.. versionadded:: 3.8

Computes a boolean property on the current graph 
using an external named boolean algorithm (plugin).
Boolean algorithm plugins are objects
implementing the tlp::BooleanAlgorithm interface in C++ or 
the :class:`tlp.BooleanAlgorithm` interface in Python.
The list of currently loaded boolean algorithm plugins can be
retrieved through the :func:`tlp.getBooleanAlgorithmPluginsList` function.
The computed values will be stored in result. 
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
To determine a plugin's parameters, you can either:
    
	* refer to its documentation
	* call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the boolean algorithm to call
:type algoName: string
:param result: a boolean property in which result of the algorithm will be stored
:type result: :class:`tlp.BooleanProperty`
:param dataSet: a data set containing some parameters to transmit to the algorithm
:type dataSet: :class:`tlp.DataSet`
:rtype: (boolean, string)
:throws: an exception if the requested boolean algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call boolean algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).

%End
	
%MethodCode
	if (pluginExists<tlp::BooleanAlgorithm>(*a0)) {
		tlp::BooleanProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
        sipIsErr = -1;
		std::string msg = "No Tulip boolean algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}
%End

//===========================================================================================

	bool applySizeAlgorithm(const std::string &algoName, tlp::SizeProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.applySizeAlgorithm(algoName, result[, dataSet = None])

.. versionadded:: 3.8

Computes a size property on the current graph 
using an external named size algorithm (plugin).
Size algorithm plugins are objects
implementing the tlp::SizeAlgorithm interface in C++ or 
the :class:`tlp.SizeAlgorithm` interface in Python.
The list of currently loaded size algorithm plugins can be
retrieved through the :func:`tlp.getSizeAlgorithmPluginsList` function.
The computed values will be stored in result. 
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
To determine a plugin's parameters, you can either:
    
	* refer to its documentation
	* call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the size algorithm to call
:type algoName: string
:param result: a size property in which result of the algorithm will be stored
:type result: :class:`tlp.SizeProperty`
:param dataSet: a data set containing some parameters to transmit to the algorithm
:type dataSet: :class:`tlp.DataSet`
:rtype: (boolean, string)
:throws: an exception if the requested size algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call size algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End
	
%MethodCode
	if (pluginExists<tlp::SizeAlgorithm>(*a0)) {
		tlp::SizeProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = -1;
		std::string msg = "No Tulip size algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End



//===========================================================================================

bool applyColorAlgorithm(const std::string &algoName, tlp::ColorProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.applyColorAlgorithm(algoName, result[, dataSet = None])

.. versionadded:: 3.8

Computes a color property on the current graph 
using an external named color algorithm (plugin).
Color algorithm plugins are objects
implementing the tlp::ColorAlgorithm interface in C++ or 
the :class:`tlp.ColorAlgorithm` interface in Python.
The list of currently loaded color algorithm plugins can be
retrieved through the :func:`tlp.getColorAlgorithmPluginsList` function.
The computed values will be stored in result. 
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
To determine a plugin's parameters, you can either:
    
	* refer to its documentation
	* call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the color algorithm to call
:type algoName: string
:param result: a color property in which result of the algorithm will be stored
:type result: :class:`tlp.ColorProperty`
:param dataSet: a data set containing some parameters to transmit to the algorithm
:type dataSet: :class:`tlp.DataSet`
:rtype: (boolean, string)
:throws: an exception if the requested color algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call color algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End

%MethodCode
	if (pluginExists<tlp::ColorAlgorithm>(*a0)) {
		tlp::ColorProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = -1;
		std::string msg = "No Tulip color algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

//===========================================================================================

bool applyStringAlgorithm(const std::string &algoName, tlp::StringProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.applyStringAlgorithm(algoName, result[, dataSet = None])

.. versionadded:: 3.8

Computes a string property on the current graph 
using an external named string algorithm (plugin).
String algorithm plugins are objects
implementing the tlp::StringAlgorithm interface in C++ or 
the :class:`tlp.StringAlgorithm` interface in Python.
The list of currently loaded string algorithm plugins can be
retrieved through the :func:`tlp.getStringAlgorithmPluginsList` function.
The computed values will be stored in result. 
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
To determine a plugin's parameters, you can either:
    
	* refer to its documentation
	* call the :func:`tlp.getDefaultPluginParameters` with the name of the plugin

Returns a tuple whose first member is a boolean indicating if the 
algorithm terminates successfully and second member is a string 
which can contain an error message.

:param algoName: the name of the string algorithm to call
:type algoName: string
:param result: a string property in which result of the algorithm will be stored
:type result: :class:`tlp.StringProperty`
:param dataSet: a data set containing some parameters to transmit to the algorithm
:type dataSet: :class:`tlp.DataSet`
:rtype: (boolean, string)
:throws: an exception if the requested string algorithm plugin is not registered in the plugins database.

.. warning:: Previous values stored in result will be deleted.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call string algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End

%MethodCode
	if (pluginExists<tlp::StringAlgorithm>(*a0)) {
		tlp::StringProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = -1;
		std::string msg = "No Tulip string algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}
%End

 //===========================================================================================
    void push(bool unpopAllowed = true, std::vector<tlp::PropertyInterface*>* propertiesToPreserveOnPop= NULL);
%Docstring
tlp.Graph.push([unpopAllowed=True])

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
tlp.Graph.pop([unpopAllowed=True])

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

	tlp::node createMetaNode(const std::set<tlp::node> &nodeSet, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(nodeSet[, multiEdges=True, delAllEdge=True])

Closes a set of existing nodes into a metanode and returns it.
Edges from nodes in the set to other nodes are replaced with
edges from the metanode to the other nodes.

:param nodeSet: a list or set of existing nodes
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
    std::set<tlp::node>::iterator it = a0->begin();
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

// For code-compatibility with Tulip 3.X
       tlp::node createMetaNode(const std::vector<tlp::node> &nodeSet, bool multiEdges = true, bool delAllEdge = true);
%MethodCode
    std::vector<tlp::node>::const_iterator it = a0->begin();
    for (; it != a0->end() ; ++it) {
      if (!sipCpp->isElement(*it)) {
          sipIsErr = throwInvalidNodeException(sipCpp, *it);
          break;
       }
    }
    if (sipIsErr != -1) {
       sipRes = new tlp::node(sipCpp->createMetaNode(std::set<tlp::node>(a0->begin(), a0->end()), a1, a2));
    }
%End

//===========================================================================================	
		
	tlp::node createMetaNode(tlp::Graph* subgraph, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(subgraph[, multiEdges=True, delAllEdge=True])

Closes a subgraph into a metanode and returns it. Edges from nodes
in the subgraph to nodes outside the subgraph are replaced with
edges from the metanode to the nodes outside the subgraph.

The provided subgraph can not be a descendant of the current graph in the graph hierarchy.
When creating a metanode, all nodes that are closed in it are first deleted
from the graph. These nodes are also deleted in the subgraphs of the graph and
in all descendant subgraphs. So if the provided subgraph is a descendant of
the graph, all its nodes and edges will be deleted and the meta-node will
point to an empty subgraph.

:param subgraph: an existing subgraph
:type subgraph: :class:`tlp.Graph`
:param multiEdges: indicates if a meta edge will be created for each underlying edge
:type multiEdges: boolean
:param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
:type delAllEdge: boolean
:rtype: :class:`tlp.node`
:throws: an exception if the provided subgraph is a descandent of the graph in the graph hierarchy or if one of the element in the subgraph does not belong to the graph.

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

	void createMetaNodes(tlp::IteratorGraph *itGraph, tlp::Graph *quotientGraph, std::vector<tlp::node>& metaNodes /Out/);
%Docstring
tlp.Graph.createMetaNodes(itGraph, quotientGraph)

Populates a quotient graph with one meta node for each iterated graph.
Returns a list of created meta-nodes.

:param itGraph: a graph iterator, (typically a subgraph iterator)
:type itGraph: a Tulip iterator on :class:`tlp.Graph` objects
:param quotientGraph: the graph that will contain the meta nodes
:type quotientGraph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.node`
%End
	
//===========================================================================================

	void openMetaNode(tlp::node metaNode, bool updateProperties = true);
%Docstring
tlp.Graph.openMetaNode(metaNode[, updateProperties=True])

Opens a metanode and replaces all edges between that
meta node and other nodes in the graph.

:param metaNode: the meta-node to open
:type metaNode: :class:`tlp.node`
:param updateProperties: If :const:`True`, open meta node will update inner nodes layout, color, size, etc...
:type updateProperties: boolean

.. warning:: This method will fail when called on the root graph.

%End
	
//===========================================================================================
	
    tlp::PropertyInterface* __getitem__(const std::string &attributeName) const;
%MethodCode
        if (!sipCpp->existProperty(*a0)) {
        	std::ostringstream oss;
        	oss << "graph property named \"" << *a0 << "\" does not exist.";
			sipIsErr = -1;    
        	PyErr_SetString(PyExc_Exception, oss.str().c_str());
        } else {
            sipRes = sipCpp->getProperty(*a0);
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
    if (sipCpp->attributeExist(*a0)) {
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
    if (sipCpp->attributeExist(*a0)) {
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

:rtype: :class:`tlp.DataSet`
%End

//===========================================================================================

	SIP_PYOBJECT __repr__() const;
%MethodCode
	std::string graphName;
	sipCpp->getAttribute("name", graphName);
	std::ostringstream oss;
	oss << "<graph \"" << graphName << "\" (id " << sipCpp->getId() << ") >";
	std::string s = oss.str();
#if PY_MAJOR_VERSION >= 3
    sipRes = PyUnicode_FromString(s.c_str());
#else
    sipRes = PyString_FromString(s.c_str());
#endif
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

tlp::Graph *loadGraph(const std::string &filename) /TransferBack/;
%Docstring
tlp.loadGraph(filename)

Loads a graph in the tlp format from a file (extension can be .tlp or .tlp.gz).
Returns a new graph or :const:`None` if the import fails.

:param filename: the path to the tlp file
:type filename: string
:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

bool saveGraph(tlp::Graph *graph, const std::string &filename);
%Docstring
tlp.saveGraph(graph, filename)

Saves a graph to a file in the tlp format. Extension of the destination file can be either 
.tlp (raw export) or .tlp.gz (compressed export). Returns :const:`True` if the graph has been 
successfully saved.

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

Saves a graph in the tlp or tlp.gz format to the same file from which it has been loaded.
Returns :const:`True` if the graph has been successfully saved.

:param graph: the graph to export
:type graph: :class:`tlp.Graph`
:rtype: boolean
:throws: an exception if there is no tlp (or tlp.gz) file attached to the graph
%End

%MethodCode
    bool ok = false;
    std::string tlpFilePath;
    if (a0->attributeExist("file")) {
      a0->getAttribute("file", tlpFilePath);
      std::string ext = tlpFilePath.substr(tlpFilePath.rfind("tlp"));
      if (ext == "tlp" || ext == "tlp.gz") {
         ok = true;
      }
    }
    if (ok) {
      sipRes = tlp::saveGraph(a0, tlpFilePath);
    } else {
      sipIsErr = -1;
      std::string msg = "No tlp (or tlp.gz) file attached to the graph \"";
      msg += a0->getName();
      msg += "\".";
      PyErr_SetString(PyExc_Exception, msg.c_str());
    }
%End

//===========================================================================================

tlp::Graph *importGraph(const std::string &importPluginName, tlp::DataSet dataSet = tlp::DataSet()) /TransferBack/;
%Docstring
tlp.importGraph(importPluginName[, dataSet, newGraph = None])

Imports a graph using a Tulip import plugin (must be loaded).
Returns a new graph or None if the import fails.

:param importPluginName: the name of the Tulip import plugin
:type importPluginName: string
:param dataSet: a dataset filled with parameters to transmit to the import plugin
:type dataSet: :class:`tlp.DataSet`
:param newGraph: if provided, add imported graph elements in that graph
:type newGraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Graph`
:throws: an exception if the requested import plugin is not registered in the plugins database. 
%End

%MethodCode
	if (pluginExists<tlp::ImportModule>(*a0)) {
		sipRes = tlp::importGraph(*a0, *a1, 0);
	} else {
		sipIsErr = -1;
		std::string msg = "No Tulip import plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End


//===========================================================================================

tlp::Graph *importGraph(const std::string &importPluginName, tlp::DataSet &dataSet, tlp::Graph *newGraph);
%Docstring
tlp.importGraph(importPluginName, dataSet[, newGraph = None])

Imports a graph using a Tulip import plugin (must be loaded).
Import plugins are objects implementing the tlp::ImportModule interface
in C++ or the :class:`tlp.ImportModule` interface in Python.
The list of currently loaded import plugins can be
retrieved through the :func:`tlp.getImportPluginsList` function.
Returns a new graph or None if the import fails.

:param importPluginName: the name of the Tulip import plugin
:type importPluginName: string
:param dataSet: a dataset filled with parameters to transmit to the import plugin
:type dataSet: :class:`tlp.DataSet`
:param newGraph: if provided, add imported graph elements in that graph
:type newGraph: :class:`tlp.Graph`
:rtype: :class:`tlp.Graph`
:throws: an exception if the requested import plugin is not registered in the plugins database.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call import modules (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End

%MethodCode
    if (pluginExists<tlp::ImportModule>(*a0)) {
        sipRes = tlp::importGraph(*a0, *a1, 0, a2);
    } else {
        sipIsErr = -1;
        std::string msg = "No Tulip import plugin named  ";
        msg += *a0;
        msg += ".";
        PyErr_SetString(PyExc_Exception, msg.c_str());
    }
%End

//===========================================================================================

    bool exportGraph(const std::string &exportPluginName, tlp::Graph *graph, const std::string &outputFilePath, tlp::DataSet dataSet = tlp::DataSet());
%Docstring
tlp.exportGraph(exportPluginName, graph, outputFilePath[, dataSet])

Exports a graph to a file using a Tulip export plugin (must be loaded).

:param graph: the graph to export
:type graph: :class:`tlp.Graph`
:param outputFilePath: the path of the file the export plugin will write to
:type outputFilePath: string
:param exportPluginName: the name of the Tulip export plugin to execute
:type exportPluginName: string
:param dataSet: a dataset filled with parameters to transmit to the export plugin
:type dataSet: :class:`tlp.DataSet`
:rtype: boolean
:throws: an exception if the requested export plugin is not registered in the plugins database. 

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call import modules (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End

%MethodCode
    if (pluginExists<tlp::ExportModule>(*a0)) {
        std::ostream *ofs = tlp::getOutputFileStream((*a2).c_str());
        if (ofs->good()) {
            sipRes = tlp::exportGraph(a1, *ofs, *a0, *a3);
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

void copyToGraph(tlp::Graph *outGraph, tlp::Graph *inGraph, tlp::BooleanProperty* inSelection=0, tlp::BooleanProperty* outSelection=0 );
%Docstring
tlp.copyToGraph(outGraph, inGraph[, inSelection = None, outSelection = None])

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
tlp.removeFromGraph(inGraph[, inSelection = None])

Removes the selected part of a graph (properties values, nodes and edges).
If no selection is done, the whole graph is reseted to default value.

:param inGraph: the graph on which to remove elements
:type inGraph: :class:`tlp.Graph`
:param inSelection: boolean property attached to inGraph, only selected elements will be removed if provided
:type inSelection: :class:`tlp.BooleanProperty`

.. warning:: The selection is extended to all selected edge ends.

%End

//===========================================================================================
tlp::IteratorGraph *getRootGraphs();
%Docstring
tlp.getRootGraphs()

Returns an iterator on all the currently existing root graphs.

:rtype: a Tulip iterator on :class:`tlp.Graph` objects
%End

};

