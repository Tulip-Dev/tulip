/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

namespace tlp {

class GlGraphRenderingParameters {
%TypeHeaderCode
#include <tulip/GlGraphRenderingParameters.h>
%End

%Docstring
.. versionadded:: 4.4

That class holds all the rendering parameters for a Tulip OpenGL graph drawing.
Use :meth:`tulipgui.tlpgui.NodeLinkDiagramComponent.getRenderingParameters` to get an instance on a such object.
%End

public:

  GlGraphRenderingParameters();


// =============================================================================================

  void setAntialiasing(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setAntiAliasing(antialiasing)

enable/disable antialiasing.

:param antialiasing: the antialiasing state
:type antialiasing: boolean
%End

// =============================================================================================

  bool isAntialiased() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isAntialiased()

Returns if antialiasing rendering is activated.

:rtype: boolean
%End

// =============================================================================================

  void setViewArrow(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setViewArrow(arrow)

enable/disable the display of arrows along the graph edges.

:param arrow: the state of the arrows rendering
:type arrow: boolean
%End


// =============================================================================================

  bool isViewArrow()const;
%Docstring
tlpgui.GlGraphRenderingParameters.isViewArrow()

Returns the state of the display of arrows along the graph edges.

:rtype: boolean
%End

// =============================================================================================

  void setViewNodeLabel(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setViewNodeLabel(nodeLabel)

enable/disable the display of nodes labels.

:param nodeLabel: the state of the nodes labels rendering
:type nodeLabel: boolean
%End


// =============================================================================================

  bool isViewNodeLabel() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isViewNodeLabel()

Returns the state of the display of nodes labels.

:rtype: boolean
%End

// =============================================================================================

  void setViewEdgeLabel(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setViewEdgeLabel(edgeLabel)

enable/disable the display of edges labels.

:param edgeLabel: the state of the edges labels rendering
:type edgeLabel: boolean
%End

// =============================================================================================

  bool isViewEdgeLabel() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isViewEdgeLabel()

Returns the state of the display of edges labels.

:rtype: boolean
%End

// =============================================================================================

  void setViewOutScreenLabel(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setViewOutScreenLabel(outScreenLabel)

enable/disable the display of labels for elements outside the screen.

:param outScreenLabel: the state of the labels rendering for elements outside the screen.
:type outScreenLabel: boolean
%End

// =============================================================================================

  bool isViewOutScreenLabel() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isViewOutScreenLabel()

Returns the state of the display of labels for elements outside the screen.

:rtype: boolean
%End

// =============================================================================================

  void setEdgeColorInterpolate(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setEdgeColorInterpolate(interpolate)

enable/disable the interpolation of nodes colors along edges.

:param interpolate: the state of the edge color interpolation
:type interpolate: boolean
%End

// =============================================================================================

  bool isEdgeColorInterpolate() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isEdgeColorInterpolate()

Returns the state of the edge color interpolation.

:rtype: boolean
%End

// =============================================================================================

  void setEdgeSizeInterpolate(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setEdgeSizeInterpolate(interpolate)

enable/disable the interpolation of nodes sizes along edges.

:param interpolate: the state of the edge size interpolation
:type interpolate: boolean
%End

// =============================================================================================

  bool isEdgeSizeInterpolate() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isEdgeSizeInterpolate()

Returns the state of the edge size interpolation.

:rtype: boolean
%End

// =============================================================================================

  void setTexturePath(const std::string& path);
%Docstring
tlpgui.GlGraphRenderingParameters.setTexturePath(path)

Sets the absolute path for textures loading.

:param path: the absolute path of a directory for textures loading.
:type path: string
%End

// =============================================================================================

  std::string getTexturePath() const;
%Docstring
tlpgui.GlGraphRenderingParameters.getTexturePath()

Returns the absolute path used for texture loading.

:rtype: string
%End

// =============================================================================================

  void setFontsPath(const std::string& path);
%Docstring
tlpgui.GlGraphRenderingParameters.setFontsPath(path)

Sets the absolute path for fonts loading (.ttf files).

:param path: the absolute path of a directory for fonts loading.
:type path: string
%End

// =============================================================================================

  std::string getFontsPath();
%Docstring
tlpgui.GlGraphRenderingParameters.getFontsPath()

Returns the absolute path used for fonts loading (.ttf files).

:rtype: string
%End

// =============================================================================================

  void setViewMetaLabel(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setViewMetaLabel(metaLabel)

enable/disable the display of labels inside meta-nodes.

:param metaLabel: the state of the display of labels inside meta-nodes.
:type edgeLabel: boolean
%End

// =============================================================================================

  bool isViewMetaLabel() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isViewMetaLabel()

Returns the state of the display of labels inside meta-nodes.

:rtype: boolean
%End

// =============================================================================================

  void setDisplayEdges(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setDisplayEdges(displayEdges)

enable/disable the display of edges.

:param displayEdges: the state of the display of edges.
:type displayEdges: boolean
%End

// =============================================================================================

  bool isDisplayEdges() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isDisplayEdges()

Returns the state of the display of edges.

:rtype: boolean
%End

// =============================================================================================

  void setDisplayNodes(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setDisplayNodes(displayNodes)

enable/disable the display of nodes.

:param displayNodes: the state of the display of nodes.
:type displayNodes: boolean
%End

// =============================================================================================

  bool isDisplayNodes() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isDisplayNodes()

Returns the state of the display of nodes.

:rtype: boolean
%End

// =============================================================================================

  void setDisplayMetaNodes(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setDisplayMetaNodes(displayMetaNodes)

enable/disable the display of nodes.

:param displayMetaNodes: the state of the display of meta-nodes.
:type displayMetaNodes: boolean
%End

// =============================================================================================

  bool isDisplayMetaNodes() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isDisplayMetaNodes()

Returns the state of the display of meta-nodes.

:rtype: boolean
%End

// =============================================================================================

  void setSelectedNodesStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setSelectedNodesStencil(stencil)

Sets the stencil value for selected nodes. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 2.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End

// =============================================================================================

  int getSelectedNodesStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getSelectedNodesStencil()

Returns the stencil value for selected nodes.

:rtype: integer 
%End

// =============================================================================================

  void setSelectedMetaNodesStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setSelectedMetaNodesStencil(stencil)

Sets the stencil value for selected meta-nodes. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 2.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End


// =============================================================================================

  int getSelectedMetaNodesStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getSelectedMetaNodesStencil()

Returns the stencil value for selected meta-nodes.

:rtype: integer 
%End

// =============================================================================================

  void setSelectedEdgesStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setSelectedEdgesStencil(stencil)

Sets the stencil value for selected edges. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 2.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End

// =============================================================================================

  int getSelectedEdgesStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getSelectedEdgesStencil()

Returns the stencil value for selected edges.

:rtype: integer 
%End

// =============================================================================================

  void setNodesStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setNodesStencil(stencil)

Sets the stencil value for nodes. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 255.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End

// =============================================================================================

  int getNodesStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getNodesStencil()

Returns the stencil value for nodes.

:rtype: integer 
%End

// =============================================================================================

  void setMetaNodesStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setMetaNodesStencil(stencil)

Sets the stencil value for meta-nodes. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 255.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End

// =============================================================================================

  int getMetaNodesStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getMetaNodesStencil()

Returns the stencil value for meta-nodes.

:rtype: integer 
%End

// =============================================================================================

  void setEdgesStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setEdgesStencil(stencil)

Sets the stencil value for edges. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 255.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End

// =============================================================================================

  int getEdgesStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getEdgesStencil()

Returns the stencil value for edges.

:rtype: integer 
%End

// =============================================================================================

  void setNodesLabelStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setNodesLabelStencil(stencil)

Sets the stencil value for nodes labels. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 255.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End

// =============================================================================================

  int getNodesLabelStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getNodesLabelStencil()

Returns the stencil value for nodes labels.

:rtype: integer 
%End

// =============================================================================================

  void setMetaNodesLabelStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setMetaNodesLabelsStencil(stencil)

Sets the stencil value for meta-nodes labels. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 255.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End

// =============================================================================================

  int getMetaNodesLabelStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getMetaNodesLabelStencil()

Returns the stencil value for meta-nodes labels.

:rtype: integer 
%End

// =============================================================================================

  void setEdgesLabelStencil(const int stencil);
%Docstring
tlpgui.GlGraphRenderingParameters.setEdgesLabelStencil(stencil)

Sets the stencil value for edges labels. The lower the value is, the more the objects will be drawn
on top of other ones. The default value is 255.

:param stencil: a value in the range [0, 255]
:type stencil: integer
%End


// =============================================================================================

  int getEdgesLabelStencil();
%Docstring
tlpgui.GlGraphRenderingParameters.getEdgesLabelStencil()

Returns the stencil value for edges labels.

:rtype: integer 
%End

// =============================================================================================

  void setElementOrdered(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setElementOrdered(elementOrdered)

enable/disable the ordered rendering of graph elements according
to the values in the element ordering property.

:param elementsOrdered: the state of the ordered rendering
:type elementOrdered: boolean
%End

// =============================================================================================

  bool isElementOrdered() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isElementOrdered()

Returns the state of the ordered rendering of graph elements.

:rtype: boolean
%End

// =============================================================================================

  void setElementOrderedDescending(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setElementOrderedDescending(elementOrderedDescending)

.. versionadded:: 4.8

Defines if the graph elements are sorted in descending order when the element ordering is activated.

:param elementsOrderedDescending: whether graph elements have to be sorted in descending order
:type elementOrdered: boolean
%End

// =============================================================================================

  bool isElementOrderedDescending() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isElementOrderedDescending()

.. versionadded:: 4.8

Returns if the graph elements are sorted in descending order when the element ordering is activated

:rtype: boolean
%End

// =============================================================================================

  void setElementOrderingProperty(tlp::NumericProperty* property /AllowNone/);
%Docstring
tlpgui.GlGraphRenderingParameters.setElementOrderingProperty(property)

Defines which property should be used for the ordered rendering of graph elements.
Setting it to :const:`None` will use the "viewMetric" property if the ordering is enabled.

:param property: the property to use to order the rendering of graph elements
:type property: :class:`tulip.tlp.NumericProperty`
%End

// =============================================================================================

  tlp::NumericProperty* getElementOrderingProperty() const;
%Docstring
tlpgui.GlGraphRenderingParameters.getElementOrderingProperty()

Returns the double property used for the ordered rendering of graph elements.

:rtype: :class:`tulip.tlp.NumericProperty`
%End

// =============================================================================================

  void setElementZOrdered(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setElementZOrdered(zOrdering)

enable/disable the ordered rendering of elements according to their
distance to the camera.

:param zOrdering: the state of the z-ordered rendering
:type zOrdering: boolean
%End

// =============================================================================================

  bool isElementZOrdered() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isElementZOrdered()

Returns the state of the z-ordered rendering.

:rtype: boolean
%End

// =============================================================================================

  void setEdge3D(const bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setEdge3D(edge3d)

enable/disable the 3D display of edges instead of 2D lines.

:param edge3d: the state of the 3D edge rendering
:type edge3d: boolean
%End

// =============================================================================================

  bool isEdge3D() const;
%Docstring
tlpgui.GlGraphRenderingParameters.isEdge3D()

Returns the state of the 3D edge rendering.

:rtype: boolean
%End

// =============================================================================================

  void setSelectionColor(const tlp::Color &color);
%Docstring
tlpgui.GlGraphRenderingParameters.setSelectionColor(color)

Sets the color of selected elements.

:param color: the selection color
:type color: :class:`tulip.tlp.Color`
%End

// =============================================================================================

  tlp::Color getSelectionColor();
%Docstring
tlpgui.GlGraphRenderingParameters.getSelectionColor()

Returns the color of selection.

:rtype: :class:`tulip.tlp.Color`
%End

// =============================================================================================

  bool isLabelScaled();
%Docstring
tlpgui.GlGraphRenderingParameters.isLabelScaled()

Returns the state of the scaling of labels to nodes sizes.

:rtype: boolean
%End

// =============================================================================================

  void setLabelScaled(bool state);
%Docstring
tlpgui.GlGraphRenderingParameters.setLabelScaled(labelScaled)

enable/disable the scaling of labels to nodes sizes.

:param labelScaled: the state of the scaling of labels to nodes sizes.
:type labelScaled: boolean
%End

// =============================================================================================

  bool isLabelFixedFontSize() const;
%Docstring
tlpgui.isLabelFixedFontSize()

Returns the state of using fixed font size with labels.

:rtype: boolean
%End

// =============================================================================================

  void setLabelFixedFontSize(bool state);
%Docstring
tlpgui.setLabelFixedFontSize(fixedFontSize)

enable/disable the use of fixed font size with labels (based on the values in the "viewFontSize" property).

:param fixedFontSize: the state of using fixed font size with labels
:type fixedFontSize: boolean
%End

// =============================================================================================

  int getLabelsDensity();
%Docstring
tlpgui.GlGraphRenderingParameters.getLabelsDensity()

Returns the labels density value.

:rtype: integer
%End

// =============================================================================================

  void setLabelsDensity(int density);
%Docstring
tlpgui.GlGraphRenderingParameters.setLabelsDensity(density)

Sets the labels density value. It must be in the range [-100, 100].
The labels are displayed according to that value :

  * -100 : no labels are displayed
  * 0 : only non overlapping labels are displayed
  * 100 : all labels are displayed

:param density: a value in the range [-100, 100]
:type density: integer
%End

// =============================================================================================

    int getMinSizeOfLabel();
%Docstring
tlpgui.GlGraphRenderingParameters.getMinSizeOfLabel()

Returns the minimum size of labels. That value is only used if the labels
are not scaled to nodes sizes.

:rtype: integer
%End

// =============================================================================================

  void setMinSizeOfLabel(int size);
%Docstring
tlpgui.GlGraphRenderingParameters.setMinSizeOfLabel(size)

Sets the minimum size of labels. That value is only used if the labels
are not scaled to nodes sizes.

:param size: the minimum size of labels
:type: integer
%End

// =============================================================================================

  int getMaxSizeOfLabel();
%Docstring
tlpgui.GlGraphRenderingParameters.getMaxSizeOfLabel()

Returns the maximum size of labels. That value is only used if the labels
are not scaled to nodes sizes.

:rtype: integer
%End

// =============================================================================================

  void setMaxSizeOfLabel(int size);
%Docstring
tlpgui.GlGraphRenderingParameters.setMaxSizeOfLabel(size)

Sets the maximum size of labels. That value is only used if the labels
are not scaled to nodes sizes.

:param size: the maximum size of labels
:type: integer
%End

// =============================================================================================

  bool getLabelsAreBillboarded();
%Docstring
tlpgui.GlGraphRenderingParameters.getLabelsAreBillboarded()

Returns the state of the labels billboarding (always facing the camera).

:rtype: boolean
%End

// =============================================================================================

  void setLabelsAreBillboarded(bool billboarded);
%Docstring
tlpgui.GlGraphRenderingParameters.setLabelsAreBillboarded(billboardLabels)

enable/disable labels billboarding (always facing the camera).

:param billboardLabels: the state of labels billboarding
:type billboardLabels: boolean
%End

// =============================================================================================

  void setDisplayFilteringProperty(tlp::BooleanProperty *filteringProperty /AllowNone/);
%Docstring
tlpgui.GlGraphRenderingParameters.setDisplayFilteringProperty(property)

Sets the boolean property used to filter the rendering of graph elements. If sets to :const:`None`,
the filtering is deactivated. If the value for a node/edge is :const:`False`, then
it will be displayed.

:param property: the boolean property used to filter the rendering of graph elements
:type property: :class:`tulip.tlp.BooleanProperty`
%End

// =============================================================================================

  tlp::BooleanProperty * getDisplayFilteringProperty() const;
%Docstring
tlpgui.GlGraphRenderingParameters.getDisplayFilteringProperty()

Returns the boolean property used to filter the rendering of graph elements.

:rtype: :class:`tulip.tlp.BooleanProperty`
%End

};
};

