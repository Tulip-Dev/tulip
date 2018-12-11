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

tulipGuiOk = True

try:
  from tulipgui import tlpgui
except:
  tulipGuiOk = False

class H3LayoutHelper(tlp.Algorithm):
  def __init__(self, context):
    tlp.Algorithm.__init__(self, context)
    self.addFloatParameter("layout scaling", "the scale factor to apply to the computed layout", "1000")

  def check(self):
    if not tlp.ConnectedTest.isConnected(self.graph):
      return (False, "The graph must be connected")
      
    return (True, "")

  def run(self):
    
    scaling = 1000
    if self.dataSet:
      scaling = self.dataSet["layout scaling"]
    
    params = tlp.getDefaultPluginParameters('H3', self.graph)
    success, errmsg = self.graph.applyLayoutAlgorithm("H3", params)
    if not success:
      if self.pluginProgress:
        self.pluginProgress.setError(errmsg)
        return False
    
    
    self.graph['viewBorderWidth'].setAllNodeValue(0)
    self.graph['viewShape'].setAllNodeValue(tlp.NodeShape.Sphere)
    self.graph['viewTgtAnchorShape'].setAllEdgeValue(tlp.EdgeExtremityShape.Cone)
  
    for n in self.graph.getNodes():
      w = self.graph['viewSize'][n][0]
      h = self.graph['viewSize'][n][1]
      s = min(w, h)
      self.graph['viewSize'][n] = tlp.Size(s)

    if tulipGuiOk:
      for v in tlpgui.getViewsOfGraph(self.graph):
        if isinstance(v, tlpgui.NodeLinkDiagramComponent):
          rp = v.getRenderingParameters()
          rp.setEdge3D(True)
          rp.setViewArrow(True)
          rp.setLabelsAreBillboarded(True)
          v.setRenderingParameters(rp)

      return True

# The line below does the magic to register the plugin into the plugin database
# and updates the GUI to make it accessible through the menus.
tulipplugins.registerPluginOfGroup("H3LayoutHelper", "H3 Layout Helper", "Antoine Lambert", "01/09/2017",
"""
Enables to easily configure a H3 layout visualisation for a connected quasi-hierarchical graph.
As this is a 3d layout, some rendering setup has to be done after the algorithm execution
in order to get an aesthetic rendering of it in Tulip.
That plugin takes care of calling the H3 layout algorithm, setting node shapes as sphere,
setting edge extremity shapes to cone and set appropriate rendering parameters for 3d
layout visualization.
"""
, "1.0", "Misc")
