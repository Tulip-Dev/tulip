from tulip import *
pluginFactory = {}
pluginModules = {}

def getCallingModuleName():
   import sys
   f = sys._current_frames().values()[0]
   f = f.f_back
   return f.f_back.f_globals['__name__']
   
def reloadTulipPythonPlugin(module):
   code = ""
   code += "import " + module + "\n"
   code += "reload(" + module + ")\n"
   exec(code)
   
def reloadTulipPythonPlugins():
   code = ""
   for module in pluginModules.keys():
      reloadTulipPythonPlugin(module)
      
def registerAlgorithmPlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.AlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.AlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerAlgorithmPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.AlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.AlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerLayoutPlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.LayoutAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.LayoutAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerLayoutPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.LayoutAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.LayoutAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerDoublePlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.DoubleAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.DoubleAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerDoublePluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.DoubleAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.DoubleAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerIntegerPlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.IntegerAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.IntegerAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerIntegerPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.IntegerAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.IntegerAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerBooleanPlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.BooleanAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.BooleanAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerBooleanPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.BooleanAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.BooleanAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerSizePlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.SizeAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.SizeAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerSizePluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.SizeAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.SizeAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerColorPlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.ColorAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ColorAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerColorPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.ColorAlgorithmFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ColorAlgorithmFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerImportPlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.ImportModuleFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ImportModuleFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerImportPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.ImportModuleFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ImportModuleFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerExportPlugin(pluginClassName, algoName, author, date, info, release):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.ExportModuleFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ExportModuleFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)
        
def registerExportPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):
        if algoName in pluginFactory.keys():
                return
        pluginModule = getCallingModuleName()
        pluginModules[pluginModule] = 1
        code = "class " + pluginClassName + "Factory(tlp.ExportModuleFactory):\n"
        code += "\tdef __init__(self):\n"
        code += "\t\ttlp.ExportModuleFactory.__init__(self)\n"
        code += "\t\tself.registerPlugin()\n"
        code += "\tdef createPluginObject(self, context):\n"
        code += "\t\timport " + pluginModule + "\n"
        code += "\t\treturn " + pluginModule + "." + pluginClassName + "(context)\n"
        code += "\tdef getName(self):\n"
        code += "\t\t return \"" + algoName + "\"\n"
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
        code += "pluginFactory[algoName] = " + pluginClassName + "Factory()\n"
        exec(code)