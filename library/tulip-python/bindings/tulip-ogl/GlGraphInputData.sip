/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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


namespace tlp {

class GlGraphInputData /Abstract/ {
%TypeHeaderCode
#include <tulip/GlGraphInputData.h>
%End

%Docstring
.. versionadded:: 4.4

That class defines all the visual properties used to render a graph (layout, colors, sizes, ...).
By default, Tulip creates a pool of default properties whose name is prefixed by "view".
Through the use of that class, you can set other properties as rendering input data.

Use :meth:`tulipgui.tlpgui.NodeLinkDiagramComponent.getInputData` to get an instance on a such object.
%End

     public:

    GlGraphInputData(tlp::Graph* graph, tlp::GlGraphRenderingParameters* parameters);

    ~GlGraphInputData();

// =============================================================================================

    tlp::Graph* getGraph();
%Docstring
tlpogl.GlGraphInputData.getGraph()

Returns the graph associated to these data.

:rtype: :class:`tulip.tlp.Graph`
%End

// =============================================================================================

    tlp::ColorProperty * getElementColor() const;
%Docstring
tlpogl.GlGraphInputData.getElementColor()

Returns the color property that defines the current colors of graph elements (see :ref:`graph-colors`).

:rtype: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementColor(tlp::ColorProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementColor(property)

Sets the color property that defines the current colors of graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    tlp::ColorProperty *getElementLabelColor() const;
%Docstring
tlpogl.GlGraphInputData.getElementLabelColor()

Returns the color property that defines the current labels colors of graph elements (see :ref:`graph-colors`).

:rtype: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementLabelColor(tlp::ColorProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementLabelColor(property)

Sets the color property that defines the current labels colors of graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================


    tlp::ColorProperty *getElementLabelBorderColor() const;
%Docstring
tlpogl.GlGraphInputData.getElementLabelBorderColor()

Returns the color property that defines the current labels border colors of graph elements (see :ref:`graph-colors`).

:rtype: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementLabelBorderColor(tlp::ColorProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementBorderColor(property)

Sets the color property that defines the current labels borders colors of graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================


    tlp::DoubleProperty *getElementLabelBorderWidth() const;
%Docstring
tlpogl.GlGraphInputData.getElementLabelBorderWidth()

Returns the double property that defines the current labels borders widths of graph elements (see :ref:`borders-widths`).

:rtype: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementLabelBorderWidth(tlp::DoubleProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementLabelBorderWidth(property)

Sets the double property that defines the current labels borders widths of graph elements (see :ref:`borders-widths`).

:param property: a double property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementSize() const;
%Docstring
tlpogl.GlGraphInputData.getElementSize()

Returns the size property that defines the current sizes of graph elements (see :ref:`graph-sizes`).

:rtype: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementSize(tlp::SizeProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementSize(property)

Sets the size property that defines the current sizes of graph elements (see :ref:`graph-sizes`).

:param property: a size property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementLabelPosition() const;
%Docstring
tlpogl.GlGraphInputData.getElementLabelPosition()

Returns the integer property that defines the current labels positions of graph elements (see :ref:`graph-labels`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementLabelPosition(tlp::IntegerProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementLabelPosition(property)

Sets the integer property that defines the current labels positions of graph elements (see :ref:`graph-labels`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementShape() const;
%Docstring
tlpogl.GlGraphInputData.getElementShape()

Returns the integer property that defines the current shapes of graph elements (see :ref:`graph-shapes`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementShape(tlp::IntegerProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementShape(property)

Sets the integer property that defines the current shapes of graph elements (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::DoubleProperty *getElementRotation() const;
%Docstring
tlpogl.GlGraphInputData.getElementRotation()

Returns the double property that defines the current rotations of graph nodes (see :ref:`nodes-rotations`).

:rtype: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementRotation(tlp::DoubleProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementRotation(property)

Sets the double property that defines the current rotations graph nodes (see :ref:`nodes-rotations`).

:param property: a double property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::BooleanProperty *getElementSelected() const;
%Docstring
tlpogl.GlGraphInputData.getElementSelected()

Returns the boolean property that defines the current selection of graph elements (see :ref:`graph-selection`).

:rtype: :class:`tulip.tlp.BooleanProperty`
%End

// =============================================================================================

    void setElementSelected(tlp::BooleanProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementSelected(property)

Sets the boolean property that defines the current selection of graph elements (see :ref:`graph-selection`).

:param property: a boolean property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.BooleanProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementFont() const;
%Docstring
tlpogl.GlGraphInputData.getElementFont()

Returns the string property that defines the current font files used to render the labels of graph elements (see :ref:`graph-labels`).

:rtype: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    void setElementFont(tlp::StringProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementFont(property)

Sets the string property that defines the current font files used to render the labels of graph elements (see :ref:`graph-labels`).

:param property: a string property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    tlp::IntegerProperty  *getElementFontSize() const;
%Docstring
tlpogl.GlGraphInputData.getElementFontSize()

Returns the integer property that defines the current font size for labels of graph elements (see :ref:`graph-labels`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementFontSize(tlp::IntegerProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementFontSize(property)

Sets the integer property that defines the current font size for labels of graph elements (see :ref:`graph-labels`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementLabel() const;
%Docstring
tlpogl.GlGraphInputData.getElementLabel()

Returns the string property that defines the current labels of graph elements (see :ref:`graph-labels`).

:rtype: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    void setElementLabel(tlp::StringProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementLabel(property)

Sets the string property that defines the current labels of graph elements (see :ref:`graph-labels`).

:param property: a string property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    tlp::LayoutProperty *getElementLayout() const;
%Docstring
tlpogl.GlGraphInputData.getElementLayout()

Returns the layout property that defines the current layout of graph elements (see :ref:`graph-layout`).

:rtype: :class:`tulip.tlp.LayoutProperty`
%End

// =============================================================================================

    void setElementLayout(tlp::LayoutProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementLayout(property)

Sets the layout property that defines the current layout of graph elements (see :ref:`graph-layout`).

:param property: a layout property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.LayoutProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementTexture() const;
%Docstring
tlpogl.GlGraphInputData.getElementTexture()

Returns the string property that defines the current image files used to texture the shapes of graph elements (see :ref:`graph-textures`).

:rtype: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    void setElementTexture(tlp::StringProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementTexture(property)

Sets the string property that defines the current image files used to texture the shapes of graph elements (see :ref:`graph-textures`).

:param property: a string property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    tlp::ColorProperty *getElementBorderColor() const;
%Docstring
tlpogl.GlGraphInputData.getElementBorderColor()

Returns the color property that defines the current borders colors of graph elements (see :ref:`graph-colors`).

:rtype: :class:`tulip.tlp.LayoutProperty`
%End

// =============================================================================================

    void setElementBorderColor(tlp::ColorProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementBorderColor(property)

Sets the color property that defines the current borders colors of graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    tlp::DoubleProperty *getElementBorderWidth() const;
%Docstring
tlpogl.GlGraphInputData.getElementBorderWidth()

Returns the double property that defines the current borders widths of graph elements (see :ref:`borders-widths`).

:rtype: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementBorderWidth(tlp::DoubleProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementBorderWidth(property)

Sets the double property that defines the current borders widths of graph elements (see :ref:`borders-widths`).

:param property: a double property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementSrcAnchorShape() const;
%Docstring
tlpogl.GlGraphInputData.getElementSrcAnchorShape()

Returns the integer property that defines the current shapes of the source edges extremities (see :ref:`graph-shapes`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementSrcAnchorShape(tlp::IntegerProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementSrcAnchorShape(property)

Sets the integer property that defines the current shapes of the source edges extremities (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementSrcAnchorSize() const;
%Docstring
tlpogl.GlGraphInputData.getElementSrcAnchorSize()

Returns the size property that defines the current sizes of the source edges extremities (see :ref:`graph-sizes`).

:rtype: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementSrcAnchorSize(tlp::SizeProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementSrcAnchorSize(property)

Sets the size property that defines the current sizes of the source edges extremities (see :ref:`graph-shapes`).

:param property: a size property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementTgtAnchorShape() const;
%Docstring
tlpogl.GlGraphInputData.getElementTgtAnchorShape()

Returns the integer property that defines the current shapes of the target edges extremities (see :ref:`graph-shapes`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementTgtAnchorShape(tlp::IntegerProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementTgtAnchorShape(property)

Sets the integer property that defines the current shapes of the target edges extremities (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementTgtAnchorSize() const;
%Docstring
tlpogl.GlGraphInputData.getElementTgtAnchorSize()

Returns the size property that defines the current sizes of the target edges extremities (see :ref:`graph-sizes`).

:rtype: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementTgtAnchorSize(tlp::SizeProperty *property);
%Docstring
tlpogl.GlGraphInputData.setElementTgtAnchorSize(property)

Sets the size property that defines the current sizes of the target edges extremities (see :ref:`graph-shapes`).

:param property: a size property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    void reloadGraphProperties();
%Docstring
tlpogl.GlGraphInputData.reloadGraphProperties()

Restores all default graph visual properties (whose name is prefixed by "view") as rendering input data.
%End

// =============================================================================================

};
};
