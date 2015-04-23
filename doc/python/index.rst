.. Tulip Python documentation master file, created by
   sphinx-quickstart on Sat Apr  2 17:21:15 2011.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. highlight:: python

Welcome to Tulip Python documentation!
=================================================

Tulip_ is an information visualization framework written in C++ dedicated to the analysis and visualization of graphs.
Tulip Python is a set of modules that exposes to Python almost all the content of the Tulip_ C++ API. The bindings
has been developed with the SIP_ tool from Riverbank_. The main
features provided by the bindings are listed below :

* creation and manipulation of graphs
* storage of data on graph elements (float, integer, boolean, color, size, coordinate, list, ...)
* application of algorithms of different types on graphs (layout, metric, clustering, ...)
* the ability to write Tulip plugins in pure Python

The bindings can be used inside the Tulip software GUI in order to run scripts on the current visualized graph.
Starting from Tulip 3.6, the bindings can also be used outside Tulip through the classical Python interpreter.

Contents
=================================================
.. toctree::
   :maxdepth: 3

   gettingstarted
   manual
   pythonplugins 
   tulipreference
   tulipoglreference
   tulipguireference
   tulippluginsreference

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`

.. _Tulip: http://www.tulip-software.org/

.. _SIP: http://www.riverbankcomputing.co.uk/software/sip/intro

.. _Riverbank: http://www.riverbankcomputing.co.uk/
