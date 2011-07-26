"""
This module provides utility functions to register Tulip plugins written in Python in the plugins database.
"""

from tulip import *
pluginFactory = {}
pluginModules = {}
testMode = False

def setTestMode(mode):
  global testMode
  testMode = mode

def getCallingModuleName():
   import sys
   f = sys._current_frames().values()[0]
   f = f.f_back
   return f.f_back.f_globals['__name__']
   
def reloadTulipPythonPlugin(pluginName):
   if pluginName in pluginModules: 
    module = pluginModules[pluginName]
    code = ""
    code += "import " + module + "\n"
    code += "reload(" + module + ")\n"
    exec(code)
   
def reloadTulipPythonPlugins():
   for plugin in pluginModules.keys():
      reloadTulipPythonPlugin(plugin)

def updateTulipMenus():
  tulipUtilsOk = True
  try:
    import tuliputils
  except ImportError:
    tulipUtilsOk = False
  if tulipUtilsOk:
    tuliputils.updatePluginsMenus()

def registerAlgorithmPlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerAlgorithmPlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip general algorithm written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.Algorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
	
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.AlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.AlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerAlgorithmPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerAlgorithmPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip general algorithm written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.Algorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the Algorithm -> General menu from the Tulip Software GUI)
	:type group: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.AlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.AlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerLayoutPlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerLayoutPlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip layout algorithm written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.LayoutAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.LayoutAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.LayoutAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerLayoutPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerLayoutPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip layout algorithm written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.LayoutAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the Algorithm -> Layout menu from the Tulip Software GUI)
	:type group: string
	"""
	
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.LayoutAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.LayoutAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerDoublePlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerDoublePlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip double algorithm (metric) written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.DoubleAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
	
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.DoubleAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.DoubleAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerDoublePluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerDoublePluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip double algorithm (metric) written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.DoubleAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the Algorithm -> Measure menu from the Tulip Software GUI)
	:type group: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.DoubleAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.DoubleAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerIntegerPlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerIntegerPlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip integer algorithm written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.IntegerAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.IntegerAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.IntegerAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerIntegerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerIntegerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip integer algorithm written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.IntegerAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the Algorithm -> Integer menu from the Tulip Software GUI)
	:type group: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.IntegerAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.IntegerAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerBooleanPlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerBooleanPlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip boolean algorithm (selection) written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.BooleanAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.BooleanAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.BooleanAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerBooleanPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerBooleanPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip boolean algorithm (selection) written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.BooleanAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the Algorithm -> Selection menu from the Tulip Software GUI)
	:type group: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.BooleanAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.BooleanAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerSizePlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerSizePlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip size algorithm written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.SizeAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.SizeAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.SizeAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerSizePluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerSizePluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip size algorithm written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.SizeAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the Algorithm -> Size menu from the Tulip Software GUI)
	:type group: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.SizeAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.SizeAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerColorPlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerColorPlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip color algorithm written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.ColorAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.ColorAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ColorAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerColorPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerColorPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip color algorithm written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.ColorAlgorithm`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the Algorithm -> Color menu from the Tulip Software GUI)
	:type group: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.ColorAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ColorAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerImportPlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerImportPlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip import module written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.PythonImportModule`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.ImportModuleFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ImportModuleFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerImportPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerImportPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip import module written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.PythonImportModule`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the File -> Import menu from the Tulip Software GUI)
	:type group: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.ImportModuleFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ImportModuleFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerExportPlugin(pluginClassName, pluginName, author, date, info, release):
  
	"""
	tulipplugins.registerExportPlugin(pluginClassName, pluginName, author, date, info, release)
	
	Registers a Tulip export module written in Python in the plugins database.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.PythonExportModule`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.ExportModuleFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ExportModuleFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return str("")\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()
        
def registerExportPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):
  
	"""
	tulipplugins.registerExportPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)
	
	Registers a Tulip export module written in Python in the plugins database and inserts it in a specific group.
	
	:param pluginClassName: the name of the Python class implementing the plugin (deriving from :class:`tlp.PythonExportModule`)
	:type pluginClassName: string
	:param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
	:type pluginName: string
	:param author: the name of the plugin's author
	:type author: string
	:param date: the date of creation of the plugin
	:type date: string
	:param info: some informations relative to the plugin
	:type info: string
	:param release: the version number of the plugin in the form X.Y
	:type release: string
	:param group: the name of the group in which the plugin will be inserted (a group corresponds to a sub-menu in the File -> Export menu from the Tulip Software GUI)
	:type group: string
	"""
  
	if testMode:
	  return
        pluginModule = getCallingModuleName()
        pluginModules[pluginName] = pluginModule
        code = "class " + pluginClassName + "Factory(tlp.ExportModuleFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ExportModuleFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + pluginName + "\"\n"
        code += "\tdef getAuthor(self):\n"
        code += "\t\t return \"" + author + "\"\n"
        code += "\tdef getGroup(self):\n"
        code += "\t\t return \"" + group + "\"\n"
        code += "\tdef getDate(self):\n"
        code += "\t\treturn \"" + date + "\"\n"
        code += "\tdef getInfo(self):\n"
        code += "\t\treturn \"" + info + "\"\n"
        code += "\tdef getRelease(self):\n"
        code += "\t\treturn \"" + release + "\"\n"
        code += "\tdef getTulipRelease(self):\n"
        code += "\t\treturn \"" + tlp.getTulipRelease() + "\"\n"
        code += "pluginFactory[pluginName] = " + pluginClassName + "Factory()\n"
        exec(code)
        updateTulipMenus()