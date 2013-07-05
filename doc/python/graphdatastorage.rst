.. py:currentmodule:: tulip

Getting / Setting data on graph elements
========================================

Tulip allows to associate different kind of data to the graph elements. These data are stored
in a structure called "property" which can be seen as an array indexed by the nodes and edges of
a graph. The class :class:`tlp.PropertyInterface` is the base interface for each graph property object.
For each data type that can be attached to graph elements, there is a specific property class. 
Below is an exhaustive list of these classes:

	* :class:`tlp.BooleanProperty` : associate boolean values to nodes and edges
	* :class:`tlp.ColorProperty` : associate colors (r,g,b,a) to nodes and edges
	* :class:`tlp.DoubleProperty`: associate floating point values to nodes and edges
	* :class:`tlp.IntegerProperty`: associate integer values to nodes and edges
	* :class:`tlp.LayoutProperty`: associate positions (x,y,z) to nodes and bends/control points to edges
	* :class:`tlp.SizeProperty`: associate sizes (width,height,depth) to nodes and edges
	* :class:`tlp.StringProperty`: associate string values to nodes and edges
	
Lists of values can also be associated to graph elements through the use of the vector property classes listed below :
	
	* :class:`tlp.BooleanVectorProperty` : associate lists of boolean values to nodes and edges
	* :class:`tlp.ColorVectorProperty` : associate lists of colors (r,g,b,a) to nodes and edges
	* :class:`tlp.DoubleVectorProperty`: associate lists of floating point values to nodes and edges
	* :class:`tlp.IntegerVectorProperty`: associate lists of integer values to nodes and edges
	* :class:`tlp.CoordVectorProperty`: associate lists of positions (x,y,z) to nodes and to edges
	* :class:`tlp.SizeVectorProperty`: associate lists of sizes (width,height,depth) to nodes and edges
	* :class:`tlp.StringVectorProperty`: associate lists of string values to nodes and edges

In Tulip, there is two different kinds of graph properties :

	* **Local property** : means that the property belongs to the graph. Its ancestors in the sub-graph hierarchy do not have access to it but its descendants do.
	* **Inherited property** : means that the property is inherited from an ancestor in the sub-graph hierarchy. Only the graph whose the property is local can delete it.
	
Creating / Accessing / Deleting a graph property
------------------------------------------------

Before using a graph property, you have to create it. To do so, use specific methods from the :class:`tlp.Graph` class like :meth:`tlp.Graph.getDoubleProperty`,
:meth:`tlp.Graph.getLayoutProperty`, :meth:`tlp.Graph.getIntegerVectorProperty`, ... (see :ref:`Access and compute graph properties <graph-class.properties>`).
All these methods take a string identifier as parameter to name the property. The created property can then be accessed / deleted through its identifier. 
It is also possible to create anonymous properties that won't be managed by a graph. Below is
some code samples that illustrate these features::

  # create a DoubleProperty called "myMetric" if it does not exist
  # otherwise return the already created property
  myMetric = graph.getDoubleProperty("myMetric")

  # Once created, a property can also be accessed through this syntax
  myMetric = graph["myMetric"]

  # The property can be deleted by the graph that has created it
  graph.delLocalProperty("myMetric")

  # Anonymous properties can also be instantiated the following way
  myOtherMetric = tlp.DoubleProperty(graph)	


Working with graph properties
-----------------------------

The sample code below illustrate how to set / get data on graph elements through the use of a graph property::

  # creating an integer property called "degree"
  degree = graph.getIntegerProperty("degree")

  # creating a boolean property called "loop"
  loop = graph.getBooleanProperty("loop")

  # filling the properties
  for n in graph.getNodes():
    degree.setNodeValue(n, graph.deg(n))
    # the syntax below can also be used
    # degree[n] = graph.deg(n)

  for e in graph.getEdges():
    loop.setEdgeValue(e, graph.source(e) == graph.target(e))
    # the syntax below can also be used
    # loop[e] = (graph.source(e) == graph.target(e))

  # reading the properties
  for n in graph.getNodes():
    d = degree.getNodeValue(n)
    # the syntax below can also be used
    # d = degree[n]
    print "degree of ", n, " = ", d

  for e in graph.getEdges():
    isLoop = loop.getEdgeValue(e)
    # the syntax below can also be used
    # isLoop = loop[e]
    if isLoop:
      print e, " is a loop"

