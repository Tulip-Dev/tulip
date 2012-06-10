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
#include <tulip/ForEach.h>
#include <fstream>
namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;
};
typedef std::pair<tlp::node, tlp::node> std_pair_0100tlp_node_0100tlp_node;

extern void releaseGraphWrapper(tlp::Graph *graph);
extern void releaseGraphHierarchyWrappers(tlp::Graph *graph);
extern void initializeGraphViewProperties(tlp::Graph * graph);
%End

%ModuleCode
void releaseGraphWrapper(tlp::Graph *graph) {
    releaseSIPWrapper(graph, sipType_tlp_Graph);
}

void releaseGraphHierarchyWrappers(tlp::Graph *graph) {
    tlp::Graph *sg = NULL;
    forEach(sg, graph->getSubGraphs()) {
        releaseGraphHierarchyWrappers(sg);
    }
    releaseGraphWrapper(graph);
}

void initializeGraphViewProperties(tlp::Graph *graph) {
    if (!graph->existProperty("viewSize")) {
        graph->getProperty<tlp::SizeProperty>("viewSize")->setAllNodeValue(tlp::Size(1,1,1));
        graph->getProperty<tlp::SizeProperty>("viewSize")->setAllEdgeValue(tlp::Size(0.125,0.125,0.5));
    }
    if (!graph->existProperty("viewColor")) {
        graph->getProperty<tlp::ColorProperty>("viewColor")->setAllNodeValue(tlp::Color(255,0,0));
        graph->getProperty<tlp::ColorProperty>("viewColor")->setAllEdgeValue(tlp::Color(0,0,0));
    }
    if (!graph->existProperty("viewShape")) {
        graph->getProperty<tlp::IntegerProperty>("viewShape")->setAllNodeValue(4);
        graph->getProperty<tlp::IntegerProperty>("viewShape")->setAllEdgeValue(0);
    }
    if (!graph->existProperty("viewFont")) {
        graph->getProperty<tlp::StringProperty>("viewFont")->setAllNodeValue(tlp::TulipBitmapDir + "font.ttf");
        graph->getProperty<tlp::StringProperty>("viewFont")->setAllEdgeValue(tlp::TulipBitmapDir + "font.ttf");
    }
    if (!graph->existProperty("viewFontSize")) {
        graph->getProperty<tlp::IntegerProperty>("viewFontSize")->setAllNodeValue(18);
        graph->getProperty<tlp::IntegerProperty>("viewFontSize")->setAllEdgeValue(18);
    }
    if (!graph->existProperty("viewSrcAnchorShape")) {
        graph->getProperty<tlp::IntegerProperty>("viewSrcAnchorShape")->setAllEdgeValue(UINT_MAX);
    }
    if (!graph->existProperty("viewSrcAnchorSize")) {
        graph->getProperty<tlp::SizeProperty>("viewSrcAnchorSize")->setAllEdgeValue(tlp::Size(0.25,0.25,0.25));
    }
    if (!graph->existProperty("viewTgtAnchorShape")) {
        graph->getProperty<tlp::IntegerProperty>("viewTgtAnchorShape")->setAllEdgeValue(50);
    }
    if (!graph->existProperty("viewTgtAnchorSize")) {
        graph->getProperty<tlp::SizeProperty>("viewTgtAnchorSize")->setAllEdgeValue(tlp::Size(0.25,0.25,0.25));
    }
}
%End

typedef std::pair<tlp::node, tlp::node> std_pair_0100tlp_node_0100tlp_node;

namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;

enum ElementType {NODE=0, EDGE};

class Graph : tlp::Observable /Abstract/ {
%TypeHeaderCode
#include <tulip/Graph.h>
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
The list of currently loaded algorithm plugins can be
retrieved through the :func:`tlp.getAlgorithmPluginsList` function.
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
:throws: an exception if the requested algorithm plugin is not registered in the plugins database.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End  	

%MethodCode
    if (pluginExists(*a0, "Algorithm")) {
        sipRes = sipCpp->applyAlgorithm(*a0, *a1, a2);
    } else {
        sipIsErr = 1;
        std::string msg = "No Tulip general algorithm plugin named  ";
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

	tlp::Graph *addSubGraph(tlp::BooleanProperty *selection=0);
%Docstring
tlp.Graph.addSubGraph([selection = None])

Creates and returns a new sub-graph. The elements of the new sub-graph are those 
selected in the selection. If there is no selection an empty sub-graph is returned.

:param selection: a Boolean property whose selected elements will be added to the sub-graph
:type selection: :class:`tlp.BooleanProperty`
:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

	tlp::Graph *addSubGraph(std::string name);
%Docstring
tlp.Graph.addSubGraph(name)

.. versionadded:: 3.7

Creates and returns a new named sub-graph of this graph.

:param name: The name of the newly created subgraph.
:type name: string
:rtype: :class:`tlp.Graph`
%End	

//===========================================================================================

 	tlp::Graph* addCloneSubGraph(std::string name = "unnamed");
%Docstring
tlp.Graph.addCloneSubGraph(name = "unnamed")

.. versionadded:: 3.7

Creates and returns a subgraph of this graph that contains all its elements.

:param name: The name of the newly created subgraph. Defaults to "unnamed".
:type name: string
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
:type nodeSet: list of :class:`tlp.node`
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
      // the sub-graph will be deleted after the call to tlp::Graph::delSubGraph(tlp::Graph *)
      // we need to release the SIP wrapper associated to the C++ pointer otherwise the following
      // Python code raises a segmentation fault :
      // sg = graph.addSubGraph()
      // graph.delSubGraph(sg)
      // print sg # or any operation on sg
      // An execption is now thrown claiming that the underlying C++ object has been deleted
      releaseGraphWrapper(a0);
      sipCpp->delSubGraph(a0);
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
      // the sub-graphs will be deleted after the call to tlp::Graph::delAllSubGraphs(tlp::Graph *)
      // we need to release the possible SIP wrappers associated to the C++ pointers otherwise the following
      // Python code raises a segmentation fault :
      // sg = graph.addSubGraph()
      // sg2 = sg.addSubGraph()
      // graph.delAllSubGraphs(sg2)
      // print sg2 # or any operation on sg2
      // An execption is now thrown claiming that the underlying C++ object has been deleted
      releaseGraphHierarchyWrappers(a0);
      sipCpp->delAllSubGraphs(a0);
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

        virtual void delNodes(tlp::Iterator<tlp::node>* itN /Transfer/, bool deleteInAllGraphs = false)=0;
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

        void addEdges(const std::vector<std_pair_0100tlp_node_0100tlp_node>& edges, std::vector<tlp::edge>& addedEdges /Out/);
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
        }
        if (!sipCpp->isElement((*a0)[i].second)) {
            sipIsErr = throwInvalidNodeException(sipCpp, (*a0)[i].second);
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
        sipCpp->delEdges(&sItEdges);
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

Returns the edge if it exists an edge between two nodes.
If no edge is found, returns an invalid edge.

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
			sipRes = new tlp::edge(sipCpp->existEdge(*a0, *a1, a2));
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
	sipRes = sipCpp->getProperty<tlp::BooleanProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::BooleanProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::BooleanVectorProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::BooleanVectorProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::LayoutProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::LayoutProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::CoordVectorProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::CoordVectorProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::ColorProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::ColorProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::ColorVectorProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::ColorVectorProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::DoubleProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::DoubleProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::DoubleVectorProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::DoubleVectorProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::StringProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::StringProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::StringVectorProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::StringVectorProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::SizeProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::SizeProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::SizeVectorProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::SizeVectorProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::IntegerProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::IntegerProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::IntegerVectorProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::IntegerVectorProperty>(*a0);
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
	sipRes = sipCpp->getProperty<tlp::GraphProperty>(*a0);
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
	sipRes = sipCpp->getLocalProperty<tlp::GraphProperty>(*a0);
%End

//===========================================================================================

	tlp::PropertyInterface* getProperty(const std::string& name);
%Docstring
tlp.Graph.getProperty(name)

Returns the property associated to name in the graph properties pool.

:param name: the name of the property to return
:type name: string
:rtype: a subclass of :class:`tlp.PropertyInterface`
:throws: an exception is there is no property with that name in the properties pool.
%End	
 
%MethodCode
    if (sipCpp->existProperty(*a0)) {
        sipRes = sipCpp->getProperty(*a0);
    } else {
        std::ostringstream oss;
        oss << "graph property named \"" << *a0 << "\" does not exist.";
        sipIsErr = -1;
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
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
        releaseSIPWrapper(prop, sipType_tlp_PropertyInterface);
        sipCpp->delLocalProperty(*a0);
    } else {
        sipIsErr = 1;
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
	
	bool computeIntegerProperty(const std::string &algoName, tlp::IntegerProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeIntegerProperty(algoName, result[, dataSet = None])

.. deprecated:: 3.8 use :meth:`tlp.Graph.applyIntegerAlgorithm` instead

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
    if (pluginExists(*a0, "Integer")) {
		tlp::IntegerProperty tmp(sipCpp);
		sipRes = sipCpp->computeProperty<tlp::IntegerProperty>(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip integer algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}
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
    if (pluginExists(*a0, "Integer")) {
		tlp::IntegerProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip integer algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}
%End

//===========================================================================================

	bool computeDoubleProperty(const std::string &algoName, tlp::DoubleProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeDoubleProperty(algoName, result[, dataSet = None])

.. deprecated:: 3.8 use :meth:`tlp.Graph.applyDoubleAlgorithm` instead

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
    if (pluginExists(*a0, "Double")) {
		tlp::DoubleProperty tmp(sipCpp);
		sipRes = sipCpp->computeProperty<tlp::DoubleProperty>(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip double algorithm plugin named  ";
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
    if (pluginExists(*a0, "Double")) {
		tlp::DoubleProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip double algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

//===========================================================================================

	bool computeLayoutProperty(const std::string &algoName, tlp::LayoutProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeLayoutProperty(algoName, result[, dataSet = None])

.. deprecated:: 3.8 use :meth:`tlp.Graph.applyLayoutAlgorithm` instead

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
    if (pluginExists(*a0, "Layout")) {
		tlp::LayoutProperty tmp(sipCpp);
		sipRes = sipCpp->computeProperty<tlp::LayoutProperty>(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip layout algorithm plugin named  ";
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
    if (pluginExists(*a0, "Layout")) {
		tlp::LayoutProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip layout algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

//===========================================================================================

	bool computeBooleanProperty(const std::string &algoName, tlp::BooleanProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeBooleanProperty(algoName, result[, dataSet = None])

.. deprecated:: 3.8 use :meth:`tlp.Graph.applyBooleanAlgorithm` instead

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
    if (pluginExists(*a0, "Boolean")) {
		tlp::BooleanProperty tmp(sipCpp);
		sipRes = sipCpp->computeProperty<tlp::BooleanProperty>(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip boolean algorithm plugin named  ";
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
    if (pluginExists(*a0, "Boolean")) {
		tlp::BooleanProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip boolean algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}
%End

//===========================================================================================

	bool computeSizeProperty(const std::string &algoName, tlp::SizeProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeSizeProperty(algoName, result[, dataSet = None])

.. deprecated:: 3.8 use :meth:`tlp.Graph.applySizeAlgorithm` instead

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
    if (pluginExists(*a0, "Size")) {
		tlp::SizeProperty tmp(sipCpp);
		sipRes = sipCpp->computeProperty<tlp::SizeProperty>(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip size algorithm plugin named  ";
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
    if (pluginExists(*a0, "Size")) {
		tlp::SizeProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip size algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

//===========================================================================================

bool computeColorProperty(const std::string &algoName, tlp::ColorProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeColorProperty(algoName, result[, dataSet = None])

.. deprecated:: 3.8 use :meth:`tlp.Graph.applyColorAlgorithm` instead

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
    if (pluginExists(*a0, "Color")) {
		tlp::ColorProperty tmp(sipCpp);
		sipRes = sipCpp->computeProperty<tlp::ColorProperty>(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip color algorithm plugin named  ";
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
    if (pluginExists(*a0, "Color")) {
		tlp::ColorProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip color algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

//===========================================================================================

bool computeStringProperty(const std::string &algoName, tlp::StringProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeStringProperty(algoName, result[, dataSet = None])

.. deprecated:: 3.8 use :meth:`tlp.Graph.applyStringAlgorithm` instead

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
    if (pluginExists(*a0, "String")) {
		tlp::StringProperty tmp(sipCpp);
		sipRes = sipCpp->computeProperty<tlp::StringProperty>(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip string algorithm plugin named  ";
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
    if (pluginExists(*a0, "String")) {
		tlp::StringProperty tmp(sipCpp);
		sipRes = sipCpp->applyPropertyAlgorithm(*a0, &tmp, *a2, NULL, a3);
		*a1 = tmp;
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip string algorithm plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}
%End

//===========================================================================================

    void addGraphObserver(tlp::GraphObserver *) const;
%Docstring
tlp.Graph.addGraphObserver(graphObserver)

.. versionadded:: 3.8

Registers a new graph observer object
that will receive notifications when the graph is modified.

:param graphObserver: the new graph observer to register
:type graphObserver: :class:`tlp.GraphObserver`
%End

//===========================================================================================

    unsigned int countGraphObservers() const;
%Docstring
tlp.Graph.countGraphObservers()

.. versionadded:: 3.8

Returns the number of registered graph observers.

:rtype: integer
%End

//===========================================================================================

    void removeGraphObserver(tlp::GraphObserver *) const;
%Docstring
tlp.Graph.removeGraphObserver(graphObserver)

.. versionadded:: 3.8

Removes a registered graph observer object.

:param graphObserver: the graph observer to remove
:type graphObserver: :class:`tlp.GraphObserver`
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

:param nodeSet: a set of existing nodes
:type nodeSet: list of :class:`tlp.node`
:param multiEdges: indicates if a meta edge will be created for each underlying edge
:type multiEdges: boolean
:param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
:type delAllEdge: boolean
:rtype: :class:`tlp.node`

 .. warning:: This method will fail when called on the root graph.

%End

//===========================================================================================	
		
	tlp::node createMetaNode(tlp::Graph* subgraph, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(subgraph[, multiEdges=True, delAllEdge=True])

Closes an existing subgraph into a metanode and returns it.  Edges from nodes
in the subgraph to nodes outside the subgraph are replaced with
edges from the metanode to the nodes outside the subgraph.

:param subgraph: an existing subgraph
:type subgraph: :class:`tlp.Graph`
:param multiEdges: indicates if a meta edge will be created for each underlying edge
:type multiEdges: boolean
:param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
:type delAllEdge: boolean
:rtype: :class:`tlp.node`

.. warning:: This method will fail when called on the root graph.

%End

//===========================================================================================

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
        if (sipCpp->existProperty(*a0)) {
            sipRes = sipCpp->getProperty(*a0);
        } else {
        	std::ostringstream oss;
        	oss << "graph property named \"" << *a0 << "\" does not exist.";
			sipIsErr = -1;    
        	PyErr_SetString(PyExc_Exception, oss.str().c_str());
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
  int state=0, iserr=0;
  if (PyBool_Check(a1)) {
    long l = PyLong_AsLong(a1);
    sipCpp->setAttribute<bool>(*a0, (l > 0));
#if PY_MAJOR_VERSION < 3
  } else if (PyInt_CheckExact(a1)) {
    sipCpp->setAttribute(*a0, PyInt_AsLong(a1)); 
#endif
  } else if (PyLong_Check(a1)) {
    sipCpp->setAttribute(*a0, PyLong_AsLong(a1));
  } else if (PyFloat_Check(a1)) {
    sipCpp->setAttribute(*a0, PyFloat_AsDouble(a1));
  } else if (sipCanConvertToType(a1, sipType_std_string, SIP_NOT_NONE)) {   
    std::string *s = reinterpret_cast<std::string *>(sipConvertToType(a1, sipType_std_string, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *s); 
  } else if (sipCanConvertToType(a1, sipType_tlp_Coord, SIP_NOT_NONE)) {    
    tlp::Coord *c = reinterpret_cast<tlp::Coord *>(sipConvertToType(a1, sipType_tlp_Coord, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *c);
  } else if (sipCanConvertToType(a1, sipType_tlp_Color, SIP_NOT_NONE)) {    
    tlp::Color *c = reinterpret_cast<tlp::Color *>(sipConvertToType(a1, sipType_tlp_Color, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *c);
  } else if (sipCanConvertToType(a1, sipType_tlp_Size, SIP_NOT_NONE)) {   
    tlp::Size *s = reinterpret_cast<tlp::Size *>(sipConvertToType(a1, sipType_tlp_Size, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *s);
  } else if (sipCanConvertToType(a1, sipType_tlp_DataSet, SIP_NOT_NONE)) {    
    tlp::DataSet *ds = reinterpret_cast<tlp::DataSet *>(sipConvertToType(a1, sipType_tlp_DataSet, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *ds);  
  } else if (sipCanConvertToType(a1, sipType_tlp_ColorScale, SIP_NOT_NONE)) {   
    tlp::ColorScale *cs = reinterpret_cast<tlp::ColorScale *>(sipConvertToType(a1, sipType_tlp_ColorScale, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *cs);
  } else if (sipCanConvertToType(a1, sipType_tlp_StringCollection, SIP_NOT_NONE)) {   
    tlp::StringCollection *sc = reinterpret_cast<tlp::StringCollection *>(sipConvertToType(a1, sipType_tlp_StringCollection, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *sc);    
  } else if (sipCanConvertToType(a1, sipType_tlp_BooleanProperty, SIP_NOT_NONE)) {    
    tlp::BooleanProperty *p = reinterpret_cast<tlp::BooleanProperty *>(sipConvertToType(a1, sipType_tlp_BooleanProperty, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, p);
  } else if (sipCanConvertToType(a1, sipType_tlp_ColorProperty, SIP_NOT_NONE)) {    
    tlp::ColorProperty *p = reinterpret_cast<tlp::ColorProperty *>(sipConvertToType(a1, sipType_tlp_ColorProperty, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, p);
  } else if (sipCanConvertToType(a1, sipType_tlp_DoubleProperty, SIP_NOT_NONE)) {   
    tlp::DoubleProperty *p = reinterpret_cast<tlp::DoubleProperty *>(sipConvertToType(a1, sipType_tlp_DoubleProperty, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, p);
  } else if (sipCanConvertToType(a1, sipType_tlp_IntegerProperty, SIP_NOT_NONE)) {    
    tlp::IntegerProperty *p = reinterpret_cast<tlp::IntegerProperty *>(sipConvertToType(a1, sipType_tlp_IntegerProperty, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, p);
  } else if (sipCanConvertToType(a1, sipType_tlp_LayoutProperty, SIP_NOT_NONE)) {   
    tlp::LayoutProperty *p = reinterpret_cast<tlp::LayoutProperty *>(sipConvertToType(a1, sipType_tlp_LayoutProperty, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, p);
  } else if (sipCanConvertToType(a1, sipType_tlp_SizeProperty, SIP_NOT_NONE)) {   
    tlp::SizeProperty *p = reinterpret_cast<tlp::SizeProperty *>(sipConvertToType(a1, sipType_tlp_SizeProperty, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, p);
  } else if (sipCanConvertToType(a1, sipType_tlp_StringProperty, SIP_NOT_NONE)) {   
    tlp::StringProperty *p = reinterpret_cast<tlp::StringProperty *>(sipConvertToType(a1, sipType_tlp_StringProperty, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, p);              
  } else if (sipCanConvertToType(a1, sipType_tlp_PropertyInterface, SIP_NOT_NONE)) {    
    tlp::PropertyInterface *p = reinterpret_cast<tlp::PropertyInterface *>(sipConvertToType(a1, sipType_tlp_PropertyInterface, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, p);
  } else if (sipCanConvertToType(a1, sipType_std_vector_0100tlp_node, SIP_NOT_NONE)) {    
    std::vector<tlp::node> *p = reinterpret_cast<std::vector<tlp::node> *>(sipConvertToType(a1, sipType_std_vector_0100tlp_node, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *p);
  } else if (sipCanConvertToType(a1, sipType_std_vector_0100tlp_edge, SIP_NOT_NONE)) {    
    std::vector<tlp::edge> *p = reinterpret_cast<std::vector<tlp::edge> *>(sipConvertToType(a1, sipType_std_vector_0100tlp_edge, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *p);
  } else if (sipCanConvertToType(a1, sipType_std_vector_0100tlp_Coord, SIP_NOT_NONE)) {    
    std::vector<tlp::Coord> *p = reinterpret_cast<std::vector<tlp::Coord> *>(sipConvertToType(a1, sipType_std_vector_0100tlp_Coord, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *p);
  } else if (sipCanConvertToType(a1, sipType_std_vector_0100tlp_Color, SIP_NOT_NONE)) {    
    std::vector<tlp::Color> *p = reinterpret_cast<std::vector<tlp::Color> *>(sipConvertToType(a1, sipType_std_vector_0100tlp_Color, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *p);
  } else if (sipCanConvertToType(a1, sipType_std_vector_0100tlp_Size, SIP_NOT_NONE)) {    
    std::vector<tlp::Size> *p = reinterpret_cast<std::vector<tlp::Size> *>(sipConvertToType(a1, sipType_std_vector_0100tlp_Size, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *p);
  } else if (sipCanConvertToType(a1, sipType_std_vector_0100std_string, SIP_NOT_NONE)) {
    std::vector<std::string> *p = reinterpret_cast<std::vector<std::string> *>(sipConvertToType(a1, sipType_std_vector_0100std_string, NULL, SIP_NOT_NONE, &state, &iserr));
    sipCpp->setAttribute(*a0, *p);
  } else {
  	sipIsErr = 1;
	std::string msg = "Object of type ";
	msg += std::string(a1->ob_type->tp_name);
	msg += " can not be store as graph attribute.";
	PyErr_SetString(PyExc_Exception, msg.c_str());
  }
%End 

//===========================================================================================

SIP_PYOBJECT getAttribute(const std::string& name);
%Docstring
tlp.Graph.getAttribute(name)

Returns a reference on an attribute of the graph or :const:`None` if it does not exist.

:param name: the name of the attribute to return.
:type name: string
:rtype: object
%End


%MethodCode
bool ok = false;
if (sipCpp->getAttributes().exist(*a0)) {
  tlp::DataType* dataType = sipCpp->getAttribute(*a0);
  if (dataType->getTypeName() == std::string(typeid(bool).name())) {
    ok = true;
    sipRes = PyBool_FromLong(*(reinterpret_cast<bool*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(int).name())) {
    ok = true;
    sipRes = PyLong_FromLong(*(reinterpret_cast<int*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(long).name())) {
    ok = true;
    sipRes = PyLong_FromLong(*(reinterpret_cast<long*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(short).name())) {
    ok = true;
    sipRes = PyLong_FromLong(*(reinterpret_cast<short*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(unsigned int).name())) {
    ok = true;
    sipRes = PyLong_FromUnsignedLong(*(reinterpret_cast<unsigned int*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(unsigned long).name())) {
    ok = true;
    sipRes = PyLong_FromUnsignedLong(*(reinterpret_cast<unsigned long*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(unsigned short).name())) {
    ok = true;
    sipRes = PyLong_FromUnsignedLong(*(reinterpret_cast<unsigned short*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(double).name())) {
    ok = true;
    sipRes = PyFloat_FromDouble(*(reinterpret_cast<double*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(float).name())) {
    ok = true;
    sipRes = PyFloat_FromDouble(*(reinterpret_cast<float*>(dataType->value)));
    
  } else if (dataType->getTypeName() == std::string(typeid(std::string).name())) {
    ok = true;
#if PY_MAJOR_VERSION >= 3
    sipRes = PyUnicode_FromString((*(reinterpret_cast<std::string*>(dataType->value))).c_str());
#else
    sipRes = PyString_FromString((*(reinterpret_cast<std::string*>(dataType->value))).c_str());
#endif
  } else if (dataType->getTypeName() == std::string(typeid(tlp::DataSet).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<tlp::DataSet *>(dataType->value), sipType_tlp_DataSet, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::DataSet).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<tlp::DataSet *>(dataType->value), sipType_tlp_DataSet, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::Coord).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<tlp::Coord *>(dataType->value), sipType_tlp_Coord, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::Color).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<tlp::Color *>(dataType->value), sipType_tlp_Color, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::Size).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<tlp::Size *>(dataType->value), sipType_tlp_Size, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::ColorScale).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<tlp::ColorScale *>(dataType->value), sipType_tlp_ColorScale, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::StringCollection).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<tlp::StringCollection *>(dataType->value), sipType_tlp_StringCollection, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::PropertyInterface*).name())) {
    ok = true;
    sipRes = sipConvertFromType(*(reinterpret_cast<tlp::PropertyInterface **>(dataType->value)), sipType_tlp_PropertyInterface, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::BooleanProperty*).name())) {
    ok = true;
    sipRes = sipConvertFromType(*(reinterpret_cast<tlp::BooleanProperty **>(dataType->value)), sipType_tlp_BooleanProperty, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::ColorProperty*).name())) {
    ok = true;
    sipRes = sipConvertFromType(*(reinterpret_cast<tlp::ColorProperty **>(dataType->value)), sipType_tlp_ColorProperty, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::DoubleProperty*).name())) {
    ok = true;
    sipRes = sipConvertFromType(*(reinterpret_cast<tlp::DoubleProperty **>(dataType->value)), sipType_tlp_DoubleProperty, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::IntegerProperty*).name())) {
    ok = true;
    sipRes = sipConvertFromType(*(reinterpret_cast<tlp::IntegerProperty **>(dataType->value)), sipType_tlp_IntegerProperty, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::LayoutProperty*).name())) {
    ok = true;
    sipRes = sipConvertFromType(*(reinterpret_cast<tlp::LayoutProperty **>(dataType->value)), sipType_tlp_LayoutProperty, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::SizeProperty*).name())) {
    ok = true;
    sipRes = sipConvertFromType(*(reinterpret_cast<tlp::SizeProperty **>(dataType->value)), sipType_tlp_SizeProperty, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::StringProperty*).name())) {
    ok = true;
    sipRes = sipConvertFromType(*(reinterpret_cast<tlp::StringProperty **>(dataType->value)), sipType_tlp_StringProperty, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(std::vector<tlp::node>).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<std::vector<tlp::node> *>(dataType->value), sipType_std_vector_0100tlp_node, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(std::set<tlp::node>).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<std::set<tlp::node> *>(dataType->value), sipType_std_set_0100tlp_node, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(std::vector<tlp::edge>).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<std::vector<tlp::edge> *>(dataType->value), sipType_std_vector_0100tlp_edge, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(std::set<tlp::edge>).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<std::set<tlp::edge> *>(dataType->value), sipType_std_set_0100tlp_edge, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(std::vector<tlp::Coord>).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<std::vector<tlp::Coord> *>(dataType->value), sipType_std_vector_0100tlp_Coord, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(std::vector<tlp::Color>).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<std::vector<tlp::Color> *>(dataType->value), sipType_std_vector_0100tlp_Color, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(std::vector<tlp::Size>).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<std::vector<tlp::Size> *>(dataType->value), sipType_std_vector_0100tlp_Size, NULL);
  } else if (dataType->getTypeName() == std::string(typeid(std::vector<std::string>).name())) {
    ok = true;
    sipRes = sipConvertFromType(reinterpret_cast<std::vector<std::string> *>(dataType->value), sipType_std_vector_0100std_string, NULL);
  }
  delete dataType;
}

if (!ok) {
	std::ostringstream oss;
  	oss << "Graph attribute named \"" << *a0 << "\" does not exist.";
  	sipRes = NULL;
  	sipIsErr = 1;
  	PyErr_SetString(PyExc_AttributeError, oss.str().c_str());
}
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

%MethodCode
    sipRes = tlp::newGraph();
    initializeGraphViewProperties(sipRes);
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

%MethodCode
    sipRes = tlp::loadGraph(*a0);
    initializeGraphViewProperties(sipRes);
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

tlp::Graph *importGraph(const std::string &importPluginName, tlp::DataSet dataSet = tlp::DataSet()) /TransferBack/;
%Docstring
tlp.importGraph(importPluginName[, dataSet, newGraph = None])

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
    if (pluginExists(*a0, "Import")) {
        sipRes = tlp::importGraph(*a0, *a1, 0);
        initializeGraphViewProperties(sipRes);
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip import plugin named  ";
		msg += *a0;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End

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
    if (pluginExists(*a0, "Import")) {
        sipRes = tlp::importGraph(*a0, *a1, 0, a2);
        initializeGraphViewProperties(sipRes);
    } else {
        sipIsErr = 1;
        std::string msg = "No Tulip import plugin named  ";
        msg += *a0;
        msg += ".";
        PyErr_SetString(PyExc_Exception, msg.c_str());
    }
%End

//===========================================================================================


bool exportGraph(tlp::Graph *graph, const std::string &outputFilePath, const std::string &exportPluginName, tlp::DataSet dataSet = tlp::DataSet());
%Docstring
tlp.exportGraph(graph, outputFilePath, exportPluginName[, dataSet])

Exports a graph to a file using a Tulip export plugin (must be loaded).
Export plugins are objects implementing the tlp::ExportModule interface
in C++ or the :class:`tlp.ExportModule` interface in Python.
The list of currently loaded export plugins can be
retrieved through the :func:`tlp.getExportPluginsList` function.

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
             able to call export modules (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End

%MethodCode
    if (pluginExists(*a2, "Export")) {
		std::ofstream ofs((*a1).c_str());
		if (ofs.is_open()) {
			sipRes = tlp::exportGraph(a0, ofs, *a2, *a3);
		} else {
			std::string msg = "Export error : The specified file path (";
			msg += *a1;
			msg += ") is not valid.";  
			printErrorMessage(msg);
			sipRes = false;
		}
		
	} else {
		sipIsErr = 1;
		std::string msg = "No Tulip export plugin named  ";
		msg += *a1;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End


//===========================================================================================

bool applyAlgorithm(tlp::Graph *graph, std::string &errMsg /Out/, tlp::DataSet *dataSet, const std::string &algoName);
%Docstring
tlp.applyAlgorithm(graph, dataSet, algoName)

.. deprecated:: 3.7 use :meth:`tlp.Graph.applyAlgorithm` instead

Applies an algorithm plugin (must be loaded) on a graph . Algorithm plugins are objects
implementing the tlp::Algorithm interface in C++ or the :class:`tlp.Algorithm` interface in Python. 
The list of currently loaded algorithm plugins can be
retrieved through the :func:`tlp.getAlgorithmPluginsList` function.
Parameters can be transmit to the algorithm trough a DataSet object (refer to the plugin 
documentation to get its parameters list). 

Returns a tuple whose first member is a boolean indicating if the algorithm has been successfully applied
and second member is a string that can contain an error message if the algorithms fails during
its execution.

:param graph: the graph on which to apply the algorithm
:type graph: :class:`tlp.Graph`
:param dataSet: a dataset filled with parameters to transmit to the algorithm
:type dataSet: :class:`tlp.DataSet`
:param algoName: the name of the algorithm plugin to call
:type algoName: string
:rtype: (boolean, string)
:throws: an exception if the requested algorithm plugin is not registered in the plugins database.

.. warning:: If you are using the bindings through the classical Python interpreter, Tulip plugins must be loaded in order to be
             able to call algorithms (see :ref:`Loading Tulip plugins <loading-plugins>`)).
%End

%MethodCode
    if (pluginExists(*a3, "Algorithm")) {
		sipRes = tlp::applyAlgorithm(a0, *a1, a2, *a3);
	} else {
		sipIsErr = 1;
        std::string msg = "No Tulip general algorithm plugin named  ";
		msg += *a3;
		msg += ".";
		PyErr_SetString(PyExc_Exception, msg.c_str());
	}	
%End


//===========================================================================================

tlp::Graph *newSubGraph(tlp::Graph *root, std::string name = "unnamed");
%Docstring
tlp.newSubGraph(root[, name = "unnamed"])

.. deprecated:: 3.7 use :meth:`tlp.Graph.addSubGraph` instead

Creates and returns an empty subgraph of a graph.

:param root: the graph on which the subgraph will be created
:type root: :class:`tlp.Graph`
:param name: the subgraph name
:type name: string
:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

tlp::Graph *newCloneSubGraph(tlp::Graph *root, std::string name = "unnamed");
%Docstring
tlp.newCloneSubGraph(root[, name = "unnamed"])

.. deprecated:: 3.7 use :meth:`tlp.Graph.addCloneSubGraph` instead

Creates and returns a subgraph of a graph root that is equal to that graph (a clone subgraph).

:param root: the graph on which the subgraph will be created
:type root: :class:`tlp.Graph`
:param name: the subgraph name
:type name: string
:rtype: :class:`tlp.Graph`
%End

//===========================================================================================

bool getSource(tlp::Graph *graph, tlp::node &n /Out/);
%Docstring
tlp.getSource(graph)

.. deprecated:: 3.7 use :meth:`tlp.Graph.getSource` instead

Finds the first node whose input degree equals 0.
Returns a tuple whose first member is a boolean indicating if such node exists and
second member is the found node (invalid if there is no source);

:param graph: the graph on which to find a source
:type graph: :class:`tlp.Graph`
:rtype: (boolean, :class:`tlp.node`)
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

};

