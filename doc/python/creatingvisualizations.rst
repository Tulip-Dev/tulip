.. py:currentmodule:: tulipgui

Creating and manipulating Tulip visualizations with the :mod:`tulipgui` module
===============================================================================

Starting the 4.4 release of Tulip, a new module called :mod:`tulipgui` is available
allowing to create and manipulate Tulip views (typically node link diagram views).
The module is usable from the Python Script editor integrated in the Tulip software GUI.

The main features offered by that module are :

    * creation of interactive Tulip visualizations
    * the ability to change the data source on opened visualizations
    * the possibility to modify the rendering parameters for node link diagram visualizations
    * the ability to save visualization snapshots to image files on disk

Using the module from the main Tulip GUI
----------------------------------------

When the :mod:`tulipgui` module is used through the Python script editor integrated in the Tulip GUI,
it enables to add and manipulate views in the Tulip workspace.

For instance, the following script works on a geographical network. We suppose
that we already have an opened Node Link Diagram view (plus one Python Script view) in Tulip for visualizing the whole network.
The script will extract the induced subgraph of the european nodes, create a new Node Link Diagram view for visualizing
it and set some rendering parameters::

    from tulip import tlp
    from tulipgui import tlpgui

    def main(graph):

      # extraction and creation of the european subgraph
      continent =  graph.getStringProperty("continent")
      europeNodes = []
      for n in graph.getNodes():
        if continent[n] == "Europe":
          europeNodes.append(n)
      europeSg = graph.inducedSubGraph(europeNodes)
      europeSg.setName("Europe")

      nlvEurope = tlpgui.createNodeLinkDiagramView(europeSg)

      # set labels scaled to node sizes mode
      renderingParameters = nlvEurope.getRenderingParameters()
      renderingParameters.setLabelScaled(True)
      nlvEurope.setRenderingParameters(renderingParameters)
