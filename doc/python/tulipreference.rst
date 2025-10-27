.. py:currentmodule:: tulip

:mod:`tulip` module API
==========================

Utility functions
-------------------

Graph management
^^^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.newGraph
    :noindex:

.. autofunction:: tulip::tlp.loadGraph
    :noindex:

.. autofunction:: tulip::tlp.loadGraph
    :noindex:

.. autofunction:: tulip::tlp.saveGraph
    :noindex:

.. autofunction:: tulip::tlp.saveGraph
    :noindex:

.. autofunction:: tulip::tlp.importGraph
    :noindex:

.. autofunction:: tulip::tlp.exportGraph
    :noindex:

.. autofunction:: tulip::tlp.copyToGraph
    :noindex:

.. autofunction:: tulip::tlp.removeFromGraph
    :noindex:

.. autofunction:: tulip::tlp.getRootGraphs
    :noindex:

Graph measures and functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.averagePathLength
    :noindex:

.. autofunction:: tulip::tlp.averageClusteringCoefficient
    :noindex:

.. autofunction:: tulip::tlp.clusteringCoefficient
    :noindex:

.. autofunction:: tulip::tlp.dagLevel
    :noindex:

.. autofunction:: tulip::tlp.minDegree
    :noindex:

.. autofunction:: tulip::tlp.maxDegree
    :noindex:

.. autofunction:: tulip::tlp.maxDistance

.. autofunction:: tulip::tlp.maxDistanceWeighted

.. autofunction:: tulip::tlp.reachableNodes

.. autofunction:: tulip::tlp.bfs
    :noindex:

.. autofunction:: tulip::tlp.bfs
    :noindex:

.. autofunction:: tulip::tlp.dfs
    :noindex:

.. autofunction:: tulip::tlp.dfs
    :noindex:

.. autofunction:: tulip::tlp.selectShortestPaths

Geometry
^^^^^^^^

.. autofunction:: tulip::tlp.computeBoundingBox
    :noindex:

.. autofunction:: tulip::tlp.computeBoundingBox
    :noindex:

.. autofunction:: tulip::tlp.computeBoundingRadius
    :noindex:

.. autofunction:: tulip::tlp.computeBoundingRadius
    :noindex:

.. autofunction:: tulip::tlp.computeConvexHull
    :noindex:

.. autofunction:: tulip::tlp.computeConvexHull
    :noindex:

.. autofunction:: tulip::tlp.computeConvexHull
    :noindex:

.. autofunction:: tulip::tlp.convexHull2D
    :noindex:

.. autofunction:: tulip::tlp.convexHull
    :noindex:

.. autofunction:: tulip::tlp.computeLinesIntersection
    :noindex:

.. autofunction:: tulip::tlp.computePolygonCentroid
    :noindex:

.. autofunction:: tulip::tlp.delaunayTriangulation
    :noindex:

.. autoclass:: tulip::tlp.VoronoiDiagram
    :members:

.. autofunction:: tulip::tlp.voronoiDiagram
    :noindex:

.. _parametric-curves:

Parametric curves computation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.computeBezierPoint
    :noindex:

.. autofunction:: tulip::tlp.computeBezierPoints
    :noindex:

.. autofunction:: tulip::tlp.computeCatmullRomPoint
    :noindex:

.. autofunction:: tulip::tlp.computeCatmullRomPoints
    :noindex:

.. autofunction:: tulip::tlp.computeOpenUniformBsplinePoint
    :noindex:

.. autofunction:: tulip::tlp.computeOpenUniformBsplinePoints
    :noindex:

Plugins Management
^^^^^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.getTulipRelease
    :noindex:

.. autofunction:: tulip::tlp.initTulipLib
    :noindex:

.. autofunction:: tulip::tlp.loadPlugins
    :noindex:

.. autofunction:: tulip::tlp.loadPluginsFromDir
    :noindex:

.. autofunction:: tulip::tlp.loadPlugin
    :noindex:

.. autofunction:: tulip::tlp.loadPluginsCheckDependencies
    :noindex:

.. autofunction:: tulip::tlp.getDefaultPluginParameters
    :noindex:

.. autofunction:: tulip::tlp.getImportPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getExportPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getAlgorithmPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getBooleanAlgorithmPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getColorAlgorithmPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getDoubleAlgorithmPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getIntegerAlgorithmPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getLayoutAlgorithmPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getSizeAlgorithmPluginsList
    :noindex:

.. autofunction:: tulip::tlp.getStringAlgorithmPluginsList
    :noindex:

Graph elements classes
----------------------------

tlp.node
^^^^^^^^

.. autoclass:: tulip::tlp.node
	:members:


tlp.edge
^^^^^^^^

.. autoclass:: tulip::tlp.edge
	:members:


.. _graph-class:

The tlp.Graph class
----------------------------

.. autoclass:: tulip::tlp.Graph

.. _graph-class.structure-modif:

Modification of the graph structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.clear
    :noindex:

.. automethod:: tulip::tlp.Graph.addNode
    :noindex:

.. automethod:: tulip::tlp.Graph.addNode
    :noindex:

.. automethod:: tulip::tlp.Graph.addNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.addNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.addNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.delNode
    :noindex:

.. automethod:: tulip::tlp.Graph.delNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.delNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.addEdge
    :noindex:

.. automethod:: tulip::tlp.Graph.addEdge
    :noindex:

.. automethod:: tulip::tlp.Graph.addEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.addEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.addEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.delEdge
    :noindex:

.. automethod:: tulip::tlp.Graph.delEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.delEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.setEdgeOrder
    :noindex:

.. automethod:: tulip::tlp.Graph.swapEdgeOrder
    :noindex:

.. automethod:: tulip::tlp.Graph.setSource
    :noindex:

.. automethod:: tulip::tlp.Graph.setTarget
    :noindex:

.. automethod:: tulip::tlp.Graph.setEnds
    :noindex:

.. automethod:: tulip::tlp.Graph.reverse
    :noindex:

.. _graph-class.subgraphs-hierarchy:

Creation and Modification of the subgraph hierarchy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.addSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.addSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.addCloneSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.inducedSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.inducedSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.delSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.delAllSubGraphs
    :noindex:

.. automethod:: tulip::tlp.Graph.getSuperGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.getRoot
    :noindex:

.. automethod:: tulip::tlp.Graph.setSuperGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.getSubGraphs
    :noindex:

.. automethod:: tulip::tlp.Graph.isSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.isDescendantGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.getSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.getSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.getDescendantGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.getDescendantGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.getNthSubGraph
    :noindex:

.. automethod:: tulip::tlp.Graph.numberOfSubGraphs
    :noindex:

.. automethod:: tulip::tlp.Graph.numberOfDescendantGraphs
    :noindex:

.. automethod:: tulip::tlp.Graph.getDescendantGraphs
    :noindex:

.. _graph-class.iterators:

Iterators on the graph structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All the provided iterators are stable, meaning you can safely delete a node/edge/subgraph
from the graph while iterating on its nodes/edges/subgraphs.

.. automethod:: tulip::tlp.Graph.getOneNode
    :noindex:

.. automethod:: tulip::tlp.Graph.getRandomNode
    :noindex:

.. automethod:: tulip::tlp.Graph.getNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.getInNode
    :noindex:

.. automethod:: tulip::tlp.Graph.getInNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.getOutNode
    :noindex:

.. automethod:: tulip::tlp.Graph.getOutNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.getInOutNodes
    :noindex:

.. automethod:: tulip::tlp.Graph.bfs
    :noindex:

.. automethod:: tulip::tlp.Graph.dfs
    :noindex:

.. automethod:: tulip::tlp.Graph.getNodeMetaInfo
    :noindex:

.. automethod:: tulip::tlp.Graph.getOneEdge
    :noindex:

.. automethod:: tulip::tlp.Graph.getRandomEdge
    :noindex:

.. automethod:: tulip::tlp.Graph.getEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.getOutEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.getInOutEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.getInEdges
    :noindex:

.. automethod:: tulip::tlp.Graph.getEdgeMetaInfo
    :noindex:

.. _graph-class.information:

Information about the graph structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.nodes

.. automethod:: tulip::tlp.Graph.nodePos

.. automethod:: tulip::tlp.Graph.edges

.. automethod:: tulip::tlp.Graph.edgePos

.. automethod:: tulip::tlp.Graph.allEdges

.. automethod:: tulip::tlp.Graph.subGraphs

.. automethod:: tulip::tlp.Graph.getId

.. automethod:: tulip::tlp.Graph.getName

.. automethod:: tulip::tlp.Graph.setName

.. automethod:: tulip::tlp.Graph.getAttributes

.. automethod:: tulip::tlp.Graph.getAttribute

.. automethod:: tulip::tlp.Graph.setAttribute

.. automethod:: tulip::tlp.Graph.removeAttribute

.. automethod:: tulip::tlp.Graph.existAttribute

.. automethod:: tulip::tlp.Graph.getEdges(node1, node2, directed=True)

.. automethod:: tulip::tlp.Graph.isEmpty

.. automethod:: tulip::tlp.Graph.numberOfNodes

.. automethod:: tulip::tlp.Graph.numberOfEdges

.. automethod:: tulip::tlp.Graph.deg

.. automethod:: tulip::tlp.Graph.indeg

.. automethod:: tulip::tlp.Graph.outdeg

.. automethod:: tulip::tlp.Graph.source

.. automethod:: tulip::tlp.Graph.target

.. automethod:: tulip::tlp.Graph.ends

.. automethod:: tulip::tlp.Graph.opposite

.. automethod:: tulip::tlp.Graph.isElement

.. automethod:: tulip::tlp.Graph.isMetaNode

.. automethod:: tulip::tlp.Graph.isElement
    :noindex:

.. automethod:: tulip::tlp.Graph.isElement
    :noindex:

.. automethod:: tulip::tlp.Graph.isMetaEdge

.. automethod:: tulip::tlp.Graph.existEdge

.. automethod:: tulip::tlp.Graph.hasEdge

.. automethod:: tulip::tlp.Graph.getSource

.. automethod:: tulip::tlp.Graph.getSink

.. _graph-class.properties:

Create/Access graph properties
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.getBooleanProperty

.. automethod:: tulip::tlp.Graph.getLocalBooleanProperty

.. automethod:: tulip::tlp.Graph.getBooleanVectorProperty

.. automethod:: tulip::tlp.Graph.getLocalBooleanVectorProperty

.. automethod:: tulip::tlp.Graph.getColorProperty

.. automethod:: tulip::tlp.Graph.getLocalColorProperty

.. automethod:: tulip::tlp.Graph.getColorVectorProperty

.. automethod:: tulip::tlp.Graph.getLocalColorVectorProperty

.. automethod:: tulip::tlp.Graph.getDoubleProperty

.. automethod:: tulip::tlp.Graph.getLocalDoubleProperty

.. automethod:: tulip::tlp.Graph.getDoubleVectorProperty

.. automethod:: tulip::tlp.Graph.getLocalDoubleVectorProperty

.. automethod:: tulip::tlp.Graph.getIntegerProperty

.. automethod:: tulip::tlp.Graph.getLocalIntegerProperty

.. automethod:: tulip::tlp.Graph.getIntegerVectorProperty

.. automethod:: tulip::tlp.Graph.getLocalIntegerVectorProperty

.. automethod:: tulip::tlp.Graph.getGraphProperty

.. automethod:: tulip::tlp.Graph.getLocalGraphProperty

.. automethod:: tulip::tlp.Graph.getLayoutProperty

.. automethod:: tulip::tlp.Graph.getLocalLayoutProperty

.. automethod:: tulip::tlp.Graph.getCoordVectorProperty

.. automethod:: tulip::tlp.Graph.getLocalCoordVectorProperty

.. automethod:: tulip::tlp.Graph.getSizeProperty

.. automethod:: tulip::tlp.Graph.getLocalSizeProperty

.. automethod:: tulip::tlp.Graph.getSizeVectorProperty

.. automethod:: tulip::tlp.Graph.getLocalSizeVectorProperty

.. automethod:: tulip::tlp.Graph.getStringProperty

.. automethod:: tulip::tlp.Graph.getLocalStringProperty

.. automethod:: tulip::tlp.Graph.getStringVectorProperty

.. automethod:: tulip::tlp.Graph.getLocalStringVectorProperty

.. automethod:: tulip::tlp.Graph.getProperty

.. automethod:: tulip::tlp.Graph.existProperty

.. automethod:: tulip::tlp.Graph.existLocalProperty

.. automethod:: tulip::tlp.Graph.delLocalProperty

.. automethod:: tulip::tlp.Graph.getLocalProperties

.. automethod:: tulip::tlp.Graph.getInheritedProperties

.. automethod:: tulip::tlp.Graph.getProperties

.. automethod:: tulip::tlp.Graph.getLocalObjectProperties

.. automethod:: tulip::tlp.Graph.getInheritedObjectProperties

.. automethod:: tulip::tlp.Graph.getObjectProperties

.. automethod:: tulip::tlp.Graph.getNodePropertiesValues

.. automethod:: tulip::tlp.Graph.getEdgePropertiesValues

.. automethod:: tulip::tlp.Graph.setNodePropertiesValues

.. automethod:: tulip::tlp.Graph.setEdgePropertiesValues

Application of algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.applyAlgorithm

.. automethod:: tulip::tlp.Graph.applyBooleanAlgorithm

.. automethod:: tulip::tlp.Graph.applyBooleanAlgorithm
    :noindex:

.. automethod:: tulip::tlp.Graph.applyColorAlgorithm

.. automethod:: tulip::tlp.Graph.applyColorAlgorithm
    :noindex:

.. automethod:: tulip::tlp.Graph.applyDoubleAlgorithm

.. automethod:: tulip::tlp.Graph.applyDoubleAlgorithm
    :noindex:

.. automethod:: tulip::tlp.Graph.applyIntegerAlgorithm

.. automethod:: tulip::tlp.Graph.applyLayoutAlgorithm

.. automethod:: tulip::tlp.Graph.applyLayoutAlgorithm
    :noindex:

.. automethod:: tulip::tlp.Graph.applySizeAlgorithm

.. automethod:: tulip::tlp.Graph.applySizeAlgorithm
    :noindex:

.. automethod:: tulip::tlp.Graph.applyStringAlgorithm

.. automethod:: tulip::tlp.Graph.applyStringAlgorithm
    :noindex:

Update management
^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.push

.. automethod:: tulip::tlp.Graph.pop

.. automethod:: tulip::tlp.Graph.unpop

.. automethod:: tulip::tlp.Graph.canPop

.. automethod:: tulip::tlp.Graph.canUnpop

.. automethod:: tulip::tlp.Graph.canPopThenUnpop

.. automethod:: tulip::tlp.Graph.popIfNoUpdates

Meta-nodes management
^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.createMetaNode

.. automethod:: tulip::tlp.Graph.createMetaNode
    :noindex:

.. automethod:: tulip::tlp.Graph.createMetaNodes

.. automethod:: tulip::tlp.Graph.openMetaNode

Tulip datatypes
---------------

tlp.Vec3f
^^^^^^^^^

.. autoclass:: tulip::tlp.Vec3f
	:members:


tlp.Vec4f
^^^^^^^^^

.. autoclass:: tulip::tlp.Vec4f

tlp.Vec4i
^^^^^^^^^

.. autoclass:: tulip::tlp.Vec4i

tlp.Coord
^^^^^^^^^

.. autoclass:: tulip::tlp.Coord
  :members:


tlp.BoundingBox
^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.BoundingBox

        .. automethod:: tulip::tlp.BoundingBox.center

        .. automethod:: tulip::tlp.BoundingBox.contains

        .. automethod:: tulip::tlp.BoundingBox.depth

        .. automethod:: tulip::tlp.BoundingBox.expand

        .. automethod:: tulip::tlp.BoundingBox.height

        .. automethod:: tulip::tlp.BoundingBox.intersect

        .. automethod:: tulip::tlp.BoundingBox.intersect
            :noindex:

        .. automethod:: tulip::tlp.BoundingBox.isValid

        .. automethod:: tulip::tlp.BoundingBox.translate

        .. automethod:: tulip::tlp.BoundingBox.width

tlp.Size
^^^^^^^^

.. autoclass:: tulip::tlp.Size

  alias of :class:`tlp.Vec3f`


tlp.Color
^^^^^^^^^

.. autoclass:: tulip::tlp.Color
	:members:


tlp.ColorScale
^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ColorScale
	:members:


tlp.StringCollection
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.StringCollection

  .. automethod:: tulip::tlp.StringCollection.at

  .. automethod:: tulip::tlp.StringCollection.empty

  .. automethod:: tulip::tlp.StringCollection.getCurrent

  .. automethod:: tulip::tlp.StringCollection.getCurrentString

  .. automethod:: tulip::tlp.StringCollection.push_back

  .. automethod:: tulip::tlp.StringCollection.setCurrent

  .. automethod:: tulip::tlp.StringCollection.setCurrent
    :noindex:

  .. automethod:: tulip::tlp.StringCollection.size

tlp.DataSet
^^^^^^^^^^^

.. autoclass:: tulip::tlp.DataSet
	:members:


tlp.PluginProgress
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.PluginProgress
	:members:


Tulip observation mechanism
----------------------------

tlp.Event
^^^^^^^^^

.. autoclass:: tulip::tlp.Event
        :members:

tlp.GraphEvent
^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.GraphEvent
        :members:

tlp.PropertyEvent
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.PropertyEvent
        :members:

tlp.Observable
^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.Observable
        :members:

Graph properties classes
----------------------------

tlp.PropertyInterface
^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.PropertyInterface
	:members:


tlp.BooleanProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.BooleanProperty

  .. automethod:: tulip::tlp.BooleanProperty.copy

  .. automethod:: tulip::tlp.BooleanProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.BooleanProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.BooleanProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.BooleanProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.BooleanProperty.reverse

  .. automethod:: tulip::tlp.BooleanProperty.reverseEdgeDirection

  .. automethod:: tulip::tlp.BooleanProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.BooleanProperty.getEdgeValue

  .. automethod:: tulip::tlp.BooleanProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.BooleanProperty.getNodeValue

  .. automethod:: tulip::tlp.BooleanProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.BooleanProperty.setAllEdgeValue
    :noindex:

  .. automethod:: tulip::tlp.BooleanProperty.setAllNodeValue

  .. automethod:: tulip::tlp.BooleanProperty.setAllNodeValue
    :noindex:

  .. automethod:: tulip::tlp.BooleanProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.BooleanProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.BooleanProperty.setEdgeValue

  .. automethod:: tulip::tlp.BooleanProperty.setNodeValue

  .. automethod:: tulip::tlp.BooleanProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.BooleanProperty.setEdgeDefaultValue
                  
tlp.ColorProperty
^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ColorProperty

  .. automethod:: tulip::tlp.ColorProperty.copy

  .. automethod:: tulip::tlp.ColorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.ColorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.ColorProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.ColorProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.ColorProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.ColorProperty.getEdgeValue

  .. automethod:: tulip::tlp.ColorProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.ColorProperty.getNodeValue

  .. automethod:: tulip::tlp.ColorProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.ColorProperty.setAllEdgeValue
    :noindex:

  .. automethod:: tulip::tlp.ColorProperty.setAllNodeValue

  .. automethod:: tulip::tlp.ColorProperty.setAllNodeValue
    :noindex:

  .. automethod:: tulip::tlp.ColorProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.ColorProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.ColorProperty.setEdgeValue

  .. automethod:: tulip::tlp.ColorProperty.setNodeValue

  .. automethod:: tulip::tlp.ColorProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.ColorProperty.setEdgeDefaultValue

tlp.NumericProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.NumericProperty
        :members:
                  copyProperty, uniformQuantification, nodesUniformQuantification, edgesUniformQuantification,
                  getEdgeDoubleMin, getEdgeDoubleMax, getNodeDoubleMin, getNodeDoubleMax,
                  getEdgeDoubleDefaultValue, getEdgeDoubleValue, getNodeDoubleDefaultValue, getNodeDoubleValue,
                  getSortedNodes, getSortedEdges, getSortedEdgesBySourceValue, getSortedEdgesByTargetValue, getSortedEdgesByExtremitiesValues

tlp.DoubleProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.DoubleProperty

  .. automethod:: tulip::tlp.DoubleProperty.copy

  .. automethod:: tulip::tlp.DoubleProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.DoubleProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.DoubleProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.DoubleProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.DoubleProperty.getNodeMin

  .. automethod:: tulip::tlp.DoubleProperty.getNodeMax

  .. automethod:: tulip::tlp.DoubleProperty.getEdgeMin

  .. automethod:: tulip::tlp.DoubleProperty.getEdgeMax

  .. automethod:: tulip::tlp.DoubleProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.DoubleProperty.getEdgeValue

  .. automethod:: tulip::tlp.DoubleProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.DoubleProperty.getNodeValue

  .. automethod:: tulip::tlp.DoubleProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.DoubleProperty.setAllNodeValue

  .. automethod:: tulip::tlp.DoubleProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.DoubleProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.DoubleProperty.setEdgeValue

  .. automethod:: tulip::tlp.DoubleProperty.setNodeValue

  .. automethod:: tulip::tlp.DoubleProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.DoubleProperty.setEdgeDefaultValue

  .. automethod:: tulip::tlp.DoubleProperty.setMetaValueCalculator

tlp.IntegerProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.IntegerProperty

  .. automethod:: tulip::tlp.IntegerProperty.copy

  .. automethod:: tulip::tlp.IntegerProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.IntegerProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.IntegerProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.IntegerProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.IntegerProperty.getNodeMin

  .. automethod:: tulip::tlp.IntegerProperty.getNodeMax

  .. automethod:: tulip::tlp.IntegerProperty.getEdgeMin

  .. automethod:: tulip::tlp.IntegerProperty.getEdgeMax

  .. automethod:: tulip::tlp.IntegerProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.IntegerProperty.getEdgeValue

  .. automethod:: tulip::tlp.IntegerProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.IntegerProperty.getNodeValue

  .. automethod:: tulip::tlp.IntegerProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.IntegerProperty.setAllNodeValue

  .. automethod:: tulip::tlp.IntegerProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.IntegerProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.IntegerProperty.setEdgeValue

  .. automethod:: tulip::tlp.IntegerProperty.setNodeValue

  .. automethod:: tulip::tlp.IntegerProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.IntegerProperty.setEdgeDefaultValue

tlp.LayoutProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.LayoutProperty

  .. automethod:: tulip::tlp.LayoutProperty.angularResolutions

  .. automethod:: tulip::tlp.LayoutProperty.averageAngularResolution

  .. automethod:: tulip::tlp.LayoutProperty.averageAngularResolution
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.averageEdgeLength

  .. automethod:: tulip::tlp.LayoutProperty.center

  .. automethod:: tulip::tlp.LayoutProperty.center
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.computeEmbedding

  .. automethod:: tulip::tlp.LayoutProperty.computeEmbedding
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.copy

  .. automethod:: tulip::tlp.LayoutProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.LayoutProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.LayoutProperty.edgeLength

  .. automethod:: tulip::tlp.LayoutProperty.getMin

  .. automethod:: tulip::tlp.LayoutProperty.getMax

  .. automethod:: tulip::tlp.LayoutProperty.normalize

  .. automethod:: tulip::tlp.LayoutProperty.perfectAspectRatio

  .. automethod:: tulip::tlp.LayoutProperty.rotateX

  .. automethod:: tulip::tlp.LayoutProperty.rotateX
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.rotateY

  .. automethod:: tulip::tlp.LayoutProperty.rotateY
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.rotateZ

  .. automethod:: tulip::tlp.LayoutProperty.rotateZ
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.scale

  .. automethod:: tulip::tlp.LayoutProperty.scale
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.translate

  .. automethod:: tulip::tlp.LayoutProperty.translate
    :noindex:

  .. automethod:: tulip::tlp.LayoutProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.LayoutProperty.getEdgeValue

  .. automethod:: tulip::tlp.LayoutProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.LayoutProperty.getNodeValue

  .. automethod:: tulip::tlp.LayoutProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.LayoutProperty.setAllNodeValue

  .. automethod:: tulip::tlp.LayoutProperty.setEdgeValue

  .. automethod:: tulip::tlp.LayoutProperty.setNodeValue

  .. automethod:: tulip::tlp.LayoutProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.LayoutProperty.setEdgeDefaultValue

tlp.SizeProperty
^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SizeProperty

  .. automethod:: tulip::tlp.SizeProperty.copy

  .. automethod:: tulip::tlp.SizeProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.SizeProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.SizeProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.SizeProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.SizeProperty.getMin

  .. automethod:: tulip::tlp.SizeProperty.getMax

  .. automethod:: tulip::tlp.SizeProperty.scale

  .. automethod:: tulip::tlp.SizeProperty.scale
    :noindex:

  .. automethod:: tulip::tlp.SizeProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.SizeProperty.getEdgeValue

  .. automethod:: tulip::tlp.SizeProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.SizeProperty.getNodeValue

  .. automethod:: tulip::tlp.SizeProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.SizeProperty.setAllNodeValue

  .. automethod:: tulip::tlp.SizeProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.SizeProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.SizeProperty.setEdgeValue

  .. automethod:: tulip::tlp.SizeProperty.setNodeValue

  .. automethod:: tulip::tlp.SizeProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.SizeProperty.setEdgeDefaultValue

tlp.StringProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.StringProperty

  .. automethod:: tulip::tlp.StringProperty.copy

  .. automethod:: tulip::tlp.StringProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.StringProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.StringProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.StringProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.StringProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.StringProperty.getEdgeValue

  .. automethod:: tulip::tlp.StringProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.StringProperty.getNodeValue

  .. automethod:: tulip::tlp.StringProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.StringProperty.setAllNodeValue

  .. automethod:: tulip::tlp.StringProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.StringProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.StringProperty.setEdgeValue

  .. automethod:: tulip::tlp.StringProperty.setNodeValue

  .. automethod:: tulip::tlp.StringProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.StringProperty.setEdgeDefaultValue

tlp.BooleanVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.BooleanVectorProperty

  .. automethod:: tulip::tlp.BooleanVectorProperty.copy

  .. automethod:: tulip::tlp.BooleanVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.BooleanVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.BooleanVectorProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.BooleanVectorProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.BooleanVectorProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.getEdgeValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.getNodeValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.setAllNodeValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.BooleanVectorProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.BooleanVectorProperty.setEdgeValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.setNodeValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.setEdgeDefaultValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.getNodeEltValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.setNodeEltValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.pushBackNodeEltValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.popBackNodeEltValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.resizeNodeValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: tulip::tlp.BooleanVectorProperty.getEdgeEltValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.setEdgeEltValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.pushBackEdgeEltValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.popBackEdgeEltValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.resizeEdgeValue

  .. automethod:: tulip::tlp.BooleanVectorProperty.resizeEdgeValue
    :noindex:

tlp.CoordVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.CoordVectorProperty

  .. automethod:: tulip::tlp.CoordVectorProperty.copy

  .. automethod:: tulip::tlp.CoordVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.CoordVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.CoordVectorProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.CoordVectorProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.CoordVectorProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.CoordVectorProperty.getEdgeValue

  .. automethod:: tulip::tlp.CoordVectorProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.CoordVectorProperty.getNodeValue

  .. automethod:: tulip::tlp.CoordVectorProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.CoordVectorProperty.setAllNodeValue

  .. automethod:: tulip::tlp.CoordVectorProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.CoordVectorProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.CoordVectorProperty.setEdgeValue

  .. automethod:: tulip::tlp.CoordVectorProperty.setNodeValue

  .. automethod:: tulip::tlp.CoordVectorProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.CoordVectorProperty.setEdgeDefaultValue

  .. automethod:: tulip::tlp.CoordVectorProperty.getNodeEltValue

  .. automethod:: tulip::tlp.CoordVectorProperty.setNodeEltValue

  .. automethod:: tulip::tlp.CoordVectorProperty.pushBackNodeEltValue

  .. automethod:: tulip::tlp.CoordVectorProperty.popBackNodeEltValue

  .. automethod:: tulip::tlp.CoordVectorProperty.resizeNodeValue

  .. automethod:: tulip::tlp.CoordVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: tulip::tlp.CoordVectorProperty.getEdgeEltValue

  .. automethod:: tulip::tlp.CoordVectorProperty.setEdgeEltValue

  .. automethod:: tulip::tlp.CoordVectorProperty.pushBackEdgeEltValue

  .. automethod:: tulip::tlp.CoordVectorProperty.popBackEdgeEltValue

  .. automethod:: tulip::tlp.CoordVectorProperty.resizeEdgeValue

  .. automethod:: tulip::tlp.CoordVectorProperty.resizeEdgeValue
    :noindex:

tlp.ColorVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ColorVectorProperty

  .. automethod:: tulip::tlp.ColorVectorProperty.copy

  .. automethod:: tulip::tlp.ColorVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.ColorVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.ColorVectorProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.ColorVectorProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.ColorVectorProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.ColorVectorProperty.getEdgeValue

  .. automethod:: tulip::tlp.ColorVectorProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.ColorVectorProperty.getNodeValue

  .. automethod:: tulip::tlp.ColorVectorProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.ColorVectorProperty.setAllNodeValue

  .. automethod:: tulip::tlp.ColorVectorProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.ColorVectorProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.ColorVectorProperty.setEdgeValue

  .. automethod:: tulip::tlp.ColorVectorProperty.setNodeValue

  .. automethod:: tulip::tlp.ColorVectorProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.ColorVectorProperty.setEdgeDefaultValue

  .. automethod:: tulip::tlp.ColorVectorProperty.getNodeEltValue

  .. automethod:: tulip::tlp.ColorVectorProperty.setNodeEltValue

  .. automethod:: tulip::tlp.ColorVectorProperty.pushBackNodeEltValue

  .. automethod:: tulip::tlp.ColorVectorProperty.popBackNodeEltValue

  .. automethod:: tulip::tlp.ColorVectorProperty.resizeNodeValue

  .. automethod:: tulip::tlp.ColorVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: tulip::tlp.ColorVectorProperty.getEdgeEltValue

  .. automethod:: tulip::tlp.ColorVectorProperty.setEdgeEltValue

  .. automethod:: tulip::tlp.ColorVectorProperty.pushBackEdgeEltValue

  .. automethod:: tulip::tlp.ColorVectorProperty.popBackEdgeEltValue

  .. automethod:: tulip::tlp.ColorVectorProperty.resizeEdgeValue

  .. automethod:: tulip::tlp.ColorVectorProperty.resizeEdgeValue
    :noindex:

tlp.DoubleVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.DoubleVectorProperty

  .. automethod:: tulip::tlp.DoubleVectorProperty.copy

  .. automethod:: tulip::tlp.DoubleVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.DoubleVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.DoubleVectorProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.DoubleVectorProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.DoubleVectorProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.getEdgeValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.getNodeValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.setAllNodeValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.DoubleVectorProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.DoubleVectorProperty.setEdgeValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.setNodeValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.setEdgeDefaultValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.getNodeEltValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.setNodeEltValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.pushBackNodeEltValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.popBackNodeEltValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.resizeNodeValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: tulip::tlp.DoubleVectorProperty.getEdgeEltValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.setEdgeEltValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.pushBackEdgeEltValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.popBackEdgeEltValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.resizeEdgeValue

  .. automethod:: tulip::tlp.DoubleVectorProperty.resizeEdgeValue
    :noindex:

tlp.IntegerVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.IntegerVectorProperty

  .. automethod:: tulip::tlp.IntegerVectorProperty.copy

  .. automethod:: tulip::tlp.IntegerVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.IntegerVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.IntegerVectorProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.IntegerVectorProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.IntegerVectorProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.getEdgeValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.getNodeValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.setAllNodeValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.IntegerVectorProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.IntegerVectorProperty.setEdgeValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.setNodeValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.setEdgeDefaultValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.getNodeEltValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.setNodeEltValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.pushBackNodeEltValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.popBackNodeEltValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.resizeNodeValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: tulip::tlp.IntegerVectorProperty.getEdgeEltValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.setEdgeEltValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.pushBackEdgeEltValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.popBackEdgeEltValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.resizeEdgeValue

  .. automethod:: tulip::tlp.IntegerVectorProperty.resizeEdgeValue
    :noindex:

tlp.SizeVectorProperty
^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SizeVectorProperty

  .. automethod:: tulip::tlp.SizeVectorProperty.copy

  .. automethod:: tulip::tlp.SizeVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.SizeVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.SizeVectorProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.SizeVectorProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.SizeVectorProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.SizeVectorProperty.getEdgeValue

  .. automethod:: tulip::tlp.SizeVectorProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.SizeVectorProperty.getNodeValue

  .. automethod:: tulip::tlp.SizeVectorProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.SizeVectorProperty.setAllNodeValue

  .. automethod:: tulip::tlp.SizeVectorProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.SizeVectorProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.SizeVectorProperty.setEdgeValue

  .. automethod:: tulip::tlp.SizeVectorProperty.setNodeValue

  .. automethod:: tulip::tlp.SizeVectorProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.SizeVectorProperty.setEdgeDefaultValue

  .. automethod:: tulip::tlp.SizeVectorProperty.getNodeEltValue

  .. automethod:: tulip::tlp.SizeVectorProperty.setNodeEltValue

  .. automethod:: tulip::tlp.SizeVectorProperty.pushBackNodeEltValue

  .. automethod:: tulip::tlp.SizeVectorProperty.popBackNodeEltValue

  .. automethod:: tulip::tlp.SizeVectorProperty.resizeNodeValue

  .. automethod:: tulip::tlp.SizeVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: tulip::tlp.SizeVectorProperty.getEdgeEltValue

  .. automethod:: tulip::tlp.SizeVectorProperty.setEdgeEltValue

  .. automethod:: tulip::tlp.SizeVectorProperty.pushBackEdgeEltValue

  .. automethod:: tulip::tlp.SizeVectorProperty.popBackEdgeEltValue

  .. automethod:: tulip::tlp.SizeVectorProperty.resizeEdgeValue

  .. automethod:: tulip::tlp.SizeVectorProperty.resizeEdgeValue
    :noindex:

tlp.StringVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.StringVectorProperty

  .. automethod:: tulip::tlp.StringVectorProperty.copy

  .. automethod:: tulip::tlp.StringVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.StringVectorProperty.copy
    :noindex:

  .. automethod:: tulip::tlp.StringVectorProperty.getNodesEqualTo

  .. automethod:: tulip::tlp.StringVectorProperty.getEdgesEqualTo

  .. automethod:: tulip::tlp.StringVectorProperty.getEdgeDefaultValue

  .. automethod:: tulip::tlp.StringVectorProperty.getEdgeValue

  .. automethod:: tulip::tlp.StringVectorProperty.getNodeDefaultValue

  .. automethod:: tulip::tlp.StringVectorProperty.getNodeValue

  .. automethod:: tulip::tlp.StringVectorProperty.setAllEdgeValue

  .. automethod:: tulip::tlp.StringVectorProperty.setAllNodeValue

  .. automethod:: tulip::tlp.StringVectorProperty.setValueToGraphNodes

  .. automethod:: tulip::tlp.StringVectorProperty.setValueToGraphEdges

  .. automethod:: tulip::tlp.StringVectorProperty.setEdgeValue

  .. automethod:: tulip::tlp.StringVectorProperty.setNodeValue

  .. automethod:: tulip::tlp.StringVectorProperty.setNodeDefaultValue

  .. automethod:: tulip::tlp.StringVectorProperty.setEdgeDefaultValue

  .. automethod:: tulip::tlp.StringVectorProperty.getNodeEltValue

  .. automethod:: tulip::tlp.StringVectorProperty.setNodeEltValue

  .. automethod:: tulip::tlp.StringVectorProperty.pushBackNodeEltValue

  .. automethod:: tulip::tlp.StringVectorProperty.popBackNodeEltValue

  .. automethod:: tulip::tlp.StringVectorProperty.resizeNodeValue

  .. automethod:: tulip::tlp.StringVectorProperty.resizeNodeValue
    :noindex:

  .. automethod:: tulip::tlp.StringVectorProperty.getEdgeEltValue

  .. automethod:: tulip::tlp.StringVectorProperty.setEdgeEltValue

  .. automethod:: tulip::tlp.StringVectorProperty.pushBackEdgeEltValue

  .. automethod:: tulip::tlp.StringVectorProperty.popBackEdgeEltValue

  .. automethod:: tulip::tlp.StringVectorProperty.resizeEdgeValue

  .. automethod:: tulip::tlp.StringVectorProperty.resizeEdgeValue
    :noindex:

Tulip plugins classes
----------------------

tlp.WithParameter
^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.WithParameter
	:members:

tlp.Plugin
^^^^^^^^^^

.. autoclass:: tulip::tlp.Plugin
        :members:

tlp.Algorithm
^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.Algorithm
	:members:


tlp.BooleanAlgorithm
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.BooleanAlgorithm
	:members:


tlp.ColorAlgorithm
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ColorAlgorithm
	:members:

tlp.DoubleAlgorithm
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.DoubleAlgorithm
        :members:

tlp.IntegerAlgorithm
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.IntegerAlgorithm
	:members:


tlp.LayoutAlgorithm
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.LayoutAlgorithm
	:members:


tlp.SizeAlgorithm
^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SizeAlgorithm
	:members:


tlp.ImportModule
^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ImportModule
	:members:


tlp.ExportModule
^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ExportModule
	:members:


Graph test classes
-------------------

tlp.SelfLoops
^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SelfLoops
	:members:


tlp.AcyclicTest
^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.AcyclicTest
	:members:


tlp.BiconnectedTest
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.BiconnectedTest
	:members:


tlp.ConnectedTest
^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ConnectedTest
	:members:


tlp.OuterPlanarTest
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.OuterPlanarTest
	:members:


tlp.PlanarityTest
^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.PlanarityTest
	:members:


tlp.SimpleTest
^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SimpleTest
	:members:


tlp.TreeTest
^^^^^^^^^^^^

.. autoclass:: tulip::tlp.TreeTest
	:members:


tlp.TriconnectedTest
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.TriconnectedTest
	:members:

Tulip Tools
-------------------

Random number generation
^^^^^^^^^^^^^^^^^^^^^^^^
There is no need to use Python API to generate random numbers. Use the Tulip dedicated functions to avoid any
side effect with plugins using random numbers (e.g., Louvain, FM^3).

.. automethod:: tulip::tlp.initRandomSequence

.. automethod:: tulip::tlp.setSeedOfRandomSequence

.. automethod:: tulip::tlp.getSeedOfRandomSequence

.. automethod:: tulip::tlp.randomDouble

.. automethod:: tulip::tlp.randomUnsignedInteger

.. automethod:: tulip::tlp.randomInteger
