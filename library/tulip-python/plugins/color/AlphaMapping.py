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

# Tulip Python plugin for mapping metric values to alpha component of graph element colors.
# In other words, it enables to compute the graph elements transparency according to the values
# stored in a numeric property of a graph.

from tulip import tlp
import tulipplugins
import math

def clamp(n, smallest, largest): 
  return max(smallest, min(n, largest))

def getFactor(v, r):
  if r == 0:
    r = 1
  if v < 0:
    v = 0
  if v > r:
    v = r
  return v / r

mappingTypeDoc = '''
That parameter defines the type of alpha mapping to perform. For the linear case, the minimum value of 
the input numeric property is mapped to a minimum alpha value picked by the user, the maximum value is 
mapped to a maximum alpha value picked by the user, and a linear interpolation is used between both to 
compute the associated alpha of the graph element color.<br/> 
For the logarithmic case, input numeric properties values are first mapped in the [1, +inf[ range. Then 
the log of each mapped value is computed and used to compute the associated alpha value of the graph 
element color trough a linear interpolation between 0 and the log of the mapped maximum value of graph elements.<br/>
If uniform, this is the same except for the interpolation: the values are sorted, numbered, and a linear 
interpolation is used on those numbers (in other words, only the order is taken into account, not the actual values).
'''

class AlphaMapping(tlp.ColorAlgorithm):
  def __init__(self, context):
    tlp.ColorAlgorithm.__init__(self, context)
    self.addNumericPropertyParameter('input property', 'The input numeric property from which to compute alpha mapping', 'viewMetric')
    self.addStringCollectionParameter('target', 'Whether alpha values are computed for nodes or edges', 'nodes;edges')
    self.addStringCollectionParameter('type', mappingTypeDoc, 'linear;logarithmic;uniform')
    self.addUnsignedIntegerParameter('min alpha', 'The minimum alpha value (between 0 and 255) to map on graph elements colors', '0')
    self.addUnsignedIntegerParameter('max alpha', 'The maximum alpha value (between 0 and 255) to map on graph elements colors', '255')

  def run(self):
    
    vColor = self.graph.getColorProperty("viewColor")
    inputMetric = self.dataSet['input property']
    minAlpha = clamp(self.dataSet['min alpha'], 0, 255)
    maxAlpha = clamp(self.dataSet['max alpha'], 0, 255)
    target = self.dataSet['target']
    mappingType = self.dataSet['type']
    
    if minAlpha > maxAlpha:
      minAlpha, maxAlpha = maxAlpha, minAlpha
    
    if mappingType == 'uniform':
      inputMetric = inputMetric.copyProperty(self.graph)
      inputMetric.uniformQuantification(300)
    
    elif mappingType == 'logarithmic':
      inputMetric = inputMetric.copyProperty(self.graph)
      minN = inputMetric.getNodeDoubleMin()
      minE = inputMetric.getEdgeDoubleMin()
      for n in self.graph.getNodes():
        inputMetric[n] = math.log(1+inputMetric[n]-minN)
      for e in self.graph.getEdges():
        inputMetric[e] = math.log(1+inputMetric[e]-minE)
    
    if target == 'nodes':
      minValue = inputMetric.getNodeDoubleMin(self.graph)
      maxValue = inputMetric.getNodeDoubleMax(self.graph)
      
      for n in self.graph.nodes():
        val = inputMetric[n]
        t = getFactor(val - minValue, maxValue - minValue)
        alpha = minAlpha + t * (maxAlpha - minAlpha)
        color = vColor[n]
        color[3] = int(alpha)
        self.result[n] = color
    
    else:
      minValue = inputMetric.getEdgeDoubleMin(self.graph)
      maxValue = inputMetric.getEdgeDoubleMax(self.graph)
      
      for e in self.graph.edges():
        val = inputMetric[e]
        t = getFactor(val - minValue, maxValue - minValue)
        alpha = minAlpha + t * (maxAlpha - minAlpha)
        color = vColor[e]
        color[3] = int(alpha)
        self.result[e] = color  
    
    return True

pluginDoc = '''
Map metric values to alpha component of graph element colors.
In other words, it enables to compute the graph elements transparency according to the values
stored in a numeric property of a graph.
'''

# The line below does the magic to register the plugin into the plugin database
# and updates the GUI to make it accessible through the menus.
tulipplugins.registerPlugin("AlphaMapping", "Alpha Mapping", "Antoine Lambert", "20/04/2017", pluginDoc, "1.1")
