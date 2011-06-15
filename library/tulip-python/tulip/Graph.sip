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
namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;
};
%End

namespace tlp {
typedef tlp::Iterator<std::string> IteratorString;
typedef tlp::Graph* GraphPointer;
typedef tlp::Iterator<tlp::GraphPointer> IteratorGraph;

enum ElementType {NODE=0, EDGE};

class Graph : tlp::Observable /Abstract/ {
%TypeHeaderCode
#include <tulip/Graph.h>
%End

public:

	Graph();
	
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

	tlp::Graph *inducedSubGraph(const std::set<tlp::node>& nodeSet);
%Docstring
tlp.Graph.inducedSubGraph(nodes)

Creates and returns a new sub-graph of the graph induced by a set of nodes. 
The sub-graph contains all the nodes of the set and all the existing edges 
between two nodes of the set including self-loops.

:param nodes: the set of nodes from which to build the induced sub-graph
:type nodes: list of :class:`tlp.node`
:rtype: :class:`tlp.Graph` 
%End

//===========================================================================================

	void delSubGraph(tlp::Graph *);
%Docstring
tlp.Graph.delSubGraph(subgraph)

Delete a sub-graph of this graph. The sub-graph's sub-graphs become sub-graphs of the graph.

:param subgraph: the sub-graph to remove
:type subgraph: :class:`tlp.Graph`
%End

//===========================================================================================

	void delAllSubGraphs(tlp::Graph *);
%Docstring
tlp.Graph.delAllSubGraphs(subgraph)

Delete a sub-graph of this graph and all its sub-graphs.

:param subgraph: the sub-graph to remove
:type subgraph: :class:`tlp.Graph` 
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

	void setSuperGraph(tlp::Graph *);
%Docstring
tlp.Graph.setSuperGraph(superGraph)

Sets the parent of the graph (use very carefully). Standard users should never use this method.

:param superGraph: the new parent for the graph in the hierarchy.
:type superGraph: :class:`tlp.Graph`

%End

//===========================================================================================
	
	tlp::IteratorGraph *getSubGraphs() const;
%Docstring
tlp.Graph.getSubGraphs()

Returns an iterator on all the sub-graphs of the graph.

:rtype: :class:`tlp.IteratorGraph` 
%End
	
%MethodCode
	sipRes = new tlp::StableIterator<tlp::Graph *>(sipCpp->getSubGraphs());
%End		

//===========================================================================================

	bool isSubGraph(tlp::Graph* sg) const;
%Docstring
tlp.Graph.isSubGraph(graph)

Returns True if the graph argument is a direct sub-graph of the graph.

:param graph: a graph
:type graph: :class:`tlp.Graph`
:rtype: boolean 
%End

//===========================================================================================

	bool isDescendantGraph(tlp::Graph* sg) const;
%Docstring
tlp.Graph.isDescendantGraph(graph)

Returns True if the graph argument is a descendant of this graph.

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

	tlp::Graph* getDescendantGraph(unsigned int id) const;
%Docstring
tlp.Graph.getDescendantGraph(id)

Returns the descendant graph with the corresponding id or :const:`None` if there is no descendant with that id. 

:param id: a graph id
:type id: integer
:rtype: :class:`tlp.Graph` or :const:`None` 
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

	void addNode(const tlp::node);
// The docstring is commented because Sphinx does not handle method overloading correctly when using autodoc
// The documentation of the method has been added manually to the Sphinx build files
	
%Docstring
tlp.Graph.addNode(node)

Adds an existing node in the graph. This node is also added in all the graph ancestors 
to maintain the sub-graph relation between graphs. Warning, the node must be an element of the 
graph hierarchy, thus it must be an element of the root graph. Warning : One can't add an existing node to the root graph.

:param node: an existing node to add to the graph
:type node: :class:`tlp.node`
:throws: an exception if the node does not belong to the root graph
%End

		
%MethodCode
	if (sipCpp->getRoot()->isElement(*a0)) {
		sipCpp->addNode(*a0);
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp->getRoot(), *a0);
	}
%End	

//===========================================================================================

	void delNode(const tlp::node, bool deleteInAllGraphs = false);
%Docstring
tlp.Graph.delNode(node[, deleteInAllGraphs = False])

Deletes a node in the graph. This node is also removed in all the sub-graphs of the graph 
to maintain the sub-graph relation between graphs. 

:param node: the node to delete
:type node: :class:`tlp.node` 
:param deleteInAllGraphs: if True, remove the node in all the hierarchy of graphs
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

	tlp::edge addEdge(const tlp::node, const tlp::node );
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

	void addEdge(const tlp::edge);
// The docstring is commented because Sphinx does not handle method overloading correctly when using autodoc
// The documentation of the method has been added manually to the Sphinx build files
	
%Docstring
tlp.Graph.addEdge(edge)

Adds an existing edge in the graph. This edge is also added in all 
the graph's ancestors to maintain the sub-graph relation between graphs. 
Warning, the edge must be an element of the graph hierarchy, thus it must be 
an element of the root graph. 
Warning : One can't add an existing edge to the root graph

:param edge: an existing edge to add to the graph
:type edge: :class:`tlp.edge`
:throws: an exception if the edge does not belong to the root graph 
%End	
	
%MethodCode
	if (sipCpp->getRoot()->isElement(*a0)) {
		sipCpp->addEdge(*a0);
	} else {
		sipIsErr = throwInvalidEdgeException(sipCpp->getRoot(), *a0);
	}
%End

//===========================================================================================

	 void delEdge(const tlp::edge, bool deleteInAllGraphs = false);
%Docstring
tlp.Graph.delEdge(edge[, deleteInAllGraphs = False])

Deletes an edge in the graph. This edge is also removed in all the sub-graphs 
of the graph to maintain the sub-graph relation between graphs. The ordering of edges is preserved. 

:param edge: the edge to delete
:type edge: :class:`tlp.edge` 
:param deleteInAllGraphs: if True, remove the edge in all the hierarchy of graphs
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

	void setEdgeOrder(const tlp::node,const std::vector<tlp::edge> &);
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

	void swapEdgeOrder(const tlp::node,const tlp::edge , const tlp::edge );
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

void setSource(const tlp::edge, const tlp::node);
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
  
void setTarget(const tlp::edge, const tlp::node);
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
  
void setEnds(const tlp::edge, const tlp::node source, const tlp::node target);
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

	void reverse(const tlp::edge);
%Docstring
tlp.Graph.reverse(edge)

Reverses the direction of an edge, the source becomes the target 
and the target becomes the source. 
Warning : The ordering is global to the entire graph hierarchy. 
Thus, by changing the ordering in a graph you change the ordering in the hierarchy.

:param edge: the edge on which to revert the direction.
:type edge: :class:`tlp.edge` 
:throws: an exception if the edge does not belong to the graph
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

	tlp::Iterator<tlp::node>* getNodes() const;
%Docstring
tlp.Graph.getNodes()

Returns an iterator on the nodes.

:rtype: :class:`tlp.IteratorNode` 
%End	
	
%MethodCode
	sipRes = new tlp::StableIterator<tlp::node>(sipCpp->getNodes());
%End	

//===========================================================================================

	tlp::node getInNode(const tlp::node,unsigned int ) const;
%Docstring
tlp.Graph.getInNode(node, i)

Returns the ith predecessor of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:param i: the position in the predecessor nodes list
:type i: integer
:throws: an exception if the node does not belong to the graph or if the requested index is lesser than the number of predecessor nodes
%End	
	
	 
%MethodCode
	if (sipCpp->isElement(*a0)) {
		if (a1 < sipCpp->indeg(*a0)) {
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

	tlp::Iterator<tlp::node>* getInNodes(const tlp::node) const;
%Docstring
tlp.Graph.getInNodes(node)

Return an iterator on the predecessors of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: :class:`tlp.IteratorNode`
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

	tlp::node getOutNode(const tlp::node,unsigned int) const ;
%Docstring
tlp.Graph.getOutNode(node, i)

Returns the ith successor of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:param i: the position in the successor nodes list
:type i: integer
:throws: an exception if the node does not belong to the graph or if the requested index is lesser than the number of successor nodes
%End	
	
%MethodCode
	if (sipCpp->isElement(*a0)) {
		sipRes = new tlp::node(sipCpp->getOutNode(*a0, a1));
	} else {
		sipIsErr = throwInvalidNodeException(sipCpp, *a0);
	}
%End

//===========================================================================================

	tlp::Iterator<tlp::node>* getOutNodes(const tlp::node) const;
%Docstring
tlp.Graph.getOutNodes(node)

Returns an iterator on the successors of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: :class:`tlp.IteratorNode`
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

	tlp::Iterator<tlp::node>* getInOutNodes(const tlp::node) const;
%Docstring
tlp.Graph.getInOutNodes(node)

Returns an iterator on the neighbours of a node. 

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: :class:`tlp.IteratorNode`
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

	tlp::Graph* getNodeMetaInfo(const tlp::node) const;
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

	tlp::Iterator<tlp::edge>* getEdges() const;
%Docstring
tlp.Graph.getEdges()

Returns an iterator on the edges.

:rtype: :class:`tlp.IteratorEdge` 
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

	tlp::Iterator<tlp::edge>* getOutEdges(const tlp::node) const;
%Docstring
tlp.Graph.getOutEdges(node)

Returns an iterator on the outgoing edges of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: :class:`tlp.IteratorEdge`
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

	tlp::Iterator<tlp::edge>* getInOutEdges(const tlp::node) const;
%Docstring
tlp.Graph.getInOutEdges(node)

Returns an iterator on the incoming and outgoing edges of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: :class:`tlp.IteratorEdge`
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

	tlp::Iterator<tlp::edge>* getInEdges(const tlp::node) const;
%Docstring
tlp.Graph.getInEdges(node)

Returns an iterator on the incoming edges of a node.

:param node: an existing node of the graph
:type node: :class:`tlp.node`
:rtype: :class:`tlp.IteratorEdge`
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

	tlp::Iterator<tlp::edge>* getEdgeMetaInfo(const tlp::edge) const;
%Docstring
tlp.Graph.getEdgeMetaInfo(edge)

Returns an iterator on the underlying edges of a meta edge.

:param edge: an existing edge of the graph
:type edge: :class:`tlp.edge`
:rtype: :class:`tlp.IteratorEdge`
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

	unsigned int numberOfNodes()const ;
%Docstring
tlp.Graph.numberOfNodes()

Returns the number of nodes in the graph.

:rtype: integer
%End

//===========================================================================================

	unsigned int numberOfEdges()const ;
%Docstring
tlp.Graph.numberOfEdges()

Returns the number of edges in the graph.

:rtype: integer
%End


//===========================================================================================

	unsigned int deg(const tlp::node)const ;
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

	unsigned int indeg(const tlp::node) const ;
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

	unsigned int outdeg(const tlp::node)const ;
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

	tlp::node source(const tlp::edge)const ;
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

	tlp::node target(const tlp::edge)const ;
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

	const std::pair<tlp::node, tlp::node>& ends(const tlp::edge) const;
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

	tlp::node opposite(const tlp::edge, const tlp::node) const ;
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

	bool isElement(const tlp::node) const ;
%Docstring
tlp.Graph.isElement(node)

Returns true if the node is an element of the graph.

:param node: a node
:type node: :class:`tlp.node`
:rtype: boolean
%End

//===========================================================================================

	bool isElement(const tlp::edge) const ;
%Docstring
tlp.Graph.isElement(edge)

Returns true if the edge is an element of the graph.

:param edge: an edge
:type edge: :class:`tlp.edge`
:rtype: boolean
%End

//===========================================================================================

	bool isMetaNode(const tlp::node) const ;
%Docstring
tlp.Graph.isMetaNode(node)

Returns true if the node is a meta-node.

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

	bool isMetaEdge(const tlp::edge) const ;
%Docstring
tlp.Graph.isMetaEdge(edge)

Returns true if the edge is a meta-edge.

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

	tlp::edge existEdge(const tlp::node , const tlp::node, bool directed=true) const ;
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

Returns True if a property of that name exists in the graph properties pool or in the pool of an ancestor in the sub-graphs hierarchy.

:param name: the name of the property
:type name: string
:rtype: boolean
%End

//===========================================================================================

	bool existLocalProperty(const std::string& name);
%Docstring
tlp.Graph.existLocalProperty(name)

Returns True if a property of that name exists in the graph properties pool.

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

//===========================================================================================

	tlp::Iterator<std::string>* getLocalProperties();
%Docstring
tlp.Graph.getLocalProperties()

Returns an iterator on the names of the properties local to the graph.

:rtype: tlp.IteratorString
%End	
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getLocalProperties());
%End		

//===========================================================================================

	tlp::Iterator<std::string>* getInheritedProperties();
%Docstring
tlp.Graph.getInheritedProperties()

Returns an iterator on the names of the properties inherited from the graph's ancestors.

:rtype: tlp.IteratorString
%End
	
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getInheritedProperties());
%End		

//===========================================================================================

	tlp::Iterator<std::string>* getProperties();
%Docstring
tlp.Graph.getInheritedProperties()

Returns an iterator on the names of all the properties attached to the graph.

:rtype: tlp.IteratorString
%End
	
%MethodCode
	sipRes = new tlp::StableIterator<std::string>(sipCpp->getProperties());
%End			
	
//===========================================================================================	
	
	bool computeIntegerProperty(const std::string &algorithm, tlp::IntegerProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeIntegerProperty(algoName, result[, dataSet = None])

Computes an integer property on the current graph 
using an external named integer algorithm (plugin).
The computed values will be stored in result. 
Warning: previous values stored in result will be deleted.
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
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
%End	

%MethodCode
	sipRes = sipCpp->computeProperty<tlp::IntegerProperty>(*a0, a1, *a2, NULL, a3);
%End

//===========================================================================================

	bool computeDoubleProperty(const std::string &algorithm, tlp::DoubleProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeDoubleProperty(algoName, result[, dataSet = None])

Computes a double property on the current graph 
using an external named double algorithm (plugin).
The computed values will be stored in result. 
Warning: previous values stored in result will be deleted.
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
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
%End
	
%MethodCode
	sipRes = sipCpp->computeProperty<tlp::DoubleProperty>(*a0, a1, *a2, NULL, a3);
%End

//===========================================================================================

	bool computeLayoutProperty(const std::string &algorithm, tlp::LayoutProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeLayoutProperty(algoName, result[, dataSet = None])

Computes a layout property on the current graph 
using an external named layout algorithm (plugin).
The computed values will be stored in result. 
Warning: previous values stored in result will be deleted.
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
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
%End
	
%MethodCode
	sipRes = sipCpp->computeProperty<tlp::LayoutProperty>(*a0, a1, *a2, NULL, a3);
%End

//===========================================================================================

	bool computeBooleanProperty(const std::string &algorithm, tlp::BooleanProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeBooleanProperty(algoName, result[, dataSet = None])

Computes a boolean property on the current graph 
using an external named boolean algorithm (plugin).
The computed values will be stored in result. 
Warning: previous values stored in result will be deleted.
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
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
%End
	
%MethodCode
	std::string s;
	sipRes = sipCpp->computeProperty<tlp::BooleanProperty>(*a0, a1, *a2, NULL, a3);
%End

//===========================================================================================

	bool computeSizeProperty(const std::string &algorithm, tlp::SizeProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeSizeProperty(algoName, result[, dataSet = None])

Computes a size property on the current graph 
using an external named size algorithm (plugin).
The computed values will be stored in result. 
Warning: previous values stored in result will be deleted.
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
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
%End
	
%MethodCode
	sipRes = sipCpp->computeProperty<tlp::SizeProperty>(*a0, a1, *a2, NULL, a3);
%End

//===========================================================================================

bool computeColorProperty(const std::string &algorithm, tlp::ColorProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeColorProperty(algoName, result[, dataSet = None])

Computes a color property on the current graph 
using an external named color algorithm (plugin).
The computed values will be stored in result. 
Warning: previous values stored in result will be deleted.
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
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
%End

%MethodCode
	sipRes = sipCpp->computeProperty<tlp::ColorProperty>(*a0, a1, *a2, NULL, a3);
%End

//===========================================================================================

bool computeStringProperty(const std::string &algorithm, tlp::StringProperty* result, std::string &msg /Out/, tlp::DataSet *data=0);
%Docstring
tlp.Graph.computeStringProperty(algoName, result[, dataSet = None])

Computes a string property on the current graph 
using an external named string algorithm (plugin).
The computed values will be stored in result. 
Warning: previous values stored in result will be deleted.
Parameters can be transmit to the algorithm
using a :class:`tlp.DataSet` . In some cases, algorithms 
can use this :class:`tlp.DataSet` in order to return some external information 
(not stored in result).
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
%End

%MethodCode
	sipRes = sipCpp->computeProperty<tlp::StringProperty>(*a0, a1, *a2, NULL, a3);
%End

//===========================================================================================
	
	void push(bool unpopAllowed = true);
%Docstring
tlp.Graph.push([unpopAllowed=True])

Marks the state of the current root graph in the hierarchy.
The next updates will be recorded in order to be undone at the
next call of the :meth:`tlp.Graph.pop` method. Be careful that all 
the updates are undone except those who affect the ordering of edges.

:param unpopAllowed: If set to False, the next updates could not be replayed after undone. If some previously undonevupdates exist they could no longer be replayed.
:type unpopAllowed: boolean
%End	
	
//===========================================================================================
	
	void pop(bool unpopAllowed = true);
%Docstring
tlp.Graph.pop([unpopAllowed=True])

Restores a previously marked state of the current root graph
in the hierarchy. The restored state does not remain marked.

:param unpopAllowed: If set to False, the undone updates could not be replayed.
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

Returns True if a previously marked state can be restored.

:rtype: boolean
%End
	
//===========================================================================================
	
	bool canUnpop();
%Docstring
tlp.Graph.canUnpop()

Returns True if some previously undone updates can be replayed.

:rtype: boolean
%End

//===========================================================================================
	
	bool canPopThenUnpop();
%Docstring
tlp.Graph.canUnpop()

Returns True if the current state updates can be undone then replayed.

:rtype: boolean
%End	

//===========================================================================================

	tlp::node createMetaNode(const std::set<tlp::node> &nodeSet, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(nodeSet[, multiEdges=True, delAllEdge=True])

Closes a set of existing nodes into a metanode and returns it.
Edges from nodes in the set to other nodes are replaced with
edges from the metanode to the other nodes.
Warning: this method will failed when called on the root graph.

:param nodeSet: a set of existing nodes
:type nodeSet: list of :class:`tlp.node`
:param multiEdges: indicates if a meta edge will be created for each underlying edge
:type multiEdges: boolean
:param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
:type delAllEdge: boolean
:rtype: :class:`tlp.node`
%End

//===========================================================================================	
		
	tlp::node createMetaNode(tlp::Graph* subGraph, bool multiEdges = true, bool delAllEdge = true);
%Docstring
tlp.Graph.createMetaNode(subgraph[, multiEdges=True, delAllEdge=True])

Closes an existing subgraph into a metanode and returns it.  Edges from nodes
in the subgraph to nodes outside the subgraph are replaced with
edges from the metanode to the nodes outside the subgraph.
Warning: this method will failed when called on the root graph.

:param subgraph: an existing subgraph
:type subgraph: :class:`tlp.Graph`
:param multiEdges: indicates if a meta edge will be created for each underlying edge
:type multiEdges: boolean
:param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
:type delAllEdge: boolean
:rtype: :class:`tlp.node`
%End

//===========================================================================================

	void createMetaNodes(tlp::IteratorGraph *itS, tlp::Graph *quotientGraph, std::vector<tlp::node>& metaNodes /Out/);
%Docstring
tlp.Graph.createMetaNodes(itGraph, quotientGraph)

Populates a quotient graph with one meta node for each iterated graph.
Returns a list of created meta-nodes.

:param itGraph: a graph iterator, (typically a subgraph iterator)
:type itGraph: :class:`tlp.IteratorGraph`
:param quotientGraph: the graph that will contain the meta nodes
:type quotientGraph: :class:`tlp.Graph`
:rtype: list of :class:`tlp.node`
%End
	
//===========================================================================================

	void openMetaNode(tlp::node n, bool updateProperties = true);
%Docstring
tlp.Graph.openMetaNode(metaNode[, updateProperties=True])

Opens a metanode and replaces all edges between that
meta node and other nodes in the graph.
Warning: this method will failed when called on the root graph.

:param metaNode: the meta-node to open
:type metaNode: :class:`tlp.node`
:param updateProperties: If True, open meta node will update inner nodes layout, color, size, etc...
:type updateProperties: boolean
%End
	
//===========================================================================================
	
	SIP_PYOBJECT __getitem__(const std::string &attributeName) const;
%MethodCode
        tlp::Iterator<std::string> *graphProperties = sipCpp->getProperties();
        bool found = false;
        while (graphProperties->hasNext()) {
        	std::string p = graphProperties->next();
        	if (p == *a0) {
        		tlp::PropertyInterface *prop = sipCpp->getProperty(p);
        		if (dynamic_cast<tlp::BooleanProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::BooleanProperty *>(prop), sipType_tlp_BooleanProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::BooleanVectorProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::BooleanVectorProperty *>(prop), sipType_tlp_BooleanVectorProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::ColorProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::ColorProperty *>(prop), sipType_tlp_ColorProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::ColorVectorProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::ColorVectorProperty *>(prop), sipType_tlp_ColorVectorProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::DoubleProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::DoubleProperty *>(prop), sipType_tlp_DoubleProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::DoubleVectorProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::DoubleVectorProperty *>(prop), sipType_tlp_DoubleVectorProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::IntegerProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::IntegerProperty *>(prop), sipType_tlp_IntegerProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::IntegerVectorProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::IntegerVectorProperty *>(prop), sipType_tlp_IntegerVectorProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::SizeProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::SizeProperty *>(prop), sipType_tlp_SizeProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::SizeVectorProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::SizeVectorProperty *>(prop), sipType_tlp_SizeVectorProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::StringProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::StringProperty *>(prop), sipType_tlp_StringProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::StringVectorProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::StringVectorProperty *>(prop), sipType_tlp_StringVectorProperty, NULL);
        			found = true;
        			break;
        		} if (dynamic_cast<tlp::LayoutProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::LayoutProperty *>(prop), sipType_tlp_LayoutProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::CoordVectorProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::CoordVectorProperty *>(prop), sipType_tlp_CoordVectorProperty, NULL);
        			found = true;
        			break;
        		} else if (dynamic_cast<tlp::GraphProperty *>(prop)) {
        			sipRes = sipConvertFromType(dynamic_cast<tlp::GraphProperty *>(prop), sipType_tlp_GraphProperty, NULL);
        			found = true;
        			break;
        		} 
        	}
        }
        delete graphProperties;
        if (!found) {
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
	
%MethodCode
	std::string graphName;
	sipCpp->getAttribute("name", graphName);
	sipRes = new std::string(graphName);
%End

//===========================================================================================

	void setName(const std::string &s) const;
%Docstring
tlp.Graph.setName(name)

Sets the name of the graph.

:param name: the new name of the graph
:type name: string
%End
	
%MethodCode
	sipCpp->setAttribute("name", *a0);
%End	
//===========================================================================================

void setAttribute(const std::string &name, SIP_PYOBJECT po);
%MethodCode
  int state=0, iserr=0;
  if (PyBool_Check(a1)) {
    long l = PyLong_AsLong(a1);
    sipCpp->setAttribute<bool>(*a0, (l > 0));
  } else if (PyInt_CheckExact(a1)) {
    sipCpp->setAttribute(*a0, PyInt_AsLong(a1)); 
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
  }
%End 

SIP_PYOBJECT getAttribute(const std::string& name);
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
    sipRes = PyString_FromString((*(reinterpret_cast<std::string*>(dataType->value))).c_str());
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::DataSet).name())) {
    ok = true;
    sipRes = sipConvertFromNewType(new tlp::DataSet(*(reinterpret_cast<tlp::DataSet *>(dataType->value))), sipType_tlp_DataSet, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::DataSet).name())) {
    ok = true;
    sipRes = sipConvertFromNewType(new tlp::DataSet(*(reinterpret_cast<tlp::DataSet *>(dataType->value))), sipType_tlp_DataSet, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::Coord).name())) {
    ok = true;
    sipRes = sipConvertFromNewType(new tlp::Coord(*(reinterpret_cast<tlp::Coord *>(dataType->value))), sipType_tlp_Coord, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::Color).name())) {
    ok = true;
    sipRes = sipConvertFromNewType(new tlp::Color(*(reinterpret_cast<tlp::Color *>(dataType->value))), sipType_tlp_Color, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::Size).name())) {
    ok = true;
    sipRes = sipConvertFromNewType(new tlp::Size(*(reinterpret_cast<tlp::Size *>(dataType->value))), sipType_tlp_Size, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::ColorScale).name())) {
    ok = true;
    sipRes = sipConvertFromNewType(new tlp::ColorScale(*(reinterpret_cast<tlp::ColorScale *>(dataType->value))), sipType_tlp_ColorScale, NULL);
    
  } else if (dataType->getTypeName() == std::string(typeid(tlp::StringCollection).name())) {
    ok = true;
    sipRes = sipConvertFromNewType(new tlp::StringCollection(*(reinterpret_cast<tlp::StringCollection *>(dataType->value))), sipType_tlp_StringCollection, NULL);
    
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
    
  }
}

if (!ok) {
  std::ostringstream oss;
  oss << "Data set entry \"" << *a0 << "\" does not exist.";
  sipRes = NULL;
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
        sipRes = PyString_FromString(s.c_str());
%End

//===========================================================================================

	long __hash__() const;
%MethodCode
        return static_cast<long>(sipCpp->getId());
%End

};

};

namespace tlp {

tlp::Graph *newGraph();
tlp::Graph *loadGraph(const std::string &filename);
bool saveGraph(tlp::Graph *, const std::string &filename);
tlp::Graph *importGraph(const std::string &alg, tlp::DataSet &dataSet, tlp::Graph *newGraph=0);
%MethodCode
	sipRes = tlp::importGraph(*a0, *a1, 0, a2);
%End

bool applyAlgorithm(tlp::Graph *graph, std::string &errMsg /Out/, tlp::DataSet *dataSet =0, const std::string &alg="any");

tlp::Graph *newSubGraph(tlp::Graph *, std::string name = "unnamed");
tlp::Graph *newCloneSubGraph(tlp::Graph *, std::string name = "unnamed");
bool getSource(tlp::Graph *, tlp::node &n);
void copyToGraph(tlp::Graph *outG, tlp::Graph *inG, tlp::BooleanProperty* inSelection=0, tlp::BooleanProperty* outSelection=0 );
void removeFromGraph(tlp::Graph *ioG, tlp::BooleanProperty *inSelection = 0);


};

