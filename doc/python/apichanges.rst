.. py:currentmodule:: tulip

Release notes and API changes
=============================

Tulip-Python 4.10
-----------------

Bugs fixes
^^^^^^^^^^

The following bugs have been fixed since the 4.9 release:

  * When calling a graph property algorithm trough dedicated methods of the :class:`tlp.Graph` class,
    for instance :meth:`tlp.Graph.applyDoubleAlgorithm`, ensure values stored in the result property
    of the algorithm are not resetted to the default ones before calling the algorithm.

  * Regression when writing a Tulip plugin in Python, it was no more possible to modify the content of the dictionnary
    storing the plugin parameters (and thus, output plugin parameters were no more taken into account).

  * On Linux platform, a side effect that could lead to a segmentation fault when importing some dynamic
    Python modules (for instance those internally used by scikit-learn) after the Tulip ones.

New methods and API improvements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following methods have been added to the API:

   * :meth:`tlp.WithParameter.addNumericPropertyParameter`

   * a new signature for the method :meth:`tlp.Graph.inducedSubGraph` taking an input selection of nodes and edges
     in parameter instead of a set of nodes

The signatures of the methods for setting all nodes / edges values in a graph property,
for instance :meth:`tlp.BooleanProperty.setAllNodeValue`, :meth:`tlp.DoubleProperty.setAllEdgeValue`,
have been updated. They now take an optionnal :class:`tlp.Graph` parameter, enabling to only
set the nodes / edges values contained in that sub-graph.

Documentation for Tulip C++ plugins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A :ref:`new section <tulippluginsdoc>` in that documentation has been added detailing the C++ plugins
bundled with Tulip that can be called from Python (for instance the great force directed layout algorithms
from OGDF). The purpose of each plugin is given but also an exhaustive list of its parameters.

Improvements for working with Tulip color scales
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As mapping colors to graph elements according to various metrics are quite a common task when
working with Tulip, several improvements have been added to facilitate their use in Python.

.. _colorScalesNewSyntax:

No need to use the :class:`tlp.ColorScale` class directly anymore
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

For commodity of use in the Python world, it is now possible to define Tulip color scales
trough the following ways:

  * using a list of :class:`tlp.Color` that will define a color scale with regular stop points

  * using a dictionnary with float keys (between 0.0 and 1.0) and :class:`tlp.Color` values

For instance, if one wants to apply a color mapping to graph nodes according to their degrees,
the following script can now be used::

  # computes a double property containing the degree of each node
  degree = tlp.DoubleProperty(graph)
  graph.applyDoubleAlgorithm('Degree', degree)

  # creates a regular color scale going from blue to yellow to red
  colorScale = [tlp.Color.Blue, tlp.Color.Yellow, tlp.Color.Red]
  # a dictionnary can also be used (allow to define non regular scales)
  # colorMap = {0: tlp.Color.Blue, 0.5: tlp.Color.Yellow, 1.0: tlp.Color.Red}

  # sets the parameters for the 'Color Mapping' color algorithm
  colorMappingParams = tlp.getDefaultPluginParameters('Color Mapping', graph)
  colorMappingParams['input property'] = degree
  colorMappingParams['color scale'] = colorScale
  # colorMappingParams['color scale'] = colorMap

  # computes the color mapping
  graph.applyColorAlgorithm('Color Mapping', colorMappingParams)

Nevertheless for backward compatibilty, instances of the :class:`tlp.ColorScale` class can still be created.

Tulip predefined color scales can now be easily retrieved from Python
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

Tulip is bundled with a lot of predefined color scales (in particular, the great ones from `Color Brewer <http://colorbrewer2.org/>`_).
You can get a preview of them in the color scale config dialog inside the main Tulip GUI. The dialog can be displayed
trough the 'Edit -> Color scales management' main menu entry or by double clicking on the color scale parameter in the
configuration table of the 'Color Mapping' algorithm.

These color scales can now be easily retrieved from Python by using the static :class:`tulipgui.tlpgui.ColorScalesManager` class.
That class lies in the :mod:`tulipgui` module as Tulip uses Qt under the hood for managing predefined color scales.
That class also allows to register new color scales in a local persistent database for further reuse in every Python working session.

For instance, if one wants to use the '9-class OrRd' scale from `Color Brewer <http://colorbrewer2.org/>`_ in the above example,
the instruction below can now be used::

  colorMappingParams['color scale'] = tlpgui.ColorScalesManager.getColorScale('OrRd_9')


Tulip-Python 4.9
----------------

Special syntax for automatic creation of graph properties through type inference
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For convenient use, in particular when importing graph data, it is now possible to create
graph properties on the fly without prior calls to the methods dedicated to these tasks (e.g.
:meth:`tlp.Graph.getBooleanProperty`, :meth:`tlp.Graph.getIntegerProperty`, :meth:`tlp.Graph.getDoubleProperty`, ...).
It allows to save a consequent lines of code but also to work faster using the tulip Python API.

For instance, consider the following task : importing nodes data from a JSON file.
As an example, we will use the following file containing data about cars with different types : string, float and integer,
and will create one node for each car and one graph property (correctly typed) for each data field.

.. code-block:: javascript

  [
    {
      "acceleration": 12.0,
      "car": "Chevrolet Chevelle Malibu",
      "cylinders": 8,
      "displacement": 307.0,
      "horsepower": 130.0,
      "id": 0,
      "model": 70,
      "mpg": 18.0,
      "origin": "US",
      "weight": 3504.0
    },
    {
      "acceleration": 11.5,
      "car": "Buick Skylark 320",
      "cylinders": 8,
      "displacement": 350.0,
      "horsepower": 165.0,
      "id": 1,
      "model": 70,
      "mpg": 15.0,
      "origin": "US",
      "weight": 3693.0
    },
    ...
  ]

To correctly import those data into Tulip graph nodes, the sample script below is sufficient::

  cars = json.loads(open('cars.json').read())

  for car in cars:
    n = graph.addNode()
    for k, v in car.items():
      graph[k][n] = v

Before Tulip 4.9, it was necessary to create the graph properties first by calling the following methods::

  graph.getDoubleProperty('acceleration')
  graph.getStringProperty('car')
  graph.getIntegerProperty('cylinders')
  graph.getDoubleProperty('displacement')
  graph.getDoubleProperty('horsepower')
  graph.getIntegerProperty('id')
  graph.getIntegerProperty('model')
  graph.getDoubleProperty('mpg')
  graph.getStringProperty('origin')
  graph.getDoubleProperty('weight')

Improvements regarding the declaration and transmission of file / directory parameters for plugins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When implementing Tulip plugins in Python (see :ref:`Writing Tulip plugins in Python <tulippythonplugins>`),
it can be usefull to declare a file / directory parameter to perform
a variety of tasks during the plugin execution: reading / writing graph data to a file, logging messages to a file, ...

Prior to the 4.9 release, it was necessary to declare a file parameter in the plugin constructor the way below::

  self.addStringParameter('file::filename', 'the path to an existing file')

The "file::" prefix acts as a hint for the Tulip GUI to create a dialog in order to easily pick a file from the filesystem.

To retrieve the path of the file selected by the user, the following instruction had to be used in the plugin main method
(:meth:`tlp.ImportModule.importGraph`, :meth:`tlp.ExportModule.exportGraph`, :meth:`tlp.Algorithm.run`)::

  filename = self.dataSet['file::filename']

That way to proceed is not really intuitive so Tulip 4.9 introduces a more user friendly mechanism to work
with file / directory parameters : two new methods have been added in order to easily declare file / directory parameters
(:meth:`tlp.WithParameter.addFileParameter`, :meth:`tlp.WithParameter.addDirectoryParameter`)
and it is no more needed to explicitely write the "file::" prefix.

So the recommended way to declare a file parameter in the plugin constructor is now the one below::

  self.addFileParameter('filename', True, 'the path to an existing file')

And to get the path of the file selected by the user, you can now simply write in the plugin main method::

  filename = self.dataSet['filename']

In the same manner, when transmitting a file parameter to a plugin trough a dictionnary (see :ref:`Applying an algorithm on a graph <applyGraphAlgorithm>`),
the "file::" prefix is no more required to be written.

Nevertheless for backward compatibility, the old mechanism can still be used.

Tulip-Python 4.8.1
------------------

New methods for getting / setting graph properties values for nodes and edges added
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Convenient methods that rely on the use of a dictionnary for setting and getting
properties values for nodes and edges have been added to the :class:`tlp.Graph` class :

  * :meth:`tlp.Graph.getNodePropertiesValues` : returns a dictionnary filled with all graph properties values for a given node
  * :meth:`tlp.Graph.getEdgePropertiesValues` : returns a dictionnary filled with all graph properties values for a given edge
  * :meth:`tlp.Graph.setNodePropertiesValues` : sets multiple properties values stored in a dictionnary for a given node
  * :meth:`tlp.Graph.setEdgePropertiesValues` : sets multiple properties values stored in a dictionnary for a given edge

For instance, the sample code below sets multiple graph view properties values for each node of a graph::

  def getRandomFontAwesomeIcon():
    iconKeys = vars(tlp.TulipFontAwesome).keys()
    while 1:
      attName = random.choice(list(iconKeys))
      attr = getattr(tlp.TulipFontAwesome, attName)
      if not attName.startswith('_') and type(attr) == str:
        return attr

  def getRandomColor():
    r = int(random.random()*255)
    g = int(random.random()*255)
    b = int(random.random()*255)
    return tlp.Color(r, g, b)

  def getRandomSize(minSize, maxSize):
    return minSize + random.random() * (maxSize - minSize)

  for n in graph.getNodes():
    values = {'viewShape': tlp.NodeShape.FontAwesomeIcon,
              'viewColor' : getRandomColor(),
              'viewSize' : getRandomSize(tlp.Size(0.1), tlp.Size(1)),
              'viewFontAwesomeIcon' : getRandomFontAwesomeIcon()}
    graph.setNodePropertiesValues(n, values)

Tulip-Python 4.8
----------------

.. _deprecatedDataSet:

Deprecation of the direct use of the :class:`tlp.DataSet` class
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Formerly, the class :class:`tlp.DataSet` was used to transmit parameters to the algorithms
that can be executed on an instance of a :class:`tlp.Graph` class (see :ref:`Applying an algorithm on a graph <applyGraphAlgorithm>`).

For commodity of use in the Python world, that class is now internally mapped to a dictionnary indexed by string keys (parameters names).
To get a dictionnary filled with default parameters for an algorithm,
you can use the :func:`tlp.getDefaultPluginParameters` function.

Nevertheless for backward compatibilty, it is still possible to create
instance of that class.

.. _deprecatedStringCollection:

Deprecation of the direct use of the :class:`tlp.StringCollection` class
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :class:`tlp.StringCollection` class represents a list of selectable string entries that can be used as plugin parameter.
Formerly, to select the string to transmit to a plugin, the following code has to be used::

  # get defaut parameters for the 'FM^3 (OGDF)' layout plugin
  params = tlp.getDefaultPluginParameters('FM^3 (OGDF)')
  # set 'Page Format' as 'Landscape'
  params['Page Format'].setCurrent('Landscape')

For syntactic sugar, the :class:`tlp.StringCollection` class does not need
to be instantiated anymore to transmit the string to the algorithm.
The creation of the string collection is handled internally
and you can now simply write::

  # get defaut parameters for the 'FM^3 (OGDF)' layout plugin
  params = tlp.getDefaultPluginParameters('FM^3 (OGDF)')
  # set 'Page Format' as 'Landscape'
  params['Page Format'] = 'Landscape'

If the provided string is not contained in the string collection associated
to a plugin parameter, an exception will be thrown when trying to execute the plugin
trough dedicated methods/functions.

Nevertheless for backward compatibilty, it is still possible to create
instance of that class.
