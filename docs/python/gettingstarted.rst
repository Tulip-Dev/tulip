.. py:currentmodule:: tulip

Getting started
===============

Using the bindings from the Tulip Software GUI
-----------------------------------------------

A lightweight Python IDE can be accessed through the graphical interface of Tulip in order to run scripts on
the graph currently visualized. In Tulip 3.6, it is integrated as a view plugin named "Python Script view"
and can be accessed through the "View" menu. The interface of this component is composed of three tabs :

	* the "Script editor" tab (see :ref:`Figure 1`) that allows to write scripts that can be applied to the
	  graph currently loaded in Tulip. To do so, the "main(graph)" has to be defined
          and is used as the script entry point. The graph currently loaded is wrapped 
          as a :class:`tlp.Graph` object and provided as parameter of the "main" function.
	  The currently edited script can be launched through the control panel located
          in the lower part of the interface. Once started, the script execution can be
          stopped at any time.

.. figure:: pythonScriptViewGUI1.png
   :align: center

   Figure 1: Screenshot of the "Script editor" tab from the Tulip Python Script view.

	* the "Modules editor" tab (see :ref:`Figure 2`) that enables to write and edit 
	  Python modules that can be immediatly imported in the main script.

.. figure:: pythonScriptViewGUI2.png
   :align: center

   Figure 2: Screenshot of the "Modules editor" tab from the Tulip Python Script view.

	* the "Interactive session" tab (see :ref:`Figure 3`) that provides an interactive
          Python shell. The graph currently loaded in Tulip is wrapped to a :class:`tlp.Graph`
          object and bound to the "graph" variate.

.. figure:: pythonScriptViewGUI3.png
   :align: center

   Figure 3: Screenshot of the "Interactive session" tab from the Tulip Python Script view.


Using the bindings from the Python Interpreter
----------------------------------------------

The Tulip Python bindings can also be used through the classical Python Interpreter. But some setup has to be done
before importing the :mod:`tulip` module. 

First, the path to the :mod:`tulip` module must be provided to Python.
In the following, <tulip_install_dir> represents the root directory of a Tulip installation.
The Tulip Python module is installed in the following directory according to your system :

	* Linux : <tulip_install_dir>/lib

	* Windows : <tulip_install_dir>/bin	

This path has to be added to the list of Python module search path. To do so, you can add it in the **PYTHONPATH**
environment variable or add it to the :data:`sys.path` list.	

Second, your system must be able to find the Tulip C++ libraries in order to use the bindings. These libraries are
also installed in the directory provided above. You have to add this path to the **LD_LIBRARY_PATH** environment variable
on Linux or to the **PATH** environment variable on Windows.

You should now be able to import the :mod:`tulip` module through the Python shell. Issue the following command
at the shell prompt to perform that task::

	>>> from tulip import *

Important, if you want to use Tulip algorithms implemented as plugins written in C++ (e.g. graph layout algorithms), 
you have to load them before being able to call them (see :func:`tlp.applyAlgorithm`, :meth:`tlp.Graph.computeLayoutProperty`, ...).
To load all the Tulip plugins written in C++, you have to execute the following sequence of command::
	
	>>> tlp.initTulipLib()
	>>> tlp.loadPlugins()





