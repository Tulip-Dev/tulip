.. py:currentmodule:: tulip

Graph/Properties observation
=============================

Tulip offers a mechanism to observe and get informed on modifications
performed on the graph structure, the sub-graphs hierarchy and the properties
values attached to the graph. This is achieved by implementing an object deriving
from the :class:`tlp.Observable`. To get informed on modifications performed on
the graph or properties values, you have to implement the :meth:`tlp.Observable.treatEvent` method.

Observing a graph
-----------------

A special type of event is available to keep track of graph modifications : :class:`tlp.GraphEvent`.
Instances of that type of event are sent by instances of :class:`tlp.Graph`. Call :meth:`tlp.Observable.addListener`
to register an observer to a graph. The sample code below illustrates that observation mechanism::

    from tulip import *

    class GraphObserver(tlp.Observable):
      def __init__(self):
        tlp.Observable.__init__(self)

      def treatEvent(self, event):
        if isinstance(event, tlp.GraphEvent):
          graph = event.getGraph()
          if event.getType() == tlp.GraphEvent.TLP_ADD_NODE:
            print("The node", event.getNode(), "has been added to the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_DEL_NODE:
            print("The node", event.getNode(), "has been deleted from the graph", graph)

          if event.getType() == tlp.GraphEvent.TLP_ADD_EDGE:
            print("The edge", event.getEdge(), "has been added to the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_DEL_EDGE:
            print("The edge", event.getEdge(), "has been deleted from the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_REVERSE_EDGE:
            print("The edge", event.getEdge(), "has been reversed in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_SET_ENDS:
            print("The edge", event.getEdge(), "will have its ends modified. Current source is", graph.source(event.getEdge()),\
                  "Current target is", graph.target(event.getEdge()))

          elif event.getType() == tlp.GraphEvent.TLP_AFTER_SET_ENDS:
            print("The edge", event.getEdge(), "had its ends modified. Current source is", graph.source(event.getEdge()),\
                  "Current target is", graph.target(event.getEdge()))

          elif event.getType() == tlp.GraphEvent.TLP_ADD_NODES:
            print("The nodes", event.getNodes(), "have been added to the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_ADD_EDGES:
            print("The edges", event.getEdges(), "have been added to the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_ADD_DESCENDANTGRAPH:
            print("A descendant graph", event.getSubGraph(), "is about to be added in the sub-graphs hierarchy of graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_AFTER_ADD_DESCENDANTGRAPH:
            print("A descendant graph", event.getSubGraph(), "has been added in the sub-graphs hierarchy of graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_DEL_DESCENDANTGRAPH:
            print("A descendant graph", event.getSubGraph(), "is about to be deleted in the sub-graphs hierarchy of graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_AFTER_DEL_DESCENDANTGRAPH:
            print("A descendant graph", event.getSubGraph(), "has been deleted in the sub-graphs hierarchy of graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_ADD_SUBGRAPH:
            print("A sub-graph", event.getSubGraph(), "is about to be added in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_AFTER_ADD_SUBGRAPH:
            print("A sub-graph", event.getSubGraph(), "has been added in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_DEL_SUBGRAPH:
            print("A sub-graph", event.getSubGraph(), "is about to be deleted in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_AFTER_DEL_SUBGRAPH:
            print("A sub-graph", event.getSubGraph(), "has been deleted in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_ADD_LOCAL_PROPERTY:
            print("A local property", event.getPropertyName(), "is about to be added in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_ADD_LOCAL_PROPERTY:
            print("A local property", event.getPropertyName(), "has been added in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_DEL_LOCAL_PROPERTY:
            print("A local property", event.getPropertyName(), "is about to be deleted in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_AFTER_DEL_LOCAL_PROPERTY:
            print("A local property", event.getPropertyName(), "has been deleted in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_ADD_INHERITED_PROPERTY:
            print("An inherited property", event.getPropertyName(), "is about to be added in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_ADD_INHERITED_PROPERTY:
            print("An inherited property", event.getPropertyName(), "has been added in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_DEL_INHERITED_PROPERTY:
            print("An inherited property", event.getPropertyName(), "is about to be deleted in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_AFTER_DEL_INHERITED_PROPERTY:
            print("An inherited property", event.getPropertyName(), "has been deleted in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_BEFORE_SET_ATTRIBUTE:
            print("An attribute", event.getAttributeName(), "is about to be set/modified in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_AFTER_SET_ATTRIBUTE:
            print("An attribute", event.getAttributeName(), "has been set/modified in the graph", graph)

          elif event.getType() == tlp.GraphEvent.TLP_REMOVE_ATTRIBUTE:
            print("An attribute", event.getAttributeName(), "has been removed in the graph", graph)

    root = tlp.newGraph()
    root.setName("root")
    graph = root.addSubGraph("graph")

    obs = GraphObserver()
    graph.addListener(obs)

    n = graph.addNode()
    n2 = graph.addNode()
    e = graph.addEdge(n, n2)
    graph.reverse(e)
    graph.setEnds(e, n, n2)

    nodes = graph.addNodes(4)
    edges = graph.addEdges([(nodes[0], nodes[1]), (nodes[2], nodes[3])])

    sg = graph.addSubGraph("sg1")
    sg2 = sg.addSubGraph("sg2")

    sg.delSubGraph(sg2)
    graph.delSubGraph(sg)

    prop = graph.getDoubleProperty("metric")
    propRoot = root.getDoubleProperty("metric_root")

    graph.delLocalProperty("metric")
    root.delLocalProperty("metric_root")

    graph.setAttribute("author", "me")
    graph.removeAttribute("author")

    graph.delEdge(e)
    graph.delNode(n)
    graph.delNode(n2)

The output of that script is the following::

    The node <node 0> has been added to the graph <graph "graph" (id 1) >
    The node <node 1> has been added to the graph <graph "graph" (id 1) >
    The edge <edge 0> has been added to the graph <graph "graph" (id 1) >
    The edge <edge 0> has been reversed in the graph <graph "graph" (id 1) >
    The edge <edge 0> will have its ends modified. Current source is <node 0> Current target is <node 1>
    The edge <edge 0> had its ends modified. Current source is <node 0> Current target is <node 1>
    The nodes [<node 2>, <node 3>, <node 4>, <node 5>] have been added to the graph <graph "graph" (id 1) >
    The edges [<edge 1>, <edge 2>] have been added to the graph <graph "graph" (id 1) >
    A sub-graph <graph "sg1" (id 2) > is about to be added in the graph <graph "graph" (id 1) >
    A descendant graph <graph "sg1" (id 2) > is about to be added in the sub-graphs hierarchy of graph <graph "graph" (id 1) >
    A sub-graph <graph "sg1" (id 2) > has been added in the graph <graph "graph" (id 1) >
    A descendant graph <graph "sg1" (id 2) > has been added in the sub-graphs hierarchy of graph <graph "graph" (id 1) >
    A descendant graph <graph "sg2" (id 3) > is about to be added in the sub-graphs hierarchy of graph <graph "graph" (id 1) >
    A descendant graph <graph "sg2" (id 3) > has been added in the sub-graphs hierarchy of graph <graph "graph" (id 1) >
    A descendant graph <graph "sg2" (id 3) > is about to be deleted in the sub-graphs hierarchy of graph <graph "graph" (id 1) >
    A descendant graph <graph "sg2" (id 3) > has been deleted in the sub-graphs hierarchy of graph <graph "graph" (id 1) >
    A sub-graph <graph "sg1" (id 2) > is about to be deleted in the graph <graph "graph" (id 1) >
    A descendant graph <graph "sg1" (id 2) > is about to be deleted in the sub-graphs hierarchy of graph <graph "graph" (id 1) >
    A sub-graph <graph "sg1" (id 2) > has been deleted in the graph <graph "graph" (id 1) >
    A descendant graph <graph "sg1" (id 2) > has been deleted in the sub-graphs hierarchy of graph <graph "graph" (id 1) >
    A local property metric is about to be added in the graph <graph "graph" (id 1) >
    A local property metric has been added in the graph <graph "graph" (id 1) >
    An inherited property metric_root is about to be added in the graph <graph "graph" (id 1) >
    An inherited property metric_root has been added in the graph <graph "graph" (id 1) >
    A local property metric is about to be deleted in the graph <graph "graph" (id 1) >
    A local property metric has been deleted in the graph <graph "graph" (id 1) >
    An inherited property metric_root is about to be deleted in the graph <graph "graph" (id 1) >
    An inherited property metric_root has been deleted in the graph <graph "graph" (id 1) >
    An attribute author is about to be set/modified in the graph <graph "graph" (id 1) >
    An attribute author has been set/modified in the graph <graph "graph" (id 1) >
    An attribute author has been removed in the graph <graph "graph" (id 1) >
    The edge <edge 0> has been deleted from the graph <graph "graph" (id 1) >
    The node <node 0> has been deleted from the graph <graph "graph" (id 1) >
    The node <node 1> has been deleted from the graph <graph "graph" (id 1) >

Observing a graph property
---------------------------

Another special type of event is available to keep track of a property modifications : :class:`tlp.PropertyEvent`.
Instances of that type of event are sent by derived instances of :class:`tlp.PropertyInterface`. Call :meth:`tlp.Observable.addListener`
to register an observer to a property. The sample code below illustrates that observation mechanism::

    from tulip import *

    class PropertyObserver(tlp.Observable):

      def __init__(self):
        tlp.Observable.__init__(self)

      def treatEvent(self, event):
        if isinstance(event, tlp.PropertyEvent):
          prop = event.getProperty()
          if event.getType() == tlp.PropertyEvent.TLP_BEFORE_SET_NODE_VALUE:
            print("Value for node", event.getNode(), "in property", prop, "is about to be modified. Current value is", prop[event.getNode()])
          elif event.getType() == tlp.PropertyEvent.TLP_AFTER_SET_NODE_VALUE:
            print("Value for node", event.getNode(), "in property", prop, "has been modified. New value is", prop[event.getNode()])
          elif event.getType() == tlp.PropertyEvent.TLP_BEFORE_SET_EDGE_VALUE:
            print("Value for edge", event.getEdge(), "in property", prop, "is about to be modified. Current value is", prop[event.getEdge()])
          elif event.getType() == tlp.PropertyEvent.TLP_AFTER_SET_EDGE_VALUE:
            print("Value for edge", event.getEdge(), "in property", prop, "has been modified. New value is", prop[event.getEdge()])
          elif event.getType() == tlp.PropertyEvent.TLP_BEFORE_SET_ALL_NODE_VALUE:
            print("Value for all nodes", "in property", prop, "is about to be modified.")
          elif event.getType() == tlp.PropertyEvent.TLP_AFTER_SET_ALL_NODE_VALUE:
            print("Value for all nodes", "in property", prop, "has been modified.")
          elif event.getType() == tlp.PropertyEvent.TLP_BEFORE_SET_ALL_EDGE_VALUE:
            print("Value for all edges", "in property", prop, "is about to be modified.")
          elif event.getType() == tlp.PropertyEvent.TLP_AFTER_SET_ALL_EDGE_VALUE:
            print("Value for all edges", "in property", prop, "has been modified.")

    graph = tlp.newGraph()
    nodes = graph.addNodes(5)
    edges = graph.addEdges([(nodes[0], nodes[1]), (nodes[2], nodes[3])])

    obs = PropertyObserver()

    metric = graph.getDoubleProperty("metric")
    metric.addListener(obs)


    metric.setAllNodeValue(-1)
    metric.setAllEdgeValue(-1)

    metric[nodes[1]] = 3.2
    metric[edges[0]] = 5.7

The output of that script is the following::

    Value for all nodes in property <Double property metric> is about to be modified.
    Value for all nodes in property <Double property metric> has been modified.
    Value for all edges in property <Double property metric> is about to be modified.
    Value for all edges in property <Double property metric> has been modified.
    Value for node <node 1> in property <Double property metric> is about to be modified. Current value is -1.0
    Value for node <node 1> in property <Double property metric> has been modified. New value is 3.2
    Value for edge <edge 0> in property <Double property metric> is about to be modified. Current value is -1.0
    Value for edge <edge 0> in property <Double property metric> has been modified. New value is 5.7
