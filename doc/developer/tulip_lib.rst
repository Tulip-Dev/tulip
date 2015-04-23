.. _tulip_library:

*************
Tulip Library
*************

.. _tulip_library_intro:

Introduction
============

Efficient visualization of graphs and their usage for data analysis implies

* the manipulation of the structure of the graph
* the extraction of parts of it
* association of values to a graph's elements (nodes and edges)
* the computation of intrinsic parameters (parameters derived from a graph's structure, e.g. in a filesystem graph (directory tree): number of files in a directory, can be computed by counting outgoing edges)
* the computation of extrinsic parameters (parameters derived from external information, e.g. in a filesystem graph: file size)

This chapter describes the Tulip data structure that takes into account all the requirements
of a graph visualization system. For each part we describe the general principle and
then we give examples explaining how to do it with the Tulip library.


.. _tulip_library_graphs:

Graphs
======

The core of the Tulip library provides an programming interface for the manipulation of graphs. It enables to access and modify the structure of a graph. The aim of this library is to be as general as possible and thus it manipulates a general class of graphs with directed edges (an edge u → v is distinct from an edge v→u), allowing loops (loop = an edge with the same node as source and target) and multiple edges (it can exist more than one edge between two nodes). In mathematics, and more specifically in graph theory, this class of graphs is often called directed multigraph or pseudograph.

Because there can be more than one edge u→v, it is not possible to distinguish two edges using only source and target (u,v). To make this possible, all the elements in Tulip are entities (C++ objects). Thus, even if two edges have the same source and the same target, they are distinct.    

The elements of a graph are encapsulated in the graph. It is therefore not possible to access the graph's structure through elements, all operations must be done by querying the graph. For example, to know the source of an edge e of graph G, one must ask G, not e, what e's source is. This makes the use of the library less intuitive, but it minimizes memory usage for entities and allows to share them between subgraphs. Building a container of elements is cheap, because to handle elements, Tulip uses small objects which use the same amount of storage as integers.

The library supports access and modification of the graph structure. The access to the structure are made by using iterators, one very important point is that the iterator are not persistent. Thus, if one modify the graph structure all the iterators on the graph structure can be invalid. This property enables to prevent from cloning the data structure and thus enables better access to it. For ease of use, Tulip includes mechanism that enables to transform an iterator into stable iterator, one must keep in mind that it corresponds to clone the data structure and thus, it should be use only if it is necessary.

If one uses Tulip only for the manipulation of one graph (no graph hierarchy), the list of available operations on the graph is given afterward. In the next section we will present the set of operations that can be preformed in order to manage a hierarchy of subgraphs.


List of main modification operations
-----------------------------------------

* *node addNode()*: creates a new node in the graph and returns its identifier.

* *edge addEdge(node,node)*: create a new edge in the graph, given the source and target.

* *void delNode(node)* : deletes the given node.

* *void delEdge(edge)* : deletes the given edge.

* *void reverse(edge)* : reverses an edge (swaps source and target).


List of main access operations
------------------------------

* *unsigned int deg(node)* : returns the degree of a node (number of edges).

* *unsigned int indeg(node)* : returns the in degree of a node (number of times it is a target).

* *unsigned int outdeg(node)* : returns the out degree of a node (number of times it is a source).

* *node source(edge)* : returns the source of an edge.

* *node target(edge)* : returns the target of an edge.

* *node opposite(edge,node)* : it enables to obtain the opposite of a node of an edge.

* *Iterator * getInNodes(node)* : returns an iterator on the predecessor nodes of a node.

* *Iterator * getOutNodes(node)* : returns an iterator on the successor nodes of a node.

* *Iterator * getInOutNodes(node)* : returns an iterator on the neighbor nodes of a node.

* *Iterator * getInEdges(node)* : returns an iterator on the predecessor edges of a node.

* *Iterator * getOutEdges(node)* : returns an iterator on the successor edges of a node.

* *Iterator * getInOutEdges(node)* : returns an iterator on the neighbor edges of a node.


.. _tulip_library_hierarchy:

Hierarchy of graphs
===================

The Tulip library can also manage subgraphs. By definition a subgraph G' of a graph G is part (subset) of the elements of G such that G' is also a graph (all sources and targets of the edges of G' must be in G'). As a subgraph is also a graph it can itself contain subgraphs. In such a hierarchy, if a graph G" is a descendant of a graph G, G" is also a subgraph of G.

One of the strong point of Tulip is to ensure efficiently that all elements are shared between graphs in a hierarchy of graphs. Thus, if a node n is an element of a graph G and of a graph G', the entity n is the same in both graphs. Of course, the parameters of the entity can change between graphs. For instance, the degree of n can be smaller for a subgraph, as it can have less edges.

The subgraph relation in the hierarchy is preserved when one modifies a graph. This means that if one adds a node to a graph, this node is automatically added to all its ancestors as well. If one deletes a node, this node is automatically deleted from all the descendants of the graph. If one reverses an edge, this edge is reversed in all the graphs of the hierarchy.

In order to manipulate a hierarchy of graphs, more functions have been added to those introduced above. They provide navigation and modification for the hierarchy. The access to the hierarchy is provided by iterators, which are not persistent and thus, if the hierarchy is modified, the iterators are invalid.


List of main modification operations
------------------------------------

* *Graph *addSubGraph()* : returns an empty subgraph of this graph.
* *Graph *delSubGraph(Graph *)* : deletes a subgraph. Its descendants
    continue to be descendants of this graph.
* *Graph *delAllSubGraph(Graph *)* : deletes a subgraph and all its descendants.
* *edge addEdge(edge)* : adds an edge element from another graph in the hierarchy.
* *void addNode(node)* : adds a node element from another graph in the hierarchy.


List of main access operations
------------------------------

* *Iterator * getSubGraphs()* : returns an iterator on the subgraphs.
* *Graph * getSuperGraph()* : returns the parent of the graph. If the graph has no parent, it returns the graph itself.


.. _tulip_library_attributes:

Attributes
==========

An attributes is a kind of property that can be associated to a graph. An attributes has a name (a string) and a value of any type. It can be, for example ,the name of a graph, or a date of creation of the graph.  

Attributes can be added and accessed with those three following member functions :

* *const DataSet getAttributes()* : returns the attributes of a graph.
* *template<typename ATTRIBUTETYPE>bool getAttribute(const std::string &name, ATTRIBUTETYPE &value)* : get an attribute associated value.
* *template<typename ATTRIBUTETYPE>void setAttribute (const std::string &name, const ATTRIBUTETYPE &value)* : set an attribute associated value.


.. _tulip_library_properties:

Properties
==========

In Tulip, a property is an attribute of an element of a graph. It is called a property in order to prevent confusion with attributes of a graph: properties are for elements and attributes are for graphs. 

In Tulip, a property is always defined for both kinds of elements (nodes and edges), so one can always query for the value of the property associated with any edge or node.

To access the value of an elements one must query the graph for a property. This makes the use of the library less intuitive, but it minimizes memory usage for properties. 

A property can be seen as an associative table where you can set and get the value for every element. All property operations have a TYPE argument, so there is no need to cast the result of a property query. The standard operations of a property are:


List of main modification operations
------------------------------------

* *void setNodeValue(node,TYPE)* : sets the value of a node.
* *void setAllNodeValue(TYPE)* : sets the value of all nodes.
* *void setEdgeValue(edge,TYPE)* : sets the value of an edge.
* *void setAllEdgeValue(TYPE)* : sets the value of all edges.


List of main access operations
------------------------------

* *TYPE getNodeValue(node)* : returns the value of a node.
* *TYPE getEdgeValue(edge)* : returns the value of an edge.

For each property type there is a specific implementation (subclass) that allows operations which are specific to the property type (see Tulip libraries documentation). For instance, it is possible to obtain the maximum value of a property if the property type is *double*. 

A graph includes a set of functions that enables to obtain/create/delete a property. Because
the C++ signature of functions does not include the return type, the syntax for this call is not 
very simple. For instance, if one wants to obtain a property containing double (called DoubleProperty in Tulip) one must use the following syntax : *DoubleProperty *metric=graph->getProperty<DoubleProperty>("name of the property");*

In the graph each property is identified by its name which is a std::string, when one asks for a property the type of this property is checked using the run time type interrogation mechanism of C++. Warning: This test only happens when one compiles its sources in DEBUG mode (default mode). In order to ease the navigation/edition of the set of properties, a set of functions is accessible through the graph interface.


List of main operations
-----------------------

* *Iterator<std::string> * getLocalProperties()* : returns an iterator on all local properties of this graph.
* *void delLocalProperty(const std::string&)* : deletes a property.
* *bool existLocalProperty(const std::string&)* : returns true if a local property exists.
* *PropertyType * getLocalProperty (const std::string&)* : returns the property, or creates it if it does not exist.

For the property mechanism described above to work with a hierarchy of graphs, a mechanism have been added to share properties between graphs, which works like this: if a property exists in an ancestor of a graph G, it also exists in the graph G. Thus, properties of graphs are inherited like members of objects in object-oriented languages. In order to facilitate the navigation/edition of properties, a set of function is accessible through the graph interface.


List of available operations
----------------------------

* *Iterator<std::string> * getInheritedProperties()* : returns an iterator on all inherited properties.
* *Iterator<std::string> * getProperties()* : returns an iterator on all properties (both inherited and local).
* *bool existProperty(const std::string&)* : returns true if the property exists (inherited or local).
* *PropertyType * getProperty(const std::string&)* : returns the property (inherited or local) or creates it if does not exist.


