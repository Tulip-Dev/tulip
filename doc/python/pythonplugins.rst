.. py:currentmodule:: tulip

.. _tulippythonplugins:

Writing Tulip plugins in Python
===============================

The Tulip Python bindings offer the possibility to write Tulip plugins in pure Python.
You can easily develop and test Python plugins from the Tulip GUI through the "Plugins editor" tab
of the Tulip plugins editor accessible through de *Develop* button (see :ref:`Using the bindings from the Tulip Software GUI <usingBindingsInTulipGUI>`).

Below is the list of plugins that can be implemented in Python :

	* General algorithms
	* Selection algorithms
	* Color algorithms
	* Metric algorithms
	* Integer algorithms
	* Layout algorithms
	* Size algorithms
	* Export modules
	* Import modules
	
Developing a plugin consists in writing a Python module (in a specific .py file) that contains the implementation 
of a Python class deriving from another one specific to the plugin type.
The plugin is then registered in the plugin database through the use of 
specific functions from the :mod:`tuliplugins` module (see :ref:`tulipplugins module API <tulippluginsref>`).
Once the plugin written and functional, you can copy the source file(s) to the following directory :
<tulip_install_dir>/lib/tulip/python and it will be automatically loaded at Tulip startup.
The remaining of this section illustrates how to write each type of plugin.

Writing a general algorithm in Python
--------------------------------------

A general algorithm must implement the :class:`tlp.Algorithm` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class GeneralAlgorithmExample(tlp.Algorithm):
		def __init__(self, context):
			tlp.Algorithm.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def check(self):
			# This method is called before applying the algorithm on the input graph.
			# You can perform some precondition checks here.
			# See comments in the run method to know how to access to the input graph.
			
			# Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
			# and the second one can be used to provide an error message
			return (True, "Ok")
	
		def run(self):
			# This method is the entry point of the algorithm when it is called
			# and must contain its implementation.
			
			# The graph on which the algorithm is applied can be accessed through
			# the "graph" class attribute (see documentation of class tlp.Graph).
	
			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The method must return a boolean indicating if the algorithm
			# has been successfully applied on the input graph.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("GeneralAlgorithmExample", "General Algorithm Demo", "author", "26/07/2011", "infos", "1.0", "Python")

Writing a selection algorithm in Python
----------------------------------------

A selection algorithm must implement the :class:`tlp.BooleanAlgorithm` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class SelectionAlgorithmExample(tlp.BooleanAlgorithm):
		def __init__(self, context):
			tlp.BooleanAlgorithm.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def check(self):
			# This method is called before applying the algorithm on the input graph.
			# You can perform some precondition checks here.
			# See comments in the run method to know how to access to the input graph.
	
			# Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
			# and the second one can be used to provide an error message
			return (True, "Ok")
	
		def run(self):
			# This method is the entry point of the algorithm when it is called
			# and must contain its implementation.
	
			# The graph on which the algorithm is applied can be accessed through
			# the "graph" class attribute (see documentation of class tlp.Graph).

			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The result of this selection algorithm must be stored in the
			# boolean property accessible through the "result" class attribute
			# (see documentation to know how to work with graph properties).

			# The method must return a boolean indicating if the algorithm
			# has been successfully applied on the input graph.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("SelectionAlgorithmExample", "Selection Algorithm Demo", "author", "27/07/2011", "infos", "1.0", "Python")

Writing a color algorithm in Python
----------------------------------------		

A color algorithm must implement the :class:`tlp.ColorAlgorithm` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class ColorAlgorithmExample(tlp.ColorAlgorithm):
		def __init__(self, context):
			tlp.ColorAlgorithm.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def check(self):
			# This method is called before applying the algorithm on the input graph.
			# You can perform some precondition checks here.
			# See comments in the run method to know how to access to the input graph.
	
			# Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
			# and the second one can be used to provide an error message
			return (True, "Ok")
	
		def run(self):
			# This method is the entry point of the algorithm when it is called
			# and must contain its implementation.	
	
			# The graph on which the algorithm is applied can be accessed through
			# the "graph" class attribute (see documentation of class tlp.Graph).
	
			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The result of this color algorithm must be stored in the
			# color property accessible through the "result" class attribute
			# (see documentation to know how to work with graph properties).
	
			# The method must return a boolean indicating if the algorithm
			# has been successfully applied on the input graph.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("ColorAlgorithmExample", "Color Algorithm Demo", "author", "27/07/2011", "infos", "1.0", "Python")
	
Writing a metric algorithm in Python
----------------------------------------	
		
A metric algorithm must implement the :class:`tlp.DoubleAlgorithm` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class MetricAlgorithmExample(tlp.DoubleAlgorithm):
		def __init__(self, context):
			tlp.DoubleAlgorithm.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def check(self):
			# This method is called before applying the algorithm on the input graph.
			# You can perform some precondition checks here.
			# See comments in the run method to know how to access to the input graph.
	
			# Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
			# and the second one can be used to provide an error message
			return (True, "Ok")
	
		def run(self):
			# This method is the entry point of the algorithm when it is called
			# and must contain its implementation.
	
			# The graph on which the algorithm is applied can be accessed through
			# the "graph" class attribute (see documentation of class tlp.Graph).
	
			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The result of this measure algorithm must be stored in the
			# double property accessible through the "result" class attribute
			# (see documentation to know how to work with graph properties).
	
			# The method must return a boolean indicating if the algorithm
			# has been successfully applied on the input graph.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("MetricAlgorithmExample", "Metric Algorithm Demo", "author", "27/07/2011", "infos", "1.0", "Python")

Writing an integer algorithm in Python
----------------------------------------	

An integer algorithm must implement the :class:`tlp.IntegerAlgorithm` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class IntegerAlgorithmExample(tlp.IntegerAlgorithm):
		def __init__(self, context):
			tlp.IntegerAlgorithm.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def check(self):
			# This method is called before applying the algorithm on the input graph.
			# You can perform some precondition checks here.
			# See comments in the run method to know how to access to the input graph.
	
			# Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
			# and the second one can be used to provide an error message
			return (True, "Ok")
	
		def run(self):
			# This method is the entry point of the algorithm when it is called
			# and must contain its implementation.
	
			# The graph on which the algorithm is applied can be accessed through
			# the "graph" class attribute (see documentation of class tlp.Graph).
	
			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The result of this integer algorithm must be stored in the
			# integer property accessible through the "result" class attribute
			# (see documentation to know how to work with graph properties).
	
			# The method must return a boolean indicating if the algorithm
			# has been successfully applied on the input graph.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("IntegerAlgorithmExample", "Integer Algorithm Demo", "author", "27/07/2011", "infos", "1.0", "Python")
	
Writing a layout algorithm in Python
----------------------------------------

A layout algorithm must implement the :class:`tlp.LayoutAlgorithm` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class LayoutAlgorithmExample(tlp.LayoutAlgorithm):
		def __init__(self, context):
			tlp.LayoutAlgorithm.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def check(self):
			# This method is called before applying the algorithm on the input graph.
			# You can perform some precondition checks here.
			# See comments in the run method to know how to access to the input graph.
	
			# Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
			# and the second one can be used to provide an error message
			return (True, "Ok")
	
		def run(self):
			# This method is the entry point of the algorithm when it is called
			# and must contain its implementation.
	
			# The graph on which the algorithm is applied can be accessed through
			# the "graph" class attribute (see documentation of class tlp.Graph).
	
			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The result of this layout algorithm must be stored in the
			# layout property accessible through the "result" class attribute
			# (see documentation to know how to work with graph properties).
	
			# The method must return a boolean indicating if the algorithm
			# has been successfully applied on the input graph.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("LayoutAlgorithmExample", "Layout Algorithm Demo", "author", "27/07/2011", "infos", "1.0", "Python")
	
Writing a size algorithm in Python
----------------------------------------

A size algorithm must implement the :class:`tlp.SizeAlgorithm` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class SizeAlgorithmExample(tlp.SizeAlgorithm):
		def __init__(self, context):
			tlp.SizeAlgorithm.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def check(self):
			# This method is called before applying the algorithm on the input graph.
			# You can perform some precondition checks here.
			# See comments in the run method to know how to access to the input graph.
	
			# Must return a tuple (boolean, string). First member indicates if the algorithm can be applied
			# and the second one can be used to provide an error message
			return (True, "Ok")
	
		def run(self):
			# This method is the entry point of the algorithm when it is called
			# and must contain its implementation.
	
			# The graph on which the algorithm is applied can be accessed through
			# the "graph" class attribute (see documentation of class tlp.Graph).
	
			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The result of this size algorithm must be stored in the
			# size property accessible through the "result" class attribute
			# (see documentation to know how to work with graph properties).
	
			# The method must return a boolean indicating if the algorithm
			# has been successfully applied on the input graph.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("SizeAlgorithmExample", "Size Algorithm Demo", "author", "27/07/2011", "infos", "1.0", "Python")

Writing an export module in Python
-----------------------------------

An export module must implement the :class:`tlp.ExportModule` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class ExportModuleExample(tlp.ExportModule):
		def __init__(self, context):
			tlp.ExportModule.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def exportGraph(self, os):
			# This method is called to export a graph.
			# The graph to export is accessible through the "graph" class attribute
			# (see documentation of class tlp.Graph).
			
			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The os parameter is an output file stream (initialized by the Tulip GUI
			# or by the tlp.exportGraph function.). 
			# To write data to the file, you have to use the following syntax :

			# write the number of nodes and edges to the file
			os << self.graph.numberOfNodes() << "\n"
			os << self.graph.numberOfEdges() << "\n"
	
			# The method must return a boolean indicating if the
			# graph has been successfully exported.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("ExportModuleExample", "Export Module Demo", "author", "27/07/2011", "infos", "1.0", "Python")

Writing an import module in Python
-----------------------------------		

An import module must implement the :class:`tlp.ImportModule` interface as illustrated below::

	from tulip import *
	import tulipplugins
	
	class ImportModuleExample(tlp.ImportModule):
		def __init__(self, context):
			tlp.ImportModule.__init__(self, context)
			# you can add parameters to the plugin here through the following syntax
			# self.add<Type>Parameter("<paramName>", "<paramDoc>", "<paramDefaultValue>")
			# (see documentation of class tlp.WithParameter to see what types of parameters are supported)
	
		def importGraph(self):
			# This method is called to import a new graph.
			# An empty graph to populate is accessible through the "graph" class attribute
			# (see documentation of class tlp.Graph).
	
			# The parameters provided by the user are stored in a Tulip DataSet 
			# and can be accessed through the "dataSet" class attribute
			# (see documentation of class tlp.DataSet).
	
			# The method must return a boolean indicating if the
			# graph has been successfully imported.
			return True
	
	# The line below does the magic to register the plugin to the plugin database
	# and updates the GUI to make it accessible through the menus.
        tulipplugins.registerPluginOfGroup("ImportModuleExample", "Import Module Demo", "author", "27/07/2011", "infos", "1.0", "Python")
			
