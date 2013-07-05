.. py:currentmodule:: tulip

Modifying the visual attributes of a graph
==========================================

A Tulip graph contains a lot of visual attributes that can be freely
modified from Python. These attributes are used by the Tulip software
when rendering the graph. Below is an exhaustive list of these attributes :

    * nodes and edges layout
    * nodes rotations
    * nodes and edges selection
    * nodes and edges colors
    * nodes and edges sizes
    * nodes and edges shapes
    * nodes and edges border colors
    * nodes and edges border widths
    * nodes and edges labels
    * nodes and edges labels colors
    * nodes and edges labels fonts
    * nodes and edges textures
    * edge extremities shapes
    * edge extremities sizes

.. _graph-layout:

Graph layout
------------

The layout of the graph can be modified through the use of the **"viewLayout"** graph property (of type :class:`tlp.LayoutProperty`).
The position of a node is defined by a 3d point (of type :class:`tlp.Coord`). Bends (or curve control points) can also be set
on the edges through a list of 3d points.

The sample code below shows how to set a random layout from Python::

    import random

    size = 100
    viewLayout = graph.getLayoutProperty("viewLayout")

    for n in graph.getNodes():
        x = random.random() * size
        y = random.random() * size
        viewLayout[n] = tlp.Coord(x, y, 0)

.. _nodes-rotations:

Rotations of the nodes
-----------------------

Rotations can be applied on graph nodes when rendering them. The rotation angles in degrees
must be stored in the **"viewRotation"** graph property (of type :class:`tlp.DoubleProperty`).

The sample code below rotates of 90 degrees the nodes whose degree is greater than 10::

    viewRotation = graph.getDoubleProperty("viewRotation")

    for n in graph.getNodes():
        if graph.deg(n) > 10:
            viewRotation[n] = 90

.. _graph-selection:

Selection
----------

Elements of the graph can be marked in a selected state. These elements will be highlighted in the Tulip viusalizations.
The current selection is stored in the **"viewSelection"** graph property (of type :class:`tlp.BooleanProperty`).

The sample code below select nodes whose degree is greater than 3 and the edges between the selected nodes::

    viewSelection = graph.getBooleanProperty("viewSelection")

    viewSelection.setAllNodeValue(False)
    viewSelection.setAllEdgeValue(False)

    for n in graph.getNodes():
        if graph.deg(n) > 3:
            viewSelection[n] = True

    for e in graph.getEdges():
        if viewSelection[graph.source(e)] and viewSelection[graph.target(e)]:
            viewSelection[e] = True

.. _graph-colors:

Colors of graph elements and labels
------------------------------------

The colors of nodes and edges can be modified through the **"viewColor"** and **"viewBorderColor"** graph properties (of type :class:`tlp.ColorProperty`).
The colors of nodes and edges labels can be modified through the **"viewLabelColor"** and **"viewLabelBorderColor"** graph property.
The colors must be given in RGBA format through an instance of the :class:`tlp.Color` class.

The sample code below colors nodes whose degree is greater than 3 in blue and the others in green::

    blue = tlp.Color(0,0,255)
    green = tlp.Color(0,255,0)

    viewColor = graph.getColorProperty("viewColor")

    for n in graph.getNodes():
        if graph.deg(n) > 3:
            viewColor[n] = blue
        else:
            viewColor[n] = green

Some predefined colors constants are also available in the :class:`tlp.Color`. Below is the exhaustive list of these constants:

    * :const:`tlp.Color.Amaranth`
    * :const:`tlp.Color.Amber`
    * :const:`tlp.Color.Apricot`
    * :const:`tlp.Color.Aquamarine`
    * :const:`tlp.Color.Azure`
    * :const:`tlp.Color.BabyBlue`
    * :const:`tlp.Color.Beige`
    * :const:`tlp.Color.Black`
    * :const:`tlp.Color.Blue`
    * :const:`tlp.Color.BlueGreen`
    * :const:`tlp.Color.BlueViolet`
    * :const:`tlp.Color.Blush`
    * :const:`tlp.Color.Bronze`
    * :const:`tlp.Color.Brown`
    * :const:`tlp.Color.Burgundy`
    * :const:`tlp.Color.Byzantium`
    * :const:`tlp.Color.Carmine`
    * :const:`tlp.Color.Cerise`
    * :const:`tlp.Color.Cerulean`
    * :const:`tlp.Color.Champagne`
    * :const:`tlp.Color.ChartreuseGreen`
    * :const:`tlp.Color.Chocolate`
    * :const:`tlp.Color.Coffee`
    * :const:`tlp.Color.Copper`
    * :const:`tlp.Color.Coral`
    * :const:`tlp.Color.Crimson`
    * :const:`tlp.Color.Cyan`
    * :const:`tlp.Color.DesertSand`
    * :const:`tlp.Color.ElectricBlue`
    * :const:`tlp.Color.Erin`
    * :const:`tlp.Color.Gold`
    * :const:`tlp.Color.Gray`
    * :const:`tlp.Color.Green`
    * :const:`tlp.Color.Harlequin`
    * :const:`tlp.Color.Indigo`
    * :const:`tlp.Color.Ivory`
    * :const:`tlp.Color.Jade`
    * :const:`tlp.Color.JungleGreen`
    * :const:`tlp.Color.Lavender`
    * :const:`tlp.Color.Lemon`
    * :const:`tlp.Color.Lilac`
    * :const:`tlp.Color.Lime`
    * :const:`tlp.Color.Magenta`
    * :const:`tlp.Color.MagentaRose`
    * :const:`tlp.Color.Maroon`
    * :const:`tlp.Color.Mauve`
    * :const:`tlp.Color.NavyBlue`
    * :const:`tlp.Color.Olive`
    * :const:`tlp.Color.Orange`
    * :const:`tlp.Color.OrangeRed`
    * :const:`tlp.Color.Orchid`
    * :const:`tlp.Color.Peach`
    * :const:`tlp.Color.Pear`
    * :const:`tlp.Color.Periwinkle`
    * :const:`tlp.Color.PersianBlue`
    * :const:`tlp.Color.Pink`
    * :const:`tlp.Color.Plum`
    * :const:`tlp.Color.PrussianBlue`
    * :const:`tlp.Color.Puce`
    * :const:`tlp.Color.Purple`
    * :const:`tlp.Color.Raspberry`
    * :const:`tlp.Color.Red`
    * :const:`tlp.Color.RedViolet`
    * :const:`tlp.Color.Rose`
    * :const:`tlp.Color.Salmon`
    * :const:`tlp.Color.Sapphire`
    * :const:`tlp.Color.Scarlet`
    * :const:`tlp.Color.Silver`
    * :const:`tlp.Color.SlateGray`
    * :const:`tlp.Color.SpringBud`
    * :const:`tlp.Color.SpringGreen`
    * :const:`tlp.Color.Tan`
    * :const:`tlp.Color.Taupe`
    * :const:`tlp.Color.Teal`
    * :const:`tlp.Color.Turquoise`
    * :const:`tlp.Color.Violet`
    * :const:`tlp.Color.Viridian`
    * :const:`tlp.Color.White`
    * :const:`tlp.Color.Yellow`

.. _graph-sizes:

Sizes of graph elements
------------------------

The display sizes of the graph elements can be modified through the use of the **"viewSize"** graph property (of type :class:`tlp.SizeProperty`).
The display sizes of the edge extremities can also be modified through the **"viewSrcAnchorSize"** and **"viewTgtAnchorSize"** graph properties.
The width, height and depth of an element must be given through an instance of the :class:`tlp.Size` class.

The sample code below set nodes sizes proportional to their degree::

    viewSize = graph.getSizeProperty("viewSize")

    baseSize = tlp.Size(1,1,1)

    for n in graph.getNodes():
        viewSize[n] = baseSize * (graph.deg(n) + 1)

.. _graph-shapes:

Shapes of graph elements
-------------------------

The shape of nodes and edges can be modified through the use of the **"viewShape"** graph property (of type :class:`tlp.IntegerProperty`).
Some predefined constants are available for code readability (shapes are defined by integers in Tulip).

Below is the exhaustive list of the constants for nodes shapes:

    * :const:`tlp.NodeShape.Billboard`
    * :const:`tlp.NodeShape.ChristmasTree`
    * :const:`tlp.NodeShape.Circle`
    * :const:`tlp.NodeShape.Cone`
    * :const:`tlp.NodeShape.Cross`
    * :const:`tlp.NodeShape.Cube`
    * :const:`tlp.NodeShape.CubeOutlined`
    * :const:`tlp.NodeShape.CubeOutlinedTransparent`
    * :const:`tlp.NodeShape.Cylinder`
    * :const:`tlp.NodeShape.Diamond`
    * :const:`tlp.NodeShape.GlowSphere`
    * :const:`tlp.NodeShape.HalfCylinder`
    * :const:`tlp.NodeShape.Hexagon`
    * :const:`tlp.NodeShape.Pentagon`
    * :const:`tlp.NodeShape.Ring`
    * :const:`tlp.NodeShape.RoundedBox`
    * :const:`tlp.NodeShape.Sphere`
    * :const:`tlp.NodeShape.Square`
    * :const:`tlp.NodeShape.Star`
    * :const:`tlp.NodeShape.Triangle`
    * :const:`tlp.NodeShape.Window`

Below is the exhaustive list of the constants for edges shapes:

    * :const:`tlp.EdgeShape.Polyline`
    * :const:`tlp.EdgeShape.BezierCurve`
    * :const:`tlp.EdgeShape.CatmullRomCurve`
    * :const:`tlp.EdgeShape.CubicBSplineCurve`

The shape of the edge extremities can also be modified through the **"viewSrcAnchorShape"** and **"viewTgtAnchorShape"** graph properties.
Constants are also available for code readability, here is the exhaustive list :

    * :const:`tlp.EdgeExtremityShape.None`
    * :const:`tlp.EdgeExtremityShape.Arrow`
    * :const:`tlp.EdgeExtremityShape.Circle`
    * :const:`tlp.EdgeExtremityShape.Cone`
    * :const:`tlp.EdgeExtremityShape.Cross`
    * :const:`tlp.EdgeExtremityShape.Cube`
    * :const:`tlp.EdgeExtremityShape.CubeOutlinedTransparent`
    * :const:`tlp.EdgeExtremityShape.Cylinder`
    * :const:`tlp.EdgeExtremityShape.Diamond`
    * :const:`tlp.EdgeExtremityShape.GlowSphere`
    * :const:`tlp.EdgeExtremityShape.HalfCylinder`
    * :const:`tlp.EdgeExtremityShape.Hexagon`
    * :const:`tlp.EdgeExtremityShape.Pentagon`
    * :const:`tlp.EdgeExtremityShape.Ring`
    * :const:`tlp.EdgeExtremityShape.Sphere`
    * :const:`tlp.EdgeExtremityShape.Square`
    * :const:`tlp.EdgeExtremityShape.Star`

The sample code below set the shape of the selected nodes to a circle::

    viewShape = graph.getIntegerProperty("viewShape")
    viewSelection = graph.getBooleanProperty("viewSelection")

    for n in graph.getNodes():
        if viewSelection[n]:
            viewShape[n] = tlp.NodeShape.Circle

.. _graph-labels:

Labels of graph elements
------------------------

The labels associated to graph elements can modified through the **"viewLabel"** graph property (of type :class:`tlp.StringProperty`).
The font used to render the labels can be modified through the **"viewFont"** graph property (of type :class:`tlp.StringProperty`).
A font is described by a path to a TrueType font file (.ttf).

The sample code below labels nodes according to their id::

    viewLabel = graph.getStringProperty("viewLabel")

    for n in graph.getNodes():
        viewLabel[n] = "Node " + str(n.id)


The position of the label relative to the associated elements can also be modified through the **"viewLabelPosition"** graph property (of type :class:`tlp.IntegerProperty`).
Constants are defined for code readability, below is the exhaustive list:

    * :const:`tlp.LabelPosition.Center`
    * :const:`tlp.LabelPosition.Top`
    * :const:`tlp.LabelPosition.Bottom`
    * :const:`tlp.LabelPosition.Left`
    * :const:`tlp.LabelPosition.Right`

.. _borders-widths:

Border width of graph elements and labels
------------------------------------------

The border width of graph elements can be modified through the **"viewBorderWidth"** graph property (of type :class:`tlp.DoubleProperty`).
The border width of graph elements labels can be modified through the **"viewLabelBorderWidth"** graph property (of type :class:`tlp.DoubleProperty`).
The width is defined by a floating point value.

.. _graph-textures:

Applying a texture to nodes or edges
-------------------------------------

A texture can be applied when Tulip renders the graph elements. Setting a texture to graph elements can be done through
the **"viewTexture"** graph property (of type :class:`tlp.StringProperty`).
A texture is described by a path to an image file. Note that the image must have the same width and height for
correct texture loading.
