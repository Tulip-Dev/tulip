.. userHandbook documentation master file, created by
   sphinx-quickstart on Mon Jul  1 10:58:47 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

#################################################
Welcome to the Tulip user oriented documentation!
#################################################

Research by the information visualization community has shown that using a visual representation of data-sets enables faster analysis by the end users. Tulip, created by David AUBER, is a contribution to the area of information visualization, InfoViz.

.. image:: _images/tulip-screen.png
    :width: 600

Tulip offers the following features:

* Visualization and edition of graphs. All parts of the framework have been built with large graphs in mind, up to 10.000.000 elements.
* Navigation through geometric operations, extraction of subgraphs, filtering to change the representation.
* Metric computation, layout, selection, extraction of view and subgraphs.
* Graph management: one can work on several graphs at once, import and export them in various formats	(including CSV, GML, dot, snapshots).
* Plugin architecture: one can add new algorithms (generate a graph, spatialize it (i.e. layout), compute a metric...), ways of interactions, views...
* Scripting in Python, from within the GUI.


.. highlight:: c


.. _contents:

Contents:
=========

.. toctree::
   :maxdepth: 2

   gui
   tutorial_beginner
   workspace
   algorithm
   functions
   plugins_management


Indices and tables
==================

* :ref:`genindex`
* :ref:`search`


