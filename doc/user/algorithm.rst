.. _algorithm:

**********
Algorithms
**********

.. _algo:

Algorithms can modify the layout, the set of selected elements, the size and color of elements, or compute a metric. As explained in the :ref:`quick presentation of the GUI <algo_window>`, algorithms are accessed through the *Algorithm* menu, located in the left panel.

The standard Tulip tool contains a lot of frequently updated and improved algorithms. With its plugin manager system, you can also search among databases to import or even implement your own algorithms and integrate them in Tulip.


.. _algo_algorithm:

Algorithm
=========

You will find in this menu algorithms using and influencing the graph structure.

* Clustering:

  These operations gives you the possibility to partition your graph and to regroup the elements by similarity, creating clusters.

* Topological test:

  It exists several specific graph topologies, some allowing certain properties or algorithms to be computed easier. The basic tests can be found in this sub-menu.

* Topology update:

  If your graph does not comply to the exigences of a distinctive topology, you will find here some functions to transform and adapt it.

* Triangulation:

???


.. _algo_color:

Coloring
========

This function realizes a color mapping on the nodes or the edges colour. The property on which the colour scale have to be applied can be specified. Further modularity is achieved as the user is free to select a colour scale among several presets or to create his own. The color scale can also use different gradients, from a smooth progression to a more discontinued ramp.


.. _algo_label:

Labeling
========

You can use this operation to transfer the values in one of the properties into the *viewLabel*, allowing a display of the values in the *Node Link Diagram view*. This action can also be realized in the Spreadsheet view.


.. _algo_layout:

Layout
======

Tulip allows the visualization of informations, and thus, proposes several layout algorithms to display information and data in a neat fashion.

* Basic:

  The standard functions can be found in this sub-menu such as the *Circular* display or the *Random layout*.

* Force Directed:

  These layouts will try to place nodes so that the distance in the graph (specific metric on the edges) should be the closest to the distance on the drawing. Known such algorithm are the FM^3, the GEM Frick and the Kameda Kawai.

* Hierarchical:

  Those representation, in accord to their name, are perfect for presenting hierarchical structures or graph showing precedence relations. The Balloon and the Sugiyama algorithms are good example of such layout.

* Misc:

  This sub-menu contains miscellaneous functions, notably the packing and overlap removal algorithms.

* Multilevel:

  Multilevel layout is computed by including gradually the initial nodes into a layout, thus iteratively improving the node placement. The MMM and the fast multipole layout follow these steps. The iterativity allows to gradually enhance the representation.

* Planar:

  These algorithms are specialized in generating aesthetic plannar layouts. With minimized edge crossings, those representations offer understandable 2D visualizations.

* Tree:

  According to the sub-menu name, these layouts are particularly adapted to represent trees or hierarchical data. 


.. _algo_measure:

Measure
=======

Measure algorithms are used to compute different metrics (on edges or nodes). The computed values are assigned by default to the viewMetric property.

* Clustering

  One can use those tools to compute useful informations to identify clusters and linked elements.
 
* Component

  The algorithms here are jsut what you need if you want to find and decompose your graph in connected components.

* Graph

  The measures computed in this subsection concern the graph structure. Among the functions available you may find: the betweenness centrality, the degree, the eccentricity or even the K-Cores computation often used in social network analysis.

* Hierarchical

  Those algorithms are specific for tree and hierarchical representations. You will find algorithms allowing to compute the depth, the induced number of nodes or leaves, the dag level...

* Misc

  This subsection gathers all the miscallaneous computations such as an identity mapping, putting the *id* of the nodes and edges into another property, or a random metric generation, affecting random values to each element choose property.

.. _algo_size:

Resizing
========

Tulip provides two resizing algorithms. Those tools can be used to modify the *viewSize* property (by default) according to some parameters and another property. Several additionnal options allows you to control the visualization transformations through the minimum and maximum size allowed, the type of element to modify (edge or node) or the proportion to respect.


.. _algo_select:

Selection
=========

Those algorithms simply select a subset of the nodes and edges (i.e. they modify the viewSelection property) according to a certain fashion. You can use them to select loops, minimum spanning tree, dag or forest, connected components...
 
