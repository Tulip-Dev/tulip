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

from tulip import tlp
import tulipplugins

numpyOk = True

# h3 module requires numpy so we need to check it is available
# in the Python environment
try:
    # https://github.com/buzzfeed/pyh3
    from h3.tree import Tree
except Exception:
    numpyOk = False


class H3Layout(tlp.LayoutAlgorithm):

    def __init__(self, context):
        tlp.LayoutAlgorithm.__init__(self, context)

        self.addFloatParameter(
            'layout scaling',
            'the scale factor to apply to the computed layout', '1000')

    def check(self):

        if not numpyOk:
            return (False,
                    ('Python numpy module is required to execute that '
                     'layout algorithm.\nYou can easily install it with '
                     'the pip tool or your package manager if you are a '
                     'linux user.'))

        if not tlp.ConnectedTest.isConnected(self.graph):
            return (False, 'The graph must be connected')

        return (True, '')

    def run(self):

        # compute a spanning tree of the input graph
        spanningTree = tlp.TreeTest.computeTree(self.graph)

        # get edges to use an input for the H3 layout implementation
        # from buzzfeed and reverse their orientation as it does not
        # use the same direction as in Tulip graphs
        edges = [self.graph.ends(e)[::-1] for e in spanningTree.getEdges()]

        # compute the layout
        tree = Tree(edges)

        # copy result to Tulip layout property
        self.result.setAllEdgeValue([])
        for n in self.graph.getNodes():
            self.result[n] = (tree.nodes[n].coord.x,
                              tree.nodes[n].coord.y,
                              tree.nodes[n].coord.z)

        # apply some scaling factor to the layout to get a correct rendering
        # in Tulip
        scaling = 1000
        if self.dataSet:
            scaling = self.dataSet['layout scaling']

        self.result.scale((scaling, scaling, scaling))

        # cleanup computed spanning tree
        tlp.TreeTest.cleanComputedTree(self.graph, spanningTree)

        return True


pluginDoc = """
Implements the H3 layout technique for drawing large directed graphs as
node-link diagrams in 3D hyperbolic space. That algorithm can lay out much
larger structures than can be handled using traditional techniques
for drawing general graphs because it assumes a hierarchical nature
of the data.
It was first published as:<br/><b> H3: Laying out Large Directed Graphs
in 3D Hyperbolic Space</b>, Tamara Munzner, Proceedings of the 1997 IEEE
Symposium on Information Visualization, Phoenix, AZ, pp 2-10, 1997.<br/>
The implementation in Python (MIT License) has been written by
BuzzFeed engineers (https://github.com/buzzfeed/pyh3).
"""

# The line below does the magic to register the plugin into the plugin database
# and updates the GUI to make it accessible through the menus.
tulipplugins.registerPluginOfGroup(
    'H3Layout', 'H3', 'Antoine Lambert', '30/08/2017',
    pluginDoc, '1.0', 'Hierarchical')
