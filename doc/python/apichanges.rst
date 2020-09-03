.. py:currentmodule:: tulip

Release notes and API changes
=============================

Tulip-Python 5.3.1
------------------

API improvements
^^^^^^^^^^^^^^^^

Values for graph properties of type :class:`tlp.ColorProperty`, :class:`tlp.LayoutProperty` and
:class:`tlp.SizeProperty` can be set using tuples since Tulip-Python 5.1. But some restrictions
regarding the types they should contain was making this feature confusing to use. In particular,
trying to set a node value for a :class:`tlp.LayoutProperty` or a :class:`tlp.SizeProperty`
using a tuple filled with ``int`` was raising an error as a tuple only filled with ``float``
was expected.

These type restrictions have been removed and you can now safely write::

  viewLayout = graph.getLayoutProperty('viewLayout')
  viewLayout[n] = (0, 0)
  viewLayout[n] = (10.5, 0, 1.5)

  viewSize = graph.getSizeProperty('viewSize')
  viewSize[n] = (2, 2)
  viewSize[n] = (1.5, 1, 1)

  # or even shorter
  graph['viewLayout'][n] = (0, 0)
  graph['viewLayout'][n] = (10.5, 0, 1.5)

  graph['viewSize'][n] = (2, 2)
  graph['viewSize'][n] = (1.5, 1, 1)

Tulip-Python 5.2
-----------------

Testing if a node or an edge belongs to a graph using classical Python idioms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

It is now possible to test if a node or an edge is contained in a graph
using the classical Python idiom based on the in keyword.

To test if an instance of :class:`tlp.node` belongs to an instance
of :class:`tlp.Graph`, you can now write::

  if n in graph:
    # n belongs to the graph
  else:
    # n does not belong to the graph

To test if an instance of :class:`tlp.edge` belongs to an instance
of :class:`tlp.Graph`, you can now write::

  if e in graph:
    # e belongs to the graph
  else:
    # e does not belong to the graph

You can also test if an edge defined by a tuple of two instances of
:class:`tlp.node` belongs to an instance of :class:`tlp.Graph`::

  if (src, tgt) in graph:
    # the edge belongs to the graph
  else:
    # the edge does not belong to the graph

Tulip-Python 5.1
-----------------

This is mainly a bugs fixes release for the Tulip bindings with some API improvements.
The focus since the 5.0 release has been on stabilizing the API by writing non regression tests.

Bugs fixes
^^^^^^^^^^

The following bugs have been corrected since the 5.0 release:

  * methods of class :class:`tlp.VoronoiDiagram` were not callable due to a visibility issue.
  * on MacOS, the impossibility to import the :mod:`tulipgui` module when using the Anaconda Python distribution.
  * segfaults when calling the iterator version of the methods :meth:`tlp.Graph.delNodes` and :meth:`tlp.Graph.delEdges`
  * some other internal issues

Also, the automatic graph property creation by type inference introduced in Tulip-Python 4.9
should now behave correctly for all supported property types (especially list ones).

API improvements
^^^^^^^^^^^^^^^^

Values for graph properties of type :class:`tlp.ColorProperty`, :class:`tlp.LayoutProperty` and
:class:`tlp.SizeProperty` can now be set using tuples.

To modify the content of a :class:`tlp.ColorProperty`, it is now possible to use the
following syntax::

  viewColor = graph.getColorProperty('viewColor')
  viewColor[n] = (100, 0, 255)
  viewColor[n] = (0, 0, 255, 200)

  # or even shorter
  graph['viewColor'][n] = (100, 0, 255)
  graph['viewColor'][n] = (0, 0, 255, 200)

To modify the content of a :class:`tlp.LayoutProperty` or :class:`tlp.SizeProperty`,
it is now possible to use the following syntax::

  viewLayout = graph.getLayoutProperty('viewLayout')
  viewLayout[n] = (0.0, 0.0)
  viewLayout[n] = (10.0, 10.0, 10.0)

  viewSize = graph.getSizeProperty('viewSize')
  viewSize[n] = (2.0, 2.0)
  viewSize[n] = (1.0, 1.0, 1.0)

  # or even shorter
  graph['viewLayout'][n] = (0.0, 0.0)
  graph['viewLayout'][n] = (10.0, 10.0, 10.0)

  graph['viewSize'][n] = (2.0, 2.0)
  graph['viewSize'][n] = (1.0, 1.0, 1.0)

.. warning::

  In order to successfully set a property values using a tuple, the types of the elements
  it contains and its length are constrained according to the property type:

    * for :class:`tlp.ColorProperty`: the tuple must contain 3 or 4 integers between 0 and 255
    * for :class:`tlp.LayoutProperty` and :class:`tlp.SizeProperty`: the tuple must contain 2 or 3
      float (meaning the value will fail to be set if the tuple contains integers)

Another slight API change is that now optional parameters for functions and class methods
can now be used as keyword arguments. Signature of functions and methods have been adapted
accordingly in the whole documentation.

Removal of the tulipogl module
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Due to its small content, the tulipogl module has been removed. Nevertheless, the features it offered
have been splitted in the :mod:`tulip` and :mod:`tulipgui` modules:

  * :ref:`parametric-curves`
  * :class:`tulipgui.tlpgui.GlGraphInputData`
  * :class:`tulipgui.tlpgui.GlGraphRenderingParameters`

Tulip-Python 5.0
-----------------

The main efforts on that release have been done in a better integration of the Python development
features in the main Tulip software GUI. A revamped Python IDE window is now available centralizing Python
script and Python plugin development instead of having those spread all around in the interface
(see :ref:`Using the bindings from the Tulip Software GUI <usingBindingsInTulipGUI>`).

Regarding the bindings, this is mainly a bugs and memory leaks fix release with few API changes that benefits
from the great performance improvements recently integrated in the tulip-core C++ library.

New Tulip plugins have also been added (see :ref:`updated list <tulippluginsdoc>`), notably
a bunch of import ones : social network models, Bibtex files, GraphML files, npm package dependencies graph, ...

Bugs fixes
^^^^^^^^^^

The following bugs have been corrected since the 4.10 release:

  * fix a regression that prevented to use :class:`tlp.Vec3f` class as an alias for :class:`tlp.Size`

  * fix the impossibility to import the :mod:`tulip` module when using Python 3.6 on Linux

  * string constants defined in class :class:`tlp.TulipFontAwesome` were not valid when using Python 3.x
    due to a conversion issue

  * fix crash when not passing parameters to Tulip export plugins in function :func:`tlp.exportGraph`

  * fix the use of the :ref:`file parameters improvement feature <fileParametersImprovement>` introduced
    in Tulip-Python 4.9 for import and export plugins

  * fix a regression in the :ref:`string collection plugin parameter improvement <deprecatedStringCollection>`
    when writing Tulip Python plugins

  * fix possible crashs that could occur when a :class:`tlp.Graph` instance owned by Python and not C++
    (for instance when using :func:`tlp.newGraph()`) is garbage collected

  * on windows platform when using the Python IDE from the main Tulip GUI:

    * the 'Pause script' feature is now correctly handled

    * fix initialisation of tulip modules when multiple Python distributions are installed on the
      host system (for instance those from Anaconda and Python.org)

New methods and API changes
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following methods have been added:

  * :meth:`tlp.Graph.nodes`
  * :meth:`tlp.Graph.nodePos`
  * :meth:`tlp.Graph.getSink`
  * :meth:`tlp.Graph.edges`
  * :meth:`tlp.Graph.edgePos`
  * :meth:`tlp.Graph.allEdges`
  * :meth:`tlp.Graph.popIfNoUpdates`
  * :meth:`tlp.WithParameter.addUnsignedIntegerParameter`
  * :meth:`tlp.Plugin.icon`
  * :meth:`tlp.ImportModule.fileExtensions`
  * :meth:`tlp.ImportModule.gzipFileExtensions`
  * :meth:`tlp.ImportModule.allFileExtensions`
  * :meth:`tlp.NumericProperty.copyProperty`

The signature of the method :meth:`tlp.Graph.inducedSubGraph` and :meth:`tlp.Graph.createMetaNode` have been updated,
they should now take a list of :class:`tlp.node` instead of a set of :class:`tlp.node` as parameter.

The signatures update from the last release of the methods for setting all nodes / edges values in a graph property,
for instance :meth:`tlp.BooleanProperty.setAllNodeValue`, :meth:`tlp.DoubleProperty.setAllEdgeValue`,
have been deprecated and will be removed in the next release. You should now use dedicated methods instead, for
instance :meth:`tlp.BooleanProperty.setValueToGraphNodes`, :meth:`tlp.DoubleProperty.setValueToGraphEdges`.

The methods :meth:`tlp.Graph.setNodePropertiesValues` and :meth:`tlp.Graph.setEdgePropertiesValues`
now benefit from the type inference feature introduced in Tulip-Python 4.9, meaning you can now create
properties and set their values in one call. The signatures of the methods :meth:`tlp.Graph.addNode`
and :meth:`tlp.Graph.addEdge` have also been updated in order to set properties values while adding elements
to a graph.

All graph properties classes now have methods to return iterators on graph elements who have a
specific value: :meth:`tlp.DoubleProperty.getNodesEqualTo`, :meth:`tlp.StringProperty.getEdgesEqualTo`, ...

Tulip-Python 4.10
-----------------

Bugs fixes
^^^^^^^^^^

The following bugs have been fixed since the 4.9 release:

  * When calling a graph property algorithm through dedicated methods of the :class:`tlp.Graph` class,
    for instance :meth:`tlp.Graph.applyDoubleAlgorithm`, ensure values stored in the result property
    of the algorithm are not resetted to the default ones before calling the algorithm.

  * Regression when writing a Tulip plugin in Python, it was no more possible to modify the content of the dictionary
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
have been updated. They now take an optional :class:`tlp.Graph` parameter, enabling to only
set the nodes / edges values contained in that subgraph.

The method :meth:`tlp.Graph.attributeExist` is now deprecated, :meth:`tlp.Graph.existAttribute`
should now be used instead.

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

No need to use the :class:`tlp.ColorScale` class directly as plugin parameter anymore
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

For commodity of use in the Python world, it is now possible to define Tulip color scales
through the following ways:

  * using a list of :class:`tlp.Color` that will define a color scale with regular stop points

  * using a dictionary with float keys (between 0.0 and 1.0) and :class:`tlp.Color` values

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
through the 'Edit -> Color scales management' main menu entry or by double clicking on the color scale parameter in the
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
It allows to avoid many lines of code but also to work faster using the tulip Python API.

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

.. _fileParametersImprovement:

Improvements regarding the declaration and transmission of file / directory parameters for plugins
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When implementing Tulip plugins in Python (see :ref:`Writing Tulip plugins in Python <tulippythonplugins>`),
it can be useful to declare a file / directory parameter to perform
various tasks during the plugin execution: reading / writing graph data to a file, logging messages to a file, ...

Prior to the 4.9 release, it was necessary to declare a file parameter in the plugin constructor the way below::

  self.addStringParameter('file::filename', 'the path to an existing file')

The "file::" prefix acts as a hint for the Tulip GUI to create a dialog in order to easily pick a file from the filesystem.

To retrieve the path of the file selected by the user, the following instruction had to be used in the plugin main method
(:meth:`tlp.ImportModule.importGraph`, :meth:`tlp.ExportModule.exportGraph`, :meth:`tlp.Algorithm.run`)::

  filename = self.dataSet['file::filename']

Sice this approach is not really intuitive so Tulip 4.9 introduces a more user_friendly mechanism to work
with file / directory parameters : two new methods have been added in order to easily declare file / directory parameters
(:meth:`tlp.WithParameter.addFileParameter`, :meth:`tlp.WithParameter.addDirectoryParameter`)
and it is no more needed to explicitely write the "file::" prefix.

So the recommended way to declare a file parameter in the plugin constructor is now the one below::

  self.addFileParameter('filename', True, 'the path to an existing file')

And to get the path of the file selected by the user, you can now simply write in the plugin main method::

  filename = self.dataSet['filename']

In the same manner, when transmitting a file parameter to a plugin through a dictionary (see :ref:`Applying an algorithm on a graph <applyGraphAlgorithm>`),
the "file::" prefix is no more required to be written.

Nevertheless for backward compatibility, the old mechanism can still be used.

Tulip-Python 4.8.1
------------------

New methods for getting / setting graph properties values for nodes and edges added
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Convenient methods that rely on the use of a dictionary for setting and getting
properties values for nodes and edges have been added to the :class:`tlp.Graph` class :

  * :meth:`tlp.Graph.getNodePropertiesValues` : returns a dictionary filled with all graph properties values for a given node
  * :meth:`tlp.Graph.getEdgePropertiesValues` : returns a dictionary filled with all graph properties values for a given edge
  * :meth:`tlp.Graph.setNodePropertiesValues` : sets multiple properties values stored in a dictionary for a given node
  * :meth:`tlp.Graph.setEdgePropertiesValues` : sets multiple properties values stored in a dictionary for a given edge

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
              'viewIcon' : getRandomFontAwesomeIcon()}
    graph.setNodePropertiesValues(n, values)

Tulip-Python 4.8
----------------

.. _deprecatedDataSet:

Deprecation of the direct use of the :class:`tlp.DataSet` class
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Formerly, the class :class:`tlp.DataSet` was used to transmit parameters to the algorithms
that can be executed on an instance of a :class:`tlp.Graph` class (see :ref:`Applying an algorithm on a graph <applyGraphAlgorithm>`).

For commodity of use in the Python world, that class is now internally mapped to a dictionary indexed by string keys (parameters names).
To get a dictionary filled with default parameters for an algorithm,
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
through dedicated methods/functions.

Nevertheless for backward compatibilty, it is still possible to create
instance of that class.
