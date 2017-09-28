# This file is part of Tulip (http://tulip.labri.fr)
#
# Authors: David Auber and the Tulip development Team
# from LaBRI, University of Bordeaux
#
# Tulip is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3
# of the License, or (at your option) any later version.
#
# Tulip is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.

import unittest

from tulip import tlp

NB_NODES = 5
NB_EDGES = 2 * (NB_NODES - 1)

class TestGraphStructure(unittest.TestCase):

  # generate a simple graph with a cycle going from node 0 to node (NB_NODES-2)
  # and node (NB_NODES-1) connected to all the others
  def setUp(self):
    self.graph = tlp.newGraph()
    self.nodes = []
    self.edges = []
    for i in range(NB_NODES):
      self.nodes.append(self.graph.addNode())

    for i in range(NB_NODES - 1):
      if i != NB_NODES - 2:
        self.edges.append(self.graph.addEdge(self.nodes[i], self.nodes[i+1]))
      else:
        self.edges.append(self.graph.addEdge(self.nodes[i], self.nodes[0]))
      self.edges.append(self.graph.addEdge(self.nodes[i], self.nodes[-1]))


  def tearDown(self):
    self.graph = None
    self.nodes = None
    self.edges = None


  def test_nb_elements(self):
    self.assertEqual(self.graph.numberOfNodes(), NB_NODES)
    self.assertEqual(self.graph.numberOfEdges(), NB_EDGES)
    self.assertEqual(len(list(self.graph.getNodes())), NB_NODES)
    self.assertEqual(len(list(self.graph.getEdges())), NB_EDGES)
    self.assertEqual(len(self.graph.nodes()), NB_NODES)
    self.assertEqual(len(self.graph.edges()), NB_EDGES)


  def test_is_element(self):
    for n in self.nodes:
      self.assertTrue(self.graph.isElement(n))

    for e in self.edges:
      self.assertTrue(self.graph.isElement(e))

    for n in self.graph.getNodes():
      self.assertIn(n, self.nodes)

    for n in self.graph.nodes():
      self.assertIn(n, self.nodes)

    for e in self.graph.getEdges():
      self.assertIn(e, self.edges)
      self.assertIn(self.graph.source(e), self.nodes)
      self.assertIn(self.graph.target(e), self.nodes)
      ends = self.graph.ends(e)
      self.assertIn(ends[0], self.nodes)
      self.assertIn(ends[1], self.nodes)

    for e in self.graph.edges():
      self.assertIn(e, self.edges)
      self.assertIn(self.graph.source(e), self.nodes)
      self.assertIn(self.graph.target(e), self.nodes)

    for i in range(NB_NODES - 1):
      self.assertTrue(self.graph.hasEdge(self.nodes[i], self.nodes[i+1]))
      self.assertIn(self.graph.existEdge(self.nodes[i], self.nodes[i+1]), self.edges)
      self.assertTrue(self.graph.hasEdge(self.nodes[i], self.nodes[-1]))
      self.assertIn(self.graph.existEdge(self.nodes[i], self.nodes[-1]), self.edges)

    self.assertFalse(self.graph.isElement(tlp.node(NB_NODES)))
    self.assertFalse(self.graph.isElement(tlp.edge(NB_EDGES)))

  def test_node_degrees(self):
    for i in range(NB_NODES-1):
      self.assertEqual(self.graph.deg(self.nodes[i]), 3)
      self.assertEqual(self.graph.indeg(self.nodes[i]), 1)
      self.assertEqual(self.graph.outdeg(self.nodes[i]), 2)

    self.assertEqual(self.graph.deg(self.nodes[NB_NODES-1]), NB_NODES-1)
    self.assertEqual(self.graph.indeg(self.nodes[NB_NODES-1]), NB_NODES-1)
    self.assertEqual(self.graph.outdeg(self.nodes[NB_NODES-1]), 0)

  def test_add_invalid_edge(self):
    with self.assertRaises(Exception) as cm:
      self.graph.addEdge(tlp.node(NB_NODES), tlp.node(NB_NODES+1))
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

  def test_del_elements(self):

    with self.assertRaises(Exception) as cm:
      self.graph.delNode(tlp.node(NB_NODES))
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.delEdge(tlp.edge(NB_EDGES))
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))

    for i, e in enumerate(self.edges):
      self.graph.delEdge(e)
      self.assertEqual(self.graph.numberOfEdges(), NB_EDGES - (i+1))
      self.assertFalse(self.graph.isElement(e))

    for i, n in enumerate(self.nodes):
      self.graph.delNode(n)
      self.assertEqual(self.graph.numberOfNodes(), NB_NODES - (i+1))
      self.assertFalse(self.graph.isElement(n))


  def test_del_nodes_from_list(self):

    invalid_nodes=[tlp.node(NB_NODES), tlp.node(NB_NODES+1)]

    with self.assertRaises(Exception) as cm:
      self.graph.delNodes(invalid_nodes)
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    self.graph.delNodes(self.nodes)
    self.assertEqual(self.graph.numberOfNodes(), 0)
    self.assertEqual(self.graph.numberOfEdges(), 0)


  def test_del_nodes_from_iterator(self):
    self.graph.delNodes(self.graph.getNodes())
    self.assertEqual(self.graph.numberOfNodes(), 0)
    self.assertEqual(self.graph.numberOfEdges(), 0)


  def test_del_edges_from_list(self):

    invalid_edges=[tlp.edge(NB_EDGES), tlp.edge(NB_EDGES+1)]

    with self.assertRaises(Exception) as cm:
      self.graph.delEdges(invalid_edges)
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))

    self.graph.delEdges(self.edges)
    self.assertEqual(self.graph.numberOfEdges(), 0)


  def test_del_edges_from_iterator(self):
    self.graph.delEdges(self.graph.getEdges())
    self.assertEqual(self.graph.numberOfEdges(), 0)


  def test_add_then_del_nodes(self):
    new_nodes = self.graph.addNodes(NB_NODES)
    self.assertEqual(self.graph.numberOfNodes(), 2*NB_NODES)

    for n in new_nodes:
      self.assertTrue(self.graph.isElement(n))

    self.graph.delNodes(new_nodes)
    self.assertEqual(self.graph.numberOfNodes(), NB_NODES)

    for n in new_nodes:
      self.assertFalse(self.graph.isElement(n))


  def test_add_then_del_edges(self):

    invalid_edges=[(tlp.node(NB_NODES), tlp.node(NB_NODES+1)),
                   (tlp.node(NB_NODES+1), tlp.node(NB_NODES+2))]

    with self.assertRaises(Exception) as cm:
      self.graph.addEdges(invalid_edges)
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    edges_to_add = []
    for i in range(NB_EDGES):
      edge = (self.graph.getRandomNode(), self.graph.getRandomNode())
      edges_to_add.append(edge)

    new_edges = self.graph.addEdges(edges_to_add)
    self.assertEqual(self.graph.numberOfEdges(), 2*NB_EDGES)

    for i, e in enumerate(new_edges):
      self.assertTrue(self.graph.isElement(e))
      self.assertEqual(self.graph.source(e), edges_to_add[i][0])
      self.assertEqual(self.graph.target(e), edges_to_add[i][1])

    self.graph.delEdges(new_edges)
    self.assertEqual(self.graph.numberOfEdges(), NB_EDGES)

    for n in new_edges:
      self.assertFalse(self.graph.isElement(e))


  def test_add_invalid_node_or_edge_to_root_graph(self):
    with self.assertRaises(Exception) as cm:
      self.graph.addNode(tlp.node(NB_NODES))
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.addEdge(tlp.edge(NB_EDGES))
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))


  def test_edge_orders(self):
    with self.assertRaises(Exception) as cm:
      self.graph.setEdgeOrder(tlp.node(NB_NODES), [])
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.swapEdgeOrder(tlp.node(NB_NODES), tlp.edge(), tlp.edge())
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.swapEdgeOrder(self.graph.getRandomNode(), tlp.edge(NB_EDGES), tlp.edge(NB_EDGES+1))
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))

    for n in self.graph.getNodes():
      edges = list(self.graph.getInOutEdges(n))
      edges = list(reversed(edges))
      self.graph.setEdgeOrder(n, edges)
      self.assertEqual(list(self.graph.getInOutEdges(n)), edges)
      self.assertEqual(self.graph.allEdges(n), edges)

      self.graph.swapEdgeOrder(n, edges[0], edges[-1])
      edges[0], edges[-1] = edges[-1], edges[0]
      self.assertEqual(list(self.graph.getInOutEdges(n)), edges)
      self.assertEqual(self.graph.allEdges(n), edges)


  def test_edge_extremities_modification(self):

    with self.assertRaises(Exception) as cm:
      self.graph.ends(tlp.edge(NB_EDGES))
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.reverse(tlp.edge(NB_EDGES))
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.setSource(tlp.edge(NB_EDGES), tlp.node())
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.setTarget(tlp.edge(NB_EDGES), tlp.node())
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.setSource(self.graph.getRandomEdge(), tlp.node(NB_NODES))
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.setTarget(self.graph.getRandomEdge(), tlp.node(NB_NODES))
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.setEnds(tlp.edge(NB_EDGES), tlp.node(), tlp.node())
      self.assertEquals(cm.exception.args[0],
                        'Edge with id %s does not belong to graph "%s" (id %s)' %
                        (NB_EDGES, self.graph.getName(), self.graph.getId()))

    with self.assertRaises(Exception) as cm:
      self.graph.setEnds(self.graph.getRandomEdge(), tlp.node(NB_NODES), tlp.node(NB_NODES+1))
      self.assertEquals(cm.exception.args[0],
                        'Node with id %s does not belong to graph "%s" (id %s)' %
                        (NB_NODES, self.graph.getName(), self.graph.getId()))

    for e in self.graph.edges():
      ends = self.graph.ends(e)
      self.graph.reverse(e)
      self.assertEqual(self.graph.source(e), ends[1])
      self.assertEqual(self.graph.target(e), ends[0])

      new_src = self.graph.getRandomNode()
      new_tgt = self.graph.getRandomNode()
      self.graph.setSource(e, new_src)
      self.graph.setTarget(e, new_tgt)
      self.assertEqual(self.graph.source(e), new_src)
      self.assertEqual(self.graph.target(e), new_tgt)
      self.assertEqual(self.graph.ends(e), (new_src, new_tgt))

      new_src = self.graph.getRandomNode()
      new_tgt = self.graph.getRandomNode()
      self.graph.setEnds(e, new_src, new_tgt)
      self.assertEqual(self.graph.source(e), new_src)
      self.assertEqual(self.graph.target(e), new_tgt)
      self.assertEqual(self.graph.ends(e), (new_src, new_tgt))
