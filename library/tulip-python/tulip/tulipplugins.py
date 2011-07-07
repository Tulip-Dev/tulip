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