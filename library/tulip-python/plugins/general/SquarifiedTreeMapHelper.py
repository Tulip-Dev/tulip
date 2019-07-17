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

# Simple Python plugin to ease the configuration of a Treemap visualization
# in the main Tulip GUI. As the treemap layout is different from classical
# node link diagram representation, some visual properties setup has to be
# done in order to get an aesthetic visualization of it in the main Tulip GUI.
# Notably, edge colors must be set to full transparency to avoid rendering
# artifacts, node shapes must be set to square, node border colors must be
# adjusted (everything but black) and node border widths must be visible
# (different from 0).

from tulip import tlp
import tulipplugins


class SquarifiedTreeMapHelper(tlp.Algorithm):

    def __init__(self, context):
        tlp.Algorithm.__init__(self, context)

        self.addNumericPropertyParameter(
            'metric',
            ('An optional metric used to estimate the size allocated '
             'to each node'), '', False)

        self.addFloatParameter(
            'aspect ratio',
            ('The aspect ratio (height/width) for the rectangle corresponding'
             ' to the root node'), '1')

        self.addBooleanParameter(
            'treemap type',
            ('If true, use original Treemaps (B. Shneiderman) otherwise use'
             'Squarified Treemaps (J. J. van Wijk)'), 'false')

        self.addColorParameter(
            'border color',
            'The border color that will be applied to all treemap nodes',
            '(255, 255, 255, 255)')

        self.addLayoutPropertyParameter(
            'layout', 'The output treemap layout', 'viewLayout',
            True, False, True)

        self.addSizePropertyParameter(
            'sizes', 'The output treemap sizes', 'viewSize', True, False, True)

        self.addIntegerPropertyParameter(
            'shapes', 'The output treemap shapes', 'viewShape',
            True, False, True)

        self.addColorPropertyParameter(
            'colors', 'The output treemap colors', 'viewColor',
            True, False, True)

        self.addColorPropertyParameter(
            'border colors', 'The output treemap border colors',
            'viewBorderColor', True, False, True)

        self.addDoublePropertyParameter(
            'border widths', 'The output treemap border widths',
            'viewBorderWidth', True, False, True)

    def check(self):
        # Input graph must be a tree for the treemap layout
        if not tlp.TreeTest.isTree(self.graph):
            return (False, 'The graph must be a tree.')

        return (True, '')

    def run(self):
        # retrieve parameter values
        metric = self.dataSet['metric']
        aspectRatio = self.dataSet['aspect ratio']
        treeMapType = self.dataSet['treemap type']
        borderColor = self.dataSet['border color']
        layout = self.dataSet['layout']
        sizes = self.dataSet['sizes']
        shapes = self.dataSet['shapes']
        colors = self.dataSet['colors']
        borderColors = self.dataSet['border colors']
        borderWidths = self.dataSet['border widths']

        # call the 'Squarified Tree Map' layout algorithm
        params = tlp.getDefaultPluginParameters('Squarified Tree Map',
                                                self.graph)
        params['Node Size'] = sizes
        params['Node Shape'] = shapes
        params['metric'] = metric
        params['Aspect Ratio'] = aspectRatio
        params['Treemap Type'] = treeMapType

        self.graph.applyLayoutAlgorithm('Squarified Tree Map', layout, params)

        # set edge colors and border colors to be fully transparent
        colors.setAllEdgeValue(tlp.Color(0, 0, 0, 0))
        borderColors.setAllEdgeValue(tlp.Color(0, 0, 0, 0))

        # set new border color for nodes
        borderColors.setAllNodeValue(borderColor)
        # ensure node borders are visible
        borderWidths.setAllNodeValue(1.0)

        # set the square shape to all leaf nodes
        for n in self.graph.getNodes():
            if self.graph.outdeg(n) == 0:
                shapes[n] = tlp.NodeShape.Square

        return True


pluginDoc = """
Enables to easily configure a treemap layout visualisation for a tree.
As the treemap layout is different from classical node link diagram
representation, some visual properties setup has to be done in order
to get an aesthetic visualization of it in Tulip.
This plugin takes care of calling the 'Squarified Tree Map' layout algorithm
and adjust some visual properties to get a correct rendering of the treemap.
"""

# The line below does the magic to register the plugin into the plugin database
# and updates the GUI to make it accessible through the menus.
tulipplugins.registerPlugin(
    'SquarifiedTreeMapHelper', 'Squarified Tree Map Helper',
    'Antoine Lambert', '27/04/2017', pluginDoc, '1.0')
