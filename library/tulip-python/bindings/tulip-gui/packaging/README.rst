Module description
==================

Graphs play an important role in many research areas, such as biology, microelectronics, social
sciences, data mining, and computer science. Tulip (http://tulip.labri.fr) [1]_ [2]_ is an
Information Visualization framework dedicated to the analysis and visualization of such relational
data. Written in C++ the framework enables the development of algorithms, visual encodings,
interaction techniques, data models, and domain-specific visualizations.

The Tulip GUI library is available to the Python community through the Tulip-Python
bindings [3]_ allowing to create and manipulate Tulip views (typically Node Link diagrams)
trough the tulipgui module. It has to be used with the `tulip <https://pypi.python.org/pypi/tulip-python>`_ module
dedicated to the creation, storage and manipulation of the graphs to visualize.
The bindings have been developed using the `SIP <https://pypi.python.org/pypi/SIP>`_
tool [4]_ from Riverbank Computed Limited, allowing to easily create quality Python bindings for any C/C++ library.

The main features provided by the bindings are the following ones:

    * creation of interactive Tulip visualizations (Adjacency Matrix, Geographic, Histogram,
      Node Link Diagram, Parallel Coordinates, Pixel Oriented, Scatter Plot, Self Organizing Map, Spreadsheet)
    * the ability to change the data source on opened visualizations
    * the possibilty to modify the rendering parameters for node link diagram visualizations
    * the ability to save visualization snapshots to image files on disk

Release notes
==============

Some information regarding the Tulip-Python releases pushed on the Python Packaging Index:

    * **5.0.0**: based on Tulip 5.0.0 released on 27/06/2017

    * **4.10.0**: based on Tulip 4.10.0 released on 08/12/2016

    * **4.9.0** : based on Tulip 4.9.0 released on 08/07/2016

    * **4.8.1** : based on Tulip 4.8.1 released on 16/02/2016

    * **4.8.0** : Initial release based on Tulip 4.8

Example
========

The following script imports the tree structure of the file system directory of the Python
standard library, applies colors to nodes according to degrees, computes a tree layout and quadratic
Bézier shapes for edges. The imported graph and its visual encoding are then visualized
by creating an interactive Node Link Diagram view.
A window containing an OpenGL visualization of the graph will be created and displayed.

.. code:: python

    from tulip import tlp
    from tulipgui import tlpgui

    import os

    # get the root directory of the Python Standard Libraries
    pythonStdLibPath = os.path.dirname(os.__file__)

    # call the 'File System Directory' import plugin from Tulip
    # importing the tree structure of a file system
    params = tlp.getDefaultPluginParameters('File System Directory')
    params['directory color'] = tlp.Color.Blue
    params['other color'] = tlp.Color.Red
    params['directory'] = pythonStdLibPath
    graph = tlp.importGraph('File System Directory', params)

    # compute an anonymous graph double property that will store node degrees
    degree = tlp.DoubleProperty(graph)
    degreeParams = tlp.getDefaultPluginParameters('Degree')
    graph.applyDoubleAlgorithm('Degree', degree, degreeParams)

    # create a heat map color scale
    heatMap = tlp.ColorScale([tlp.Color.Green, tlp.Color.Black, tlp.Color.Red])

    # linearly map node degrees to colors using the 'Color Mapping' plugin from Tulip
    # using the heat map color scale
    colorMappingParams = tlp.getDefaultPluginParameters('Color Mapping', graph)
    colorMappingParams['input property'] = degree
    colorMappingParams['color scale'] = heatMap
    graph.applyColorAlgorithm('Color Mapping', colorMappingParams)

    # apply the 'Bubble Tree' graph layout plugin from Tulip
    graph.applyLayoutAlgorithm('Bubble Tree')

    # compute quadratic bezier shapes for edges
    curveEdgeParams = tlp.getDefaultPluginParameters('Curve edges', graph)
    curveEdgeParams['curve type'] = 'QuadraticDiscrete'
    graph.applyAlgorithm('Curve edges', curveEdgeParams)

    # create a node link diagram view of the graph,
    # a window containing the Tulip OpenGL visualization
    # will be created and displayed
    nodeLinkView = tlpgui.createNodeLinkDiagramView(graph)
    # set some rendering parameters for the graph
    renderingParameters = nodeLinkView.getRenderingParameters()
    renderingParameters.setViewArrow(True)
    renderingParameters.setMinSizeOfLabel(8)
    renderingParameters.setEdgeColorInterpolate(True)
    nodeLinkView.setRenderingParameters(renderingParameters)



References
==========

.. [1] David Auber, Romain Bourqui, Maylis Delest, Antoine Lambert,
       Patrick Mary, Guy Mélançon, Bruno Pinaud, Benjamin Renoust and Jason Vallet.
       TULIP 4. Research report. LaBRI - Laboratoire Bordelais de Recherche en Informatique. 2016.
       https://hal.archives-ouvertes.fr/hal-01359308/

.. [2] David Auber, Daniel Archambault, Romain Bourqui, Antoine Lambert, Morgan Mathiaut,
       Patrick Mary, Maylis Delest, Jonathan Dubois, and Guy Mélançon. The Tulip 3 Framework:
       A Scalable Software Library for Information Visualization Applications Based on Relational
       Data. Technical report RR-7860, INRIA, January 2012

.. [3] Antoine Lambert and David Auber. Graph analysis and visualization with Tulip-Python.
       EuroSciPy 2012 - 5th European meeting on Python in Science, Bruxelles

.. [4] Riverbank Computing Limited. SIP - a tool for automatically generating Python bindings for
       C and C++ libraries. http://www.riverbankcomputing.co.uk/software/sip

