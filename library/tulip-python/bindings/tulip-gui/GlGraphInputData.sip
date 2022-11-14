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

class GlGraphInputData /Abstract/ {
%TypeHeaderCode
#include <tulip/GlGraphInputData.h>
%End

%Docstring
.. versionadded:: 4.4

This class defines all the visual properties used to render a graph (layout, colors, sizes, ...).
By default, Tulip creates a pool of default properties whose name is prefixed by "view".
Through the use of this class, you can set other properties as rendering input data.

Use :meth:`tulipgui.tlpgui.NodeLinkDiagramComponent.getInputData` to get an instance of such an object.
%End

     public:

    GlGraphInputData(tlp::Graph* graph, tlp::GlGraphRenderingParameters* parameters);

    ~GlGraphInputData();

// =============================================================================================

    tlp::Graph* getGraph();
%Docstring
tlpgui.GlGraphInputData.getGraph()

Returns the graph associated to this data.

:rtype: :class:`tulip.tlp.Graph`
%End

// =============================================================================================

    tlp::ColorProperty * getElementColor() const;
%Docstring
tlpgui.GlGraphInputData.getElementColor()

Returns the color property that defines the current color of the graph elements (see :ref:`graph-colors`).

:rtype: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementColor(tlp::ColorProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementColor(property)

Sets the color property that defines the current color of the graph elements (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    tlp::ColorProperty *getElementLabelColor() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabelColor()

Returns the color property that defines the current color of the graph element labels (see :ref:`graph-colors`).

:rtype: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementLabelColor(tlp::ColorProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLabelColor(property)

Sets the color property that defines the current colors of the graph element labels (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================


    tlp::ColorProperty *getElementLabelBorderColor() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabelBorderColor()

Returns the color property that defines the current colors of the graph element label borders (see :ref:`graph-colors`).

:rtype: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    void setElementLabelBorderColor(tlp::ColorProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementBorderColor(property)

Sets the color property that defines the current colors of the graph element label borders (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================


    tlp::DoubleProperty *getElementLabelBorderWidth() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabelBorderWidth()

Returns the double property that defines the current widths of the graph element label borders (see :ref:`borders-widths`).

:rtype: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementLabelBorderWidth(tlp::DoubleProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLabelBorderWidth(property)

Sets the double property that defines the current widths of the graph element label borders (see :ref:`borders-widths`).

:param property: a double property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementSize() const;
%Docstring
tlpgui.GlGraphInputData.getElementSize()

Returns the size property that defines the current sizes of the graph elements (see :ref:`graph-sizes`).

:rtype: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementSize(tlp::SizeProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementSize(property)

Sets the size property that defines the current sizes of the graph elements (see :ref:`graph-sizes`).

:param property: a size property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementLabelPosition() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabelPosition()

Returns the integer property that defines the current positions of the graph element labels (see :ref:`graph-labels`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementLabelPosition(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLabelPosition(property)

Sets the integer property that defines the current position of the graph element labels (see :ref:`graph-labels`).

:param property: an integer property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementShape() const;
%Docstring
tlpgui.GlGraphInputData.getElementShape()

Returns the integer property that defines the current shapes of the graph elements (see :ref:`graph-shapes`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementShape(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementShape(property)

Sets the integer property that defines the current shapes of the graph elements (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::DoubleProperty *getElementRotation() const;
%Docstring
tlpgui.GlGraphInputData.getElementRotation()

Returns the double property that defines the current rotations of the graph nodes (see :ref:`nodes-rotations`).

:rtype: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementRotation(tlp::DoubleProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementRotation(property)

Sets the double property that defines the current rotations of the graph nodes (see :ref:`nodes-rotations`).

:param property: a double property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::BooleanProperty *getElementSelected() const;
%Docstring
tlpgui.GlGraphInputData.getElementSelected()

Returns the Boolean property that defines the current selection of the graph elements (see :ref:`graph-selection`).

:rtype: :class:`tulip.tlp.BooleanProperty`
%End

// =============================================================================================

    void setElementSelected(tlp::BooleanProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementSelected(property)

Sets the Boolean property that defines the current selection of the graph elements (see :ref:`graph-selection`).

:param property: a Boolean property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.BooleanProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementFont() const;
%Docstring
tlpgui.GlGraphInputData.getElementFont()

Returns the string property that defines the font files used to render the graph element labels (see :ref:`graph-labels`).

:rtype: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    void setElementFont(tlp::StringProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementFont(property)

Sets the string property that defines the font files used to render the graph elements labels (see :ref:`graph-labels`).

:param property: a string property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    tlp::IntegerProperty  *getElementFontSize() const;
%Docstring
tlpgui.GlGraphInputData.getElementFontSize()

Returns the integer property that defines the font sizes for the graph element labels (see :ref:`graph-labels`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementFontSize(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementFontSize(property)

Sets the integer property that defines the font sizes for the graph element labels (see :ref:`graph-labels`).

:param property: an integer property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementLabel() const;
%Docstring
tlpgui.GlGraphInputData.getElementLabel()

Returns the string property that defines the graph element labels (see :ref:`graph-labels`).

:rtype: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    void setElementLabel(tlp::StringProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLabel(property)

Sets the string property that defines the graph element labels (see :ref:`graph-labels`).

:param property: a string property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    tlp::LayoutProperty *getElementLayout() const;
%Docstring
tlpgui.GlGraphInputData.getElementLayout()

Returns the layout property that defines the graph element coordinates (see :ref:`graph-layout`).

:rtype: :class:`tulip.tlp.LayoutProperty`
%End

// =============================================================================================

    void setElementLayout(tlp::LayoutProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementLayout(property)

Sets the layout property that defines the graph element coordinates (see :ref:`graph-layout`).

:param property: a layout property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.LayoutProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementTexture() const;
%Docstring
tlpgui.GlGraphInputData.getElementTexture()

Returns the string property that defines the image files used to texture the graph element shapes (see :ref:`graph-textures`).

:rtype: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    void setElementTexture(tlp::StringProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementTexture(property)

Sets the string property that defines the image files used to texture the graph elements shapes (see :ref:`graph-textures`).

:param property: a string property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================

    tlp::ColorProperty *getElementBorderColor() const;
%Docstring
tlpgui.GlGraphInputData.getElementBorderColor()

Returns the color property that defines the graph element border colors (see also :ref:`graph-colors`).

:rtype: :class:`tulip.tlp.LayoutProperty`
%End

// =============================================================================================

    void setElementBorderColor(tlp::ColorProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementBorderColor(property)

Sets the color property that defines the graph element border colors (see :ref:`graph-colors`).

:param property: a color property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.ColorProperty`
%End

// =============================================================================================

    tlp::DoubleProperty *getElementBorderWidth() const;
%Docstring
tlpgui.GlGraphInputData.getElementBorderWidth()

Returns the double property that defines the graph element border widths (see :ref:`borders-widths`).

:rtype: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    void setElementBorderWidth(tlp::DoubleProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementBorderWidth(property)

Sets the double property that defines the graph element border widths (see :ref:`borders-widths`).

:param property: a double property attached to the graph associated to these data.
:type property: :class:`tulip.tlp.DoubleProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementSrcAnchorShape() const;
%Docstring
tlpgui.GlGraphInputData.getElementSrcAnchorShape()

Returns the integer property that defines the shapes of the extremity of the source edges (see :ref:`graph-shapes`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementSrcAnchorShape(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementSrcAnchorShape(property)

Sets the integer property that defines the shapes of the extremity of the source edges (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementSrcAnchorSize() const;
%Docstring
tlpgui.GlGraphInputData.getElementSrcAnchorSize()

Returns the size property that defines the sizes of the extremity of the source edges (see :ref:`graph-sizes`).

:rtype: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementSrcAnchorSize(tlp::SizeProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementSrcAnchorSize(property)

Sets the size property that defines the sizes of the extremity of the source edges (see :ref:`graph-shapes`).

:param property: a size property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    tlp::IntegerProperty *getElementTgtAnchorShape() const;
%Docstring
tlpgui.GlGraphInputData.getElementTgtAnchorShape()

Returns the integer property that defines the shapes of the extremity of the target edges (see :ref:`graph-shapes`).

:rtype: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    void setElementTgtAnchorShape(tlp::IntegerProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementTgtAnchorShape(property)

Sets the integer property that defines the shapes of the extremity of the target edges (see :ref:`graph-shapes`).

:param property: an integer property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.IntegerProperty`
%End

// =============================================================================================

    tlp::SizeProperty *getElementTgtAnchorSize() const;
%Docstring
tlpgui.GlGraphInputData.getElementTgtAnchorSize()

Returns the size property that defines the sizes of the extremity of the target edges  (see :ref:`graph-sizes`).

:rtype: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    void setElementTgtAnchorSize(tlp::SizeProperty *property);
%Docstring
tlpgui.GlGraphInputData.setElementTgtAnchorSize(property)

Sets the size property that defines the sizes of the extremity of the target edges  (see :ref:`graph-shapes`).

:param property: a size property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.SizeProperty`
%End

// =============================================================================================

    tlp::StringProperty *getElementIcon() const;
%Docstring
tlpgui.GlGraphInputData.getElementIcon()

    .. versionadded:: 5.7

Returns the string property that defines the icon names for nodes (see :class:`tlp.TulipFontAwesome` and :class:`tlp.TulipMaterialDesignIcons`).

:rtype: :class:`tulip.tlp.StringProperty`
%End

// =============================================================================================
    void setElementIcon(tlp::StringProperty *property);

    %Docstring
        .. versionadded:: 5.7
    tlpgui.GlGraphInputData.setElementIcon(property)

Sets the string property that defines the icon names for nodes (see :class:`tlp.TulipFontAwesome` and :class:`tlp.TulipMaterialDesignIcons`).

:param property: a string property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.StringProperty`

%End

// =============================================================================================
    tlp::DoubleProperty *getEdgeLengthRatio() const;
%Docstring

tlpgui.GlGraphInputData.getEdgeLengthRatio()
       .. versionadded:: 5.7

Returns the double property that defines the length ratios of the edges to display.

:rtype: :class:`tulip.tlp.DoubleProperty`
%End
    // =============================================================================================
    void setEdgeLengthRatio(tlp::DoubleProperty *property);

%Docstring

tlpgui.GlGraphInputData.setEdgeLengthRatio(property )
.. versionadded:: 5.7

Sets the double property that defines the length ratios of the edges to display.

:param property: a double property attached to the graph associated to this data.
:type property: :class:`tulip.tlp.DoubleProperty`

%End

    // =============================================================================================

    void reloadGraphProperties();
%Docstring
tlpgui.GlGraphInputData.reloadGraphProperties()

Restores all default graph visual properties (whose name is prefixed by "view") as rendering input data.
%End

// =============================================================================================

};
};
