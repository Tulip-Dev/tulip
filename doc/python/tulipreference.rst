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
    :noindex:

.. autofunction:: tulip::tlp.maxDistanceWeighted
    :noindex:

.. autofunction:: tulip::tlp.reachableNodes
    :noindex:

.. autofunction:: tulip::tlp.bfs
    :noindex:

.. autofunction:: tulip::tlp.bfs
    :noindex:

.. autofunction:: tulip::tlp.dfs
    :noindex:

.. autofunction:: tulip::tlp.dfs
    :noindex:

.. autofunction:: tulip::tlp.selectShortestPaths
    :noindex:

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
    :noindex:

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

.. automethod:: tulip::tlp.Graph.isElement

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

.. automethod:: tulip::tlp.Graph.applyColorAlgorithm

.. automethod:: tulip::tlp.Graph.applyColorAlgorithm

.. automethod:: tulip::tlp.Graph.applyDoubleAlgorithm

.. automethod:: tulip::tlp.Graph.applyDoubleAlgorithm

.. automethod:: tulip::tlp.Graph.applyIntegerAlgorithm

.. automethod:: tulip::tlp.Graph.applyLayoutAlgorithm

.. automethod:: tulip::tlp.Graph.applyLayoutAlgorithm

.. automethod:: tulip::tlp.Graph.applySizeAlgorithm

.. automethod:: tulip::tlp.Graph.applySizeAlgorithm

.. automethod:: tulip::tlp.Graph.applyStringAlgorithm

.. automethod:: tulip::tlp.Graph.applyStringAlgorithm

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
	:members: at, empty, getCurrent, getCurrentString, push_back, setCurrent, setCurrent, size

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
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, reverse, reverseEdgeDirection,
	          getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllEdgeValue, setAllNodeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.ColorProperty
^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ColorProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
                  setAllEdgeValue, setAllEdgeValue, setAllNodeValue, setAllNodeValue,
                  setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

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
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getNodeMin, getNodeMax, getEdgeMin, getEdgeMax,
	          getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.IntegerProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.IntegerProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getNodeMin, getNodeMax, getEdgeMin, getEdgeMax,
                  getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
                  setAllEdgeValue, setAllNodeValue,
                  setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.LayoutProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.LayoutProperty
	:members: angularResolutions, averageAngularResolution, averageAngularResolution,
            averageEdgeLength, center, center, computeEmbedding, computeEmbedding, copy, copy, copy,
            getNodesEqualTo, getEdgesEqualTo, edgeLength, getMin, getMax, normalize, perfectAspectRatio,
            rotateX, rotateX, rotateY, rotateY, rotateZ, rotateZ, scale, scale, translate, translate,
            getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.SizeProperty
^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SizeProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getMin, getMax, scale, scale,
                  getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
                  setAllEdgeValue, setAllNodeValue,
                  setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.StringProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.StringProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
                  setAllEdgeValue, setAllNodeValue,
                  setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue

tlp.BooleanVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.BooleanVectorProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.CoordVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.CoordVectorProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.ColorVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ColorVectorProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.DoubleVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.DoubleVectorProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.IntegerVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.IntegerVectorProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllEdgeValue, setAllNodeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.SizeVectorProperty
^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SizeVectorProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.StringVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.StringVectorProperty
        :members: copy, copy, copy, getNodesEqualTo, getEdgesEqualTo, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
            setAllEdgeValue, setAllNodeValue,
            setValueToGraphNodes, setValueToGraphEdges, setEdgeValue, setNodeValue, setNodeDefaultValue, setEdgeDefaultValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

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
