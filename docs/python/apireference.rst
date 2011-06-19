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

.. autofunction:: tulip::tlp.applyAlgorithm

.. autofunction:: tulip::tlp.newSubGraph

.. autofunction:: tulip::tlp.newCloneSubGraph

.. autofunction:: tulip::tlp.getSource

.. autofunction:: tulip::tlp.copyToGraph

.. autofunction:: tulip::tlp.removeFromGraph

Graph measures
^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.averagePathLength

.. autofunction:: tulip::tlp.averageClusteringCoefficient

.. autofunction:: tulip::tlp.clusteringCoefficient

.. autofunction:: tulip::tlp.dagLevel

.. autofunction:: tulip::tlp.minDegree

.. autofunction:: tulip::tlp.maxDegree

.. autofunction:: tulip::tlp.maxDistance

.. autofunction:: tulip::tlp.reachableNodes

Plugins Management
^^^^^^^^^^^^^^^^^^

.. autofunction:: tulip::tlp.getTulipRelease

.. autofunction:: tulip::tlp.initTulipLib

.. autofunction:: tulip::tlp.loadPlugins

.. autofunction:: tulip::tlp.loadPlugin

.. autofunction:: tulip::tlp.loadPluginsFromDir

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

.. autoclass:: tulip::tlp.node
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.edge
	:members:
	:undoc-members:

.. _graph-class:

the Graph class
----------------------------

.. autoclass:: tulip::tlp.Graph

	.. _graph-class.structure-modif:
	
	.. rubric:: Modification of the graph structure

	.. automethod:: tulip::tlp.Graph.clear

	.. automethod:: tulip::tlp.Graph.addNode
	
	.. automethod:: tulip::tlp.Graph.addNode	

	.. automethod:: tulip::tlp.Graph.delNode

	.. automethod:: tulip::tlp.Graph.addEdge
	
	.. automethod:: tulip::tlp.Graph.addEdge	

	.. automethod:: tulip::tlp.Graph.delEdge

	.. automethod:: tulip::tlp.Graph.setEdgeOrder

	.. automethod:: tulip::tlp.Graph.swapEdgeOrder
	
	.. automethod:: tulip::tlp.Graph.setSource

	.. automethod:: tulip::tlp.Graph.setTarget

	.. automethod:: tulip::tlp.Graph.setEnds

	.. automethod:: tulip::tlp.Graph.reverse
	
	.. _graph-class.subgraphs-hierarchy:

	.. rubric:: Creation and Modification of the sub-graphs hierarchy

	.. automethod:: tulip::tlp.Graph.addSubGraph

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

	.. automethod:: tulip::tlp.Graph.getDescendantGraph

	.. _graph-class.iterators:
	
	.. rubric:: Iterators on the graph structure

	.. automethod:: tulip::tlp.Graph.getOneNode

	.. automethod:: tulip::tlp.Graph.getNodes

	.. automethod:: tulip::tlp.Graph.getInNode

	.. automethod:: tulip::tlp.Graph.getInNodes

	.. automethod:: tulip::tlp.Graph.getOutNode

	.. automethod:: tulip::tlp.Graph.getOutNodes

	.. automethod:: tulip::tlp.Graph.getInOutNodes

	.. automethod:: tulip::tlp.Graph.getNodeMetaInfo

	.. automethod:: tulip::tlp.Graph.getOneEdge

	.. automethod:: tulip::tlp.Graph.getEdges

	.. automethod:: tulip::tlp.Graph.getOutEdges

	.. automethod:: tulip::tlp.Graph.getInOutEdges

	.. automethod:: tulip::tlp.Graph.getInEdges

	.. automethod:: tulip::tlp.Graph.getEdgeMetaInfo
	
	.. _graph-class.informations:

	.. rubric:: Graph, nodes and edges informations about the graph stucture

	.. automethod:: tulip::tlp.Graph.getId

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
	
	.. _graph-class.properties:

	.. rubric:: Access and compute graph properties

	.. automethod:: tulip::tlp.Graph.getBooleanProperty

	.. automethod:: tulip::tlp.Graph.getLocalBooleanProperty

	.. automethod:: tulip::tlp.Graph.computeBooleanProperty

	.. automethod:: tulip::tlp.Graph.getBooleanVectorProperty

	.. automethod:: tulip::tlp.Graph.getLocalBooleanVectorProperty

	.. automethod:: tulip::tlp.Graph.getColorProperty

	.. automethod:: tulip::tlp.Graph.getLocalColorProperty

	.. automethod:: tulip::tlp.Graph.computeColorProperty

	.. automethod:: tulip::tlp.Graph.getColorVectorProperty

	.. automethod:: tulip::tlp.Graph.getLocalColorVectorProperty

	.. automethod:: tulip::tlp.Graph.getDoubleProperty

	.. automethod:: tulip::tlp.Graph.getLocalDoubleProperty

	.. automethod:: tulip::tlp.Graph.computeDoubleProperty

	.. automethod:: tulip::tlp.Graph.getDoubleVectorProperty

	.. automethod:: tulip::tlp.Graph.getLocalDoubleVectorProperty

	.. automethod:: tulip::tlp.Graph.getIntegerProperty

	.. automethod:: tulip::tlp.Graph.getLocalIntegerProperty

	.. automethod:: tulip::tlp.Graph.computeIntegerProperty

	.. automethod:: tulip::tlp.Graph.getIntegerVectorProperty

	.. automethod:: tulip::tlp.Graph.getLocalIntegerVectorProperty	

	.. automethod:: tulip::tlp.Graph.getGraphProperty

	.. automethod:: tulip::tlp.Graph.getLocalGraphProperty

	.. automethod:: tulip::tlp.Graph.getLayoutProperty

	.. automethod:: tulip::tlp.Graph.getLocalLayoutProperty

	.. automethod:: tulip::tlp.Graph.computeLayoutProperty

	.. automethod:: tulip::tlp.Graph.getCoordVectorProperty

	.. automethod:: tulip::tlp.Graph.getLocalCoordVectorProperty

	.. automethod:: tulip::tlp.Graph.getSizeProperty

	.. automethod:: tulip::tlp.Graph.getLocalSizeProperty

	.. automethod:: tulip::tlp.Graph.computeSizeProperty

	.. automethod:: tulip::tlp.Graph.getSizeVectorProperty

	.. automethod:: tulip::tlp.Graph.getLocalSizeVectorProperty

	.. automethod:: tulip::tlp.Graph.getStringProperty

	.. automethod:: tulip::tlp.Graph.getLocalStringProperty

	.. automethod:: tulip::tlp.Graph.computeStringProperty

	.. automethod:: tulip::tlp.Graph.getStringVectorProperty

	.. automethod:: tulip::tlp.Graph.getLocalStringVectorProperty

	.. automethod:: tulip::tlp.Graph.getProperty

	.. automethod:: tulip::tlp.Graph.existProperty

	.. automethod:: tulip::tlp.Graph.existLocalProperty

	.. automethod:: tulip::tlp.Graph.delLocalProperty

	.. automethod:: tulip::tlp.Graph.getLocalProperties

	.. automethod:: tulip::tlp.Graph.getInheritedProperties

	.. automethod:: tulip::tlp.Graph.getProperties
	
	.. rubric:: Update management

	.. automethod:: tulip::tlp.Graph.push

	.. automethod:: tulip::tlp.Graph.pop

	.. automethod:: tulip::tlp.Graph.unpop

	.. automethod:: tulip::tlp.Graph.canPop

	.. automethod:: tulip::tlp.Graph.canUnpop

	.. automethod:: tulip::tlp.Graph.canPopThenUnpop()

	.. rubric:: Meta-nodes management

	.. automethod:: tulip::tlp.Graph.createMetaNode

	.. automethod:: tulip::tlp.Graph.createMetaNodes

	.. automethod:: tulip::tlp.Graph.createMetaNode

	.. automethod:: tulip::tlp.Graph.openMetaNode

Tulip datatypes
---------------

.. autoclass:: tulip::tlp.Vec3f
	:members:
	:undoc-members:
	
.. autoclass:: tulip::tlp.Vec4f

.. autoclass:: tulip::tlp.Vec4i

.. autoclass:: tulip::tlp.Coord
	:members:
	:undoc-members:
	
.. autoclass:: tulip::tlp.Size
	:members:
	:undoc-members:
	
.. autoclass:: tulip::tlp.Color
	:members:
	:undoc-members:		

Graph properties classes
----------------------------

.. autoclass:: tulip::tlp.PropertyInterface
	:members:
	:undoc-members:
	
.. autoclass:: tulip::tlp.BooleanProperty
	:members: getNodesEqualTo, getEdgesEqualTo, reverse, reverseEdgeDirection, 
	          getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue
	          
.. autoclass:: tulip::tlp.ColorProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue	          	
	
.. autoclass:: tulip::tlp.DoubleProperty
	:members: getNodeMin, getNodeMax, getEdgeMin, getEdgeMax, 
	          uniformQuantification, nodesUniformQuantification, edgesUniformQuantification, 
	          getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue

.. autoclass:: tulip::tlp.IntegerProperty
	:members: getNodeMin, getNodeMax, getEdgeMin, getEdgeMax,
			  getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue
	          
.. autoclass:: tulip::tlp.LayoutProperty
	:members: angularResolutions, averageAngularResolution, averageAngularResolution,
			  center, computeEmbedding, computeEmbedding, crossingNumber,
			  edgeLength, getMin, getMax, normalize, perfectAspectRatio, 
			  rotateX, rotateY, rotateZ, rotateZ, scale, scale, translate, translate,      
			  getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue 

.. autoclass:: tulip::tlp.SizeProperty
	:members: getMin, getMax, scale, scale,
			  getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue

.. autoclass:: tulip::tlp.StringProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue
	          
.. autoclass:: tulip::tlp.BooleanVectorProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue
	          
.. autoclass:: tulip::tlp.CoordVectorProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue	          
	          
.. autoclass:: tulip::tlp.ColorVectorProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue 

.. autoclass:: tulip::tlp.DoubleVectorProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue
	          
.. autoclass:: tulip::tlp.IntegerVectorProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue



.. autoclass:: tulip::tlp.SizeVectorProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

.. autoclass:: tulip::tlp.StringVectorProperty
	:members: getEdgeDefaultValue, getEdgeValue, getNodeDefaultValue, getNodeValue, 
	          setAllEdgeValue, setAllNodeValue, setEdgeValue, setNodeValue,
	          getNodeEltValue, setNodeEltValue, pushBackNodeEltValue, popBackNodeEltValue,
	          resizeNodeValue, resizeNodeValue, getEdgeEltValue, setEdgeEltValue,  
	          pushBackEdgeEltValue, popBackEdgeEltValue, resizeEdgeValue, resizeEdgeValue

Graph test classes
-------------------

.. autoclass:: tulip::tlp.SelfLoops
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.AcyclicTest
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.BiconnectedTest
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.ConnectedTest
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.OuterPlanarTest
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.PlanarityTest
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.SimpleTest
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.TreeTest
	:members:
	:undoc-members:

.. autoclass:: tulip::tlp.TriconnectedTest
	:members:
	:undoc-members:
