Module description
==================

Graphs play an important role in many research areas, such as biology, microelectronics, social
sciences, data mining, and computer science. Tulip (http://tulip.labri.fr) [1]_ is an
Information Visualization framework dedicated to the analysis and visualization of such relational
data. Written in C++ the framework enables the development of algorithms, visual encodings,
interaction techniques, data models, and domain-specific visualizations.

The Tulip core library is available to the Python community through the Tulip-Python
bindings [2]_. The bindings have been developed using the `SIP <https://pypi.python.org/pypi/SIP>`_ 
tool [3]_ from Riverbank Computed Limited, allowing to easily create quality Python bindings for any C/C++ library.  
The main features provided by the bindings are the following ones:

	- **Creation and manipulation of graphs** :
	  Tulip provides an efficient graph data structure for storing large and complex networks. 
	  It is also one of the few that offer the possibility to 
	  efficiently define and navigate graph hierarchies or cluster trees (nested sub-graphs).

	- **Storage of data on graph elements** :
	  Tulip allows to associate different kind of serializable data (boolean, integer, float, string, ...) 
	  and visual attributes (layout, color, size, ...) to graph elements. All these data can be easily accessed 
	  from the Tulip graph data structure facilitating the development of algorithms.

	- **Application of algorithms of different types on graph** :
	  Tulip has been designed to be easily extensible and provides a variety of graph algorithms (layout, metric, clustering, ...)
	  implemented as C++ plugins. All these algorithms can be called from Python.
	  As Tulip is dedicated to graph visualization, it is provided with numerous state of the art
	  graph layout algorithms but also a bridge to the Open Graph Drawing Framework (http://www.ogdf.net) [4]_

Release notes
==============

Some informations regarding the Tulip-Python releases pushed on the Python Packaging Index:

    * **4.8.0.post1** :

        * Fix crash on MacOS when calling OGDF layout algorithms
        * minor fixes

    * **4.8.0** : Initial release based on Tulip 4.8
	  
Example
=======

The following script imports the dependency graph from the locally installed pip packages, 
draws it using a force directed layout algorithm and serializes the resulting graph to a file
through the TLP graph format. The imported graph can then be visualized through the Tulip software
or the use of the dedicated `tulipgui <https://pypi.python.org/pypi/tulipgui-python>`_ module, 
enabling to create the OpenGL visualizations available in Tulip from Python.

.. code:: python

  import pip
  import re

  from tulip import *
	
  # create a new empty graph
  graph = tlp.newGraph()

  # dictionnary mapping package name to graph node
  packageNode = {}
	
  # iterate over locally installed pip packages
  for d in pip.get_installed_distributions():
    # add a node associated to the package
    n = graph.addNode()
    packageNode[d.key] = n
    # set node label for use with Tulip visualizations components 
    graph['viewLabel'][n] = d.key

  # iterate over locally installed pip packages
  for d in pip.get_installed_distributions():
    # iterate over package requirements
    for r in d.requires():
      # process requirement name to get its pip package name : 
      # switch to lower case and remove version infos if any
      s = str(r).lower()
      match = re.search('|'.join(map(re.escape, '<=>')), s)
      if match:
        s = s[:match.start()]
      # add an edge between the pip package and its dependency in the graph
      graph.addEdge(packageNode[d.key], packageNode[s])

  # apply a force directed layout algorithm on the graph then a connected component packing algorithm.
  # algorithms are called with their default parameters.
  # resulting layout will be stored in the defaut graph layout property named 'viewLayout'
  graph.applyLayoutAlgorithm('Fast Multipole Multilevel Embedder (OGDF)')
  graph.applyLayoutAlgorithm('Connected Component Packing (Polyomino)')
	
  # serializes the graph to a file using the TLP graph format, 
  # that file can then be opened with the Tulip software for visualization purposes.
  tlp.saveGraph(graph, 'pip_deps.tlp')

References
==========
	  
.. [1] David Auber, Daniel Archambault, Romain Bourqui, Antoine Lambert, Morgan Mathiaut,
       Patrick Mary, Maylis Delest, Jonathan Dubois, and Guy Mélançon. The Tulip 3 Framework:
       A Scalable Software Library for Information Visualization Applications Based on Relational
       Data. Technical report RR-7860, INRIA, January 2012
	   
.. [2] Antoine Lambert and David Auber. Graph analysis and visualization with Tulip-Python. 
	   EuroSciPy 2012 - 5th European meeting on Python in Science, Bruxelles
	   
.. [3] Riverbank Computing Limited. SIP - a tool for automatically generating Python bindings for
       C and C++ libraries. http://www.riverbankcomputing.co.uk/software/sip
	   
.. [4] M. Chimani, C. Gutwenger, M. Jünger, K. Klein, P. Mutzel, and M. Schulz. The Open Graph
       Drawing Framework. 15th International Symposium on Graph Drawing 2007, Sydney (GD07), 2007.
