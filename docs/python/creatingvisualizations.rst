.. py:currentmodule:: tulipgui

Creating and manipulating Tulip visualizations with the :mod:`tulipgui` module
===============================================================================

Starting the 3.8 release of Tulip, a new module called :mod:`tulipgui` is available
allowing to create and manipulate Tulip views (typically Node Link diagrams).
The module can be used inside the Python Script editor integrated in the Tulip software GUI but
also through the classical Python interpreter.

The main features offered by that module are :

    * creation of interactive Tulip visualizations
    * the ability to change the data source on opened visualizations
    * the possibilty to modify the rendering parameters for node link diagram visualizations
    * the ability to save visualization snapshots to image files on disk

Using the module from the main Tulip GUI
----------------------------------------

When the :mod:`tulipgui` module is used through the Python script editor integrated in the Tulip GUI,
it enables to add and manipulate views in the Tulip workspace.

For instance, the following script works on a geographical network. We suppose
that we already have an opened Node Link Diagram view (plus one Python Script view) in Tulip for visualizing the whole network.
The script will extract the induced sub-graph of the european nodes, create a new Node Link Diagram view for visualizing
it and set some rendering parameters::

    from tulip import *
    from tulipgui import *

    def main(graph):

        # extraction and creation of the european sub-graph
        continent =  graph.getStringProperty("continent")
        europeNodes = []
        for n in graph.getNodes():
            if continent[n] == "Europe":
                europeNodes.append(n)
        europeSg = graph.inducedSubGraph(europeNodes)
        europeSg.setName("Europe")

        # get a reference to the already opened Node Link Diagram view
        nlvWorld = tlp.getOpenedViews()[0]

        # create a new Node Link Diagram view for visualizing the Europe sub-graph
        nlvEurope = tlp.addView("Node Link Diagram view", europeSg)

        # set labels scaled to node sizes mode
        renderingParameters = nlvEurope.getRenderingParameters()
        renderingParameters.setLabelScaled(True)
        nlvEurope.setRenderingParameters(renderingParameters)

        # tiles the opened views in the Tulip workspace
        tlp.tileViews()

        # recenter the node link views after the tile operation
        nlvWorld.centerView()
        nlvEurope.centerView()



Using the module with the classical Python interpreter
------------------------------------------------------

The :mod:`tulipgui` module can also be used with the classical Python interpreter and shell.
Tulip interactive visualizations will be displayed in separate windows once they have been created.

Interactive mode
^^^^^^^^^^^^^^^^^

When working through the Python shell, Tulip views can be created interactively.
The opened views will be updated each time the graph or its properties are modified.

For instance, the following session imports a grid graph, creates a Node Link Diagram view of it
and then changes the nodes colors. The Node Link Diagram view will be updated each time a node
color is modified::

    >>> from tulip import *
    >>> from tulipgui import *
    >>> grid = tlp.importGraph("Grid")
    >>> view = tlp.addNodeLinkDiagramView(grid)
    >>> viewColor = graph.getColorProperty("viewColor")
    >>> for n in graph.getNodes():
    ...     viewColor[n] = tlp.Color(0, 255, 0)
    ...

.. warning:: When working on Windows platforms, you have to use the Python command line utility (not the IDLE one)
             if you want to use the :mod:`tulipgui` module interactively. In a same maneer, if you intend to launch python
             through a terminal, you have to used the basic windows console cmd.exe. Other shells like mintty or rxvt
             do not allow to process the GUI events, required for interactive use.

Script execution mode
^^^^^^^^^^^^^^^^^^^^^^

When executing a script from a command line through the classical python interpreter, if
Tulip views had been created during its execution, the script will terminate once all view
windows had been closed.

Below are some samples scripts illustrating the features of the :mod:`tulipgui` module.
The first script imports a grid approximation graph, computes some visual attributes on it
and creates a Node Link Diagram visualization (which will remain displayed at the end of
the script execution). :ref:`Figure 1<fig1a>` introduces a screenshot of the created view.::

    from tulip import *
    from tulipogl import *
    from tulipgui import *

    # Import a grid approximation (with default parameters)
    graph = tlp.importGraph("Grid Approximation")

    viewLayout = graph.getLayoutProperty("viewLayout")
    viewSize = graph.getSizeProperty("viewSize")
    viewBorderWidth = graph.getDoubleProperty("viewBorderWidth")
    viewColor = graph.getColorProperty("viewColor")
    viewLabel = graph.getStringProperty("viewLabel")

    # Apply an FM^3 layout on it
    fm3pParams = tlp.getDefaultPluginParameters("FM^3 (OGDF)", graph)
    fm3pParams["Unit edge length"] = 100
    graph.applyLayoutAlgorithm("FM^3 (OGDF)", viewLayout, fm3pParams)

    # Compute an anonymous degree property
    degree = tlp.DoubleProperty(graph)
    degreeParams = tlp.getDefaultPluginParameters("Degree")
    graph.applyDoubleAlgorithm("Degree", degree, degreeParams)

    # Map the node sizes to their degree
    sizeMappingParams = tlp.getDefaultPluginParameters("Metric Mapping", graph)
    sizeMappingParams["property"] = degree
    sizeMappingParams["min size"] = 1
    sizeMappingParams["max size"] = 30
    graph.applySizeAlgorithm("Metric Mapping", viewSize, sizeMappingParams)

    # Create a heat map color scale
    heatMap = tlp.ColorScale([tlp.Color(0, 255, 0), tlp.Color(0,0,0), tlp.Color(255, 0, 0)])

    # Map the node colors to their degree using the heat map color scale
    # Also set the nodes labels to their id
    for n in graph.getNodes():
        viewColor[n] = heatMap.getColorAtPos((degree[n] - degree.getNodeMin()) / (degree.getNodeMax() - degree.getNodeMin()))
        viewLabel[n] = str(n.id)

    # Add a border to edges
    viewBorderWidth.setAllEdgeValue(1)

    # Create a Node Link Diagram view and set some rendering parameters
    nodeLinkView = tlp.addNodeLinkDiagramView(graph)
    renderingParameters = nodeLinkView.getRenderingParameters()
    renderingParameters.setViewArrow(True)
    renderingParameters.setMinSizeOfLabel(20)
    nodeLinkView.setRenderingParameters(renderingParameters)


.. _fig1a:
.. figure:: tulipguiViewWindow.png
   :align: center

   Figure 1: Screenshot of the view created by the above script.

The second script aims to generate a snapshot of a file system directory visualization.
It begins by calling the "File System Directory" import plugin, then it sets some
visual attributes on graph elements and finally it creates a node link diagram view
(that will then be hided) with particular rendering parameters for taking the snapshot.
:ref:`Figure 2<fig2a>` introduces the resulting snaphot.::

    from tulip import *
    from tulipogl import *
    from tulipgui import *

    # Create an empty graph
    graph = tlp.newGraph()

    # Set the parameters for the "File System Directory" Import module
    fsImportParams = tlp.getDefaultPluginParameters("File System Directory", graph)
    fsImportParams["dir::directory"] = "/home/antoine/tulip_3_8_install"

    # Import a file system directory content as a tree
    tlp.importGraph("File System Directory", fsImportParams, graph)

    # Get some visual attributes properties
    viewLabel =  graph.getStringProperty("viewLabel")
    viewLabelColor =  graph.getColorProperty("viewLabelColor")
    viewLayout =  graph.getLayoutProperty("viewLayout")
    viewBorderWidth = graph.getDoubleProperty("viewBorderWidth")

    # Apply the "Bubble Tree" layout on the imported graph
    bubbleTreeParams = tlp.getDefaultPluginParameters("Bubble Tree", graph)
    graph.applyLayoutAlgorithm("Bubble Tree", viewLayout, bubbleTreeParams)

    # Creates a property that will be used to order the rendering of graph elements
    # as we want to be sure that the directory nodes labels will be visible
    renderingOrderingProp = graph.getDoubleProperty("rendering ordering")

    for n in graph.getNodes():
        # the "File System Directory" import plugin adds a name property containg the file name
        viewLabel[n] = graph["name"][n]
        # if the node represents a directory, ensure that its label will be visible (as we will activate the "no labels overlaps" mode)
        # also change its label color to blue
        if graph.deg(n) > 1:
            renderingOrderingProp[n] = 1
            viewLabelColor[n] = tlp.Color(0, 0, 255)
        else:
            renderingOrderingProp[n] = 0

    viewBorderWidth.setAllEdgeValue(1)

    # Create a Node Link Diagram view
    nodeLinkView = tlp.addNodeLinkDiagramView(graph)

    # Hides the view after creating it. That way, the script will terminate at the end of its execution.
    # The view window will quickly appear however.
    nodeLinkView.setVisible(False)
    renderingParams = nodeLinkView.getRenderingParameters()

    # Activate the ordered rendering mode
    renderingParams.setElementOrdered(True)
    renderingParams.setElementOrderingProperty(renderingOrderingProp)

    # Activate the "no labels overlaps" mode
    renderingParams.setLabelsDensity(0)
    renderingParams.setMinSizeOfLabel(20)
    nodeLinkView.setRenderingParameters(renderingParams)

    # Save a snapshot of the view to an image file on disk
    nodeLinkView.savePicture("/home/antoine/tulip_3_8_install.png", 1920, 1080, True)


.. _fig2a:
.. figure:: tulip_3_8_install.png
   :align: center
   :scale: 60%

   Figure 2: Snapshot obtained with the above script.

Managing the visualizations updates
------------------------------------

If you are working through the Python Script editor in the main Tulip GUI, the
updates of visualizations are blocked until the script execution terminates.
You can however trigger the redraw of visualizations through the :const:`updateVisualization(center=True)` function
of by calling the :meth:`tlp.View.draw` method.

If you are working through the classical Python interpreter (shell or script mode), the opened
visualizations will be updated each time the visualized graphs and their properties will be modified.
You can however block those updates by the function call below::

    tlp.Observable.holdObservers()

This will put the Tulip observation mecanism on hold and stop the triggers of
visualizations updates. The visualizations can still be updated by calling
the :meth:`tlp.View.draw` method.

To reactivate the automatic views updates, issue the following function call::

    tlp.Observable.unholdObservers()
