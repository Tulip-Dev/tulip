.. py:currentmodule:: tulip

Creating and manipulating graphs
================================

Creating a new graph
--------------------

To create a new Tulip graph from Python, the :func:`tlp.newGraph()` function has to be used. 
It returns an empty graph which is an instance of the :class:`tlp.Graph` class. ::

	graph = tlp.newGraph()

Importing / exporting a graph using the TLP file format
--------------------------------------------------------

TLP is the Tulip format to save a graph and its associated data to a file. The extension of
the file can be .tlp or .tlp.gz (compressed version). 

Loading a graph from a TLP file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To load a graph saved in TLP into a Python oject, the :func:`tlp.loadGraph` function has to be used. It loads 
the graph described in the file and its associated data and returns an instance of the :class:`tlp.Graph`.
For instance, the sample code below imports the graph saved to the file /home/foo/savedgraph.tlp::
	
	graph = tlp.loadGraph("/home/foo/savedgraph.tlp")

Saving a graph to a TLP file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To save an instance of a :class:`tlp.Graph` class to a TLP file, the :func:`tlp.saveGraph` function has to be used. It saves 
the graph and its associated data into a file.
For instance, the sample code below exports a graph to the file /home/foo/mygraph.tlp::

	tlp.saveGraph(graph, "/home/foo/mygraph.tlp")

Adding / Removing elements in the graph
----------------------------------------

The graph structure can be freely modified through dedicated methods of the :class:`tlp.Graph` class (see :ref:`Modification of the graph structure <graph-class.structure-modif>`) . Below is a list of some of these methods :

  * Adding elements to the graph : :meth:`tlp.Graph.addNode`, :meth:`tlp.Graph.addEdge`
  * Removing elements from the graph : :meth:`tlp.Graph.delNode`, :meth:`tlp.Graph.delEdge`, :meth:`tlp.Graph.clear`

Iterating over graph elements
------------------------------

The :class:`tlp.Graph` class provides several iterators on the graph structure (see :ref:`Iterators on the graph structure <graph-class.iterators>`). 
The sample code below illustrates the syntax to perform an iteration over all the nodes of a graph.::

      for n in graph.getNodes():
	degree = graph.deg(n)
	print "the degree of ", n, "is ", degree



