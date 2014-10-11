.. py:currentmodule:: tulip

:mod:`tulip` module API    
==========================

Utility functions
-------------------

Graph management
^^^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.newGraph

.. autofunction:: tulip::tlp.loadGraph

.. autofunction:: tulip::tlp.saveGraph

.. autofunction:: tulip::tlp.importGraph

.. autofunction:: tulip::tlp.exportGraph

.. autofunction:: tulip::tlp.copyToGraph

.. autofunction:: tulip::tlp.removeFromGraph

Graph measures and functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.averagePathLength

.. autofunction:: tulip::tlp.averageClusteringCoefficient

.. autofunction:: tulip::tlp.clusteringCoefficient

.. autofunction:: tulip::tlp.dagLevel

.. autofunction:: tulip::tlp.minDegree

.. autofunction:: tulip::tlp.maxDegree

.. autofunction:: tulip::tlp.maxDistance

.. autofunction:: tulip::tlp.reachableNodes

.. autofunction:: tulip::tlp.bfs

.. autofunction:: tulip::tlp.dfs

Geometry
^^^^^^^^

.. autofunction:: tulip::tlp.computeBoundingBox

.. autofunction:: tulip::tlp.computeBoundingBox

.. autofunction:: tulip::tlp.computeBoundingRadius

.. autofunction:: tulip::tlp.computeBoundingRadius

.. autofunction:: tulip::tlp.computeConvexHull

.. autofunction:: tulip::tlp.computeConvexHull

.. autofunction:: tulip::tlp.computeConvexHull

.. autofunction:: tulip::tlp.convexHull2D

.. autofunction:: tulip::tlp.convexHull

.. autofunction:: tulip::tlp.computeLinesIntersection

.. autofunction:: tulip::tlp.computePolygonCentroid

.. autofunction:: tulip::tlp.delaunayTriangulation

.. autoclass:: tulip::tlp.VoronoiDiagram
    :members:

.. autofunction:: tulip::tlp.voronoiDiagram

Plugins Management
^^^^^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.getTulipRelease

.. autofunction:: tulip::tlp.initTulipLib

.. autofunction:: tulip::tlp.loadPlugins

.. autofunction:: tulip::tlp.loadPluginsFromDir

.. autofunction:: tulip::tlp.loadPlugin

.. autofunction:: tulip::tlp.loadPluginsCheckDependencies

.. autofunction:: tulip::tlp.getDefaultPluginParameters

.. autofunction:: tulip::tlp.getImportPluginsList

.. autofunction:: tulip::tlp.getExportPluginsList

.. autofunction:: tulip::tlp.getAlgorithmPluginsList

.. autofunction:: tulip::tlp.getBooleanAlgorithmPluginsList

.. autofunction:: tulip::tlp.getColorAlgorithmPluginsList

.. autofunction:: tulip::tlp.getDoubleAlgorithmPluginsList

.. autofunction:: tulip::tlp.getIntegerAlgorithmPluginsList

.. autofunction:: tulip::tlp.getLayoutAlgorithmPluginsList

.. autofunction:: tulip::tlp.getSizeAlgorithmPluginsList

.. autofunction:: tulip::tlp.getStringAlgorithmPluginsList

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

.. automethod:: tulip::tlp.Graph.addNode

.. automethod:: tulip::tlp.Graph.addNodes
	
.. automethod:: tulip::tlp.Graph.addNode

.. automethod:: tulip::tlp.Graph.addNodes

.. automethod:: tulip::tlp.Graph.delNode

.. automethod:: tulip::tlp.Graph.delNodes

.. automethod:: tulip::tlp.Graph.addEdge

.. automethod:: tulip::tlp.Graph.addEdges
	
.. automethod:: tulip::tlp.Graph.addEdge

.. automethod:: tulip::tlp.Graph.addEdges

.. automethod:: tulip::tlp.Graph.delEdge

.. automethod:: tulip::tlp.Graph.delEdges

.. automethod:: tulip::tlp.Graph.setEdgeOrder

.. automethod:: tulip::tlp.Graph.swapEdgeOrder
	
.. automethod:: tulip::tlp.Graph.setSource

.. automethod:: tulip::tlp.Graph.setTarget

.. automethod:: tulip::tlp.Graph.setEnds

.. automethod:: tulip::tlp.Graph.reverse
	
.. _graph-class.subgraphs-hierarchy:

Creation and Modification of the sub-graphs hierarchy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.addSubGraph

.. automethod:: tulip::tlp.Graph.addSubGraph

.. automethod:: tulip::tlp.Graph.addCloneSubGraph

.. automethod:: tulip::tlp.Graph.inducedSubGraph

.. automethod:: tulip::tlp.Graph.delSubGraph

.. automethod:: tulip::tlp.Graph.delAllSubGraphs

.. automethod:: tulip::tlp.Graph.getSuperGraph

.. automethod:: tulip::tlp.Graph.getRoot

.. automethod:: tulip::tlp.Graph.setSuperGraph

.. automethod:: tulip::tlp.Graph.getSubGraphs

.. automethod:: tulip::tlp.Graph.isSubGraph

.. automethod:: tulip::tlp.Graph.isDescendantGraph

.. automethod:: tulip::tlp.Graph.getSubGraph

.. automethod:: tulip::tlp.Graph.getSubGraph

.. automethod:: tulip::tlp.Graph.getDescendantGraph

.. automethod:: tulip::tlp.Graph.getNthSubGraph

.. automethod:: tulip::tlp.Graph.numberOfSubGraphs

.. automethod:: tulip::tlp.Graph.numberOfDescendantGraphs

.. automethod:: tulip::tlp.Graph.getDescendantGraphs

.. _graph-class.iterators:

Iterators on the graph structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All the provided iterators are stable, meaning you can safely delete a node/edge/subgraph
from the graph while iterating on its nodes/edges/subgraphs.

.. automethod:: tulip::tlp.Graph.getOneNode

.. automethod:: tulip::tlp.Graph.getNodes

.. automethod:: tulip::tlp.Graph.getInNode

.. automethod:: tulip::tlp.Graph.getInNodes

.. automethod:: tulip::tlp.Graph.getOutNode

.. automethod:: tulip::tlp.Graph.getOutNodes

.. automethod:: tulip::tlp.Graph.getInOutNodes

.. automethod:: tulip::tlp.Graph.bfs

.. automethod:: tulip::tlp.Graph.dfs

.. automethod:: tulip::tlp.Graph.getNodeMetaInfo

.. automethod:: tulip::tlp.Graph.getOneEdge

.. automethod:: tulip::tlp.Graph.getEdges

.. automethod:: tulip::tlp.Graph.getOutEdges

.. automethod:: tulip::tlp.Graph.getInOutEdges

.. automethod:: tulip::tlp.Graph.getInEdges

.. automethod:: tulip::tlp.Graph.getEdgeMetaInfo

.. _graph-class.informations:

Informations about the graph stucture
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.getId

.. automethod:: tulip::tlp.Graph.getName

.. automethod:: tulip::tlp.Graph.setName

.. automethod:: tulip::tlp.Graph.getAttributes

.. automethod:: tulip::tlp.Graph.getAttribute

.. automethod:: tulip::tlp.Graph.setAttribute

.. automethod:: tulip::tlp.Graph.removeAttribute

.. automethod:: tulip::tlp.Graph.attributeExist

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

.. automethod:: tulip::tlp.Graph.getSource

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

Application of algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.applyAlgorithm

.. automethod:: tulip::tlp.Graph.applyBooleanAlgorithm

.. automethod:: tulip::tlp.Graph.applyColorAlgorithm

.. automethod:: tulip::tlp.Graph.applyDoubleAlgorithm

.. automethod:: tulip::tlp.Graph.applyIntegerAlgorithm

.. automethod:: tulip::tlp.Graph.applyLayoutAlgorithm

.. automethod:: tulip::tlp.Graph.applySizeAlgorithm

.. automethod:: tulip::tlp.Graph.applyStringAlgorithm

Update management
^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.push

.. automethod:: tulip::tlp.Graph.pop

.. automethod:: tulip::tlp.Graph.unpop

.. automethod:: tulip::tlp.Graph.canPop

.. automethod:: tulip::tlp.Graph.canUnpop

.. automethod:: tulip::tlp.Graph.canPopThenUnpop()

Meta-nodes management
^^^^^^^^^^^^^^^^^^^^^

.. automethod:: tulip::tlp.Graph.createMetaNode

.. automethod:: tulip::tlp.Graph.createMetaNodes

.. automethod:: tulip::tlp.Graph.createMetaNode

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
	:members:


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
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue

tlp.ColorProperty
^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ColorProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue	          	

tlp.NumericProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.NumericProperty
        :members:
                  uniformQuantification, nodesUniformQuantification, edgesUniformQuantification,
                  getEdgeDoubleMin, getEdgeDoubleMax, getNodeDoubleMin, getNodeDoubleMax,
                  getEdgeDoubleDefaultValue, getEdgeDoubleValue, getNodeDoubleDefaultValue, getNodeDoubleValue

tlp.DoubleProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.DoubleProperty
        :members: copy, copy, copy, getNodeMin, getNodeMax, getEdgeMin, getEdgeMax,
	          getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue

tlp.IntegerProperty
^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.IntegerProperty
        :members: copy, copy, copy, getNodeMin, getNodeMax, getEdgeMin, getEdgeMax,
                  getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue

tlp.LayoutProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.LayoutProperty
	:members: angularResolutions, averageAngularResolution, averageAngularResolution,
                  averageEdgeLength, center, center, computeEmbedding, computeEmbedding, copy, copy, copy,
		  edgeLength, getMin, getMax, normalize, perfectAspectRatio, 
		  rotateX, rotateX, rotateY, rotateY, rotateZ, rotateZ, scale, scale, translate, translate,      
		  getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue 

tlp.SizeProperty
^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SizeProperty
        :members: copy, copy, copy, getMin, getMax, scale, scale,
                  getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue

tlp.StringProperty
^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.StringProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue

tlp.BooleanVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.BooleanVectorProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.CoordVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^
	          
.. autoclass:: tulip::tlp.CoordVectorProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue	          

tlp.ColorVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.ColorVectorProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue 

tlp.DoubleVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.DoubleVectorProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.IntegerVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^
	          
.. autoclass:: tulip::tlp.IntegerVectorProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.SizeVectorProperty
^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.SizeVectorProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

tlp.StringVectorProperty
^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: tulip::tlp.StringVectorProperty
        :members: copy, copy, copy, getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue,
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
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

