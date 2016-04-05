.. py:currentmodule:: tulip

Important API changes
======================

Since Tulip 4.8.1
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

Since Tulip 4.8
-----------------

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
Formerly, to select the string to transmit to a plugin, the following code have to be used::

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
