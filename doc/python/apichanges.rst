.. py:currentmodule:: tulip

Important API changes
======================

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
