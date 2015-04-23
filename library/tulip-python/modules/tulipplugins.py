"""
This module provides utility functions to register Tulip plugins written in Python in the plugins database.
"""

import sys
import traceback
if sys.version_info[0] == 3:
    from imp import reload

from tulip import *
pluginFactory = {}
pluginModules = {}
testMode = False

def setTestMode(mode):
    global testMode
    testMode = mode

def getCallingModuleName():
    import sys
    if sys.version_info[0] == 3:
        f = list(sys._current_frames().values())[0]
    else:
        f = sys._current_frames().values()[0]
    while f.f_globals['__name__'] == "tulipplugins":
        f = f.f_back
    return f.f_globals['__name__']
   
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

def removePlugin(pluginName):
    tulipUtilsOk = True
    try:
        import tuliputils
    except ImportError:
        tulipUtilsOk = False
    if tulipUtilsOk:
        tuliputils.removePlugin(pluginName)

def setProcessQtEvents(processEvents):
    tulipUtilsOk = True
    try:
        import tuliputils
    except ImportError:
        tulipUtilsOk = False
    if tulipUtilsOk:
        tuliputils.setProcessQtEvents(processEvents)

def initFactory(self):
    tlp.FactoryInterface.__init__(self)
    self.registerPlugin()

def runPlugin(plugin):
    setProcessQtEvents(True)
    ret = False
    try:
        ret = plugin.real_run()
    except:
        if plugin.pluginProgress:
            plugin.pluginProgress.setError(traceback.format_exc())
        # Case where the plugin execution has not been launched through the Tulip GUI, so print the stack trace to stderr
        if type(plugin.pluginProgress) == tlp.SimplePluginProgress:
            sys.stdout.write("There was an error when running Python plugin named \"" + plugin.name() + "\". See stack trace below.\n")
            traceback.print_exc()
    setProcessQtEvents(False)
    return ret

def importGraph(plugin):
    ret = False
    setProcessQtEvents(True)
    try:
        ret = plugin.real_importGraph()
    except:
        if plugin.pluginProgress:
            plugin.pluginProgress.setError(traceback.format_exc())
        # Case where the plugin execution has not been launched through the Tulip GUI, so print the stack trace to stderr    
        if type(plugin.pluginProgress) == tlp.SimplePluginProgress:
            sys.stdout.write("There was an error when running Python plugin named \"" + plugin.name() + "\". See stack trace below.\n")
            traceback.print_exc()
    setProcessQtEvents(False)
    return ret

def exportGraph(plugin, os):
    ret = False
    setProcessQtEvents(True)
    try:
        ret = plugin.real_exportGraph(os)
    except:
        if plugin.pluginProgress:
            plugin.pluginProgress.setError(traceback.format_exc())
        # Case where the plugin execution has not been launched through the Tulip GUI, so print the stack trace to stderr    
        if type(plugin.pluginProgress) == tlp.SimplePluginProgress:
            sys.stdout.write("There was an error when running Python plugin named \"" + plugin.name() + "\". See stack trace below.\n")
            traceback.print_exc()
    setProcessQtEvents(False)
    return ret

def createPlugin(context, pluginModule, pluginClassName, pluginName, author, date, info, release, group):
    globals()[pluginModule] = __import__(pluginModule, globals(), locals(), [], 0)

    plugin = eval(pluginModule + "." + pluginClassName + "(context)", globals(), locals())
    if hasattr(plugin, "run"):
        plugin.real_run = plugin.run
        plugin.run = lambda : runPlugin(plugin)
    elif hasattr(plugin, "importGraph"):
        plugin.real_importGraph = plugin.importGraph
        plugin.importGraph = lambda : importGraph(plugin)
    elif hasattr(plugin, "exportGraph"):
        plugin.real_exportGraph = plugin.exportGraph
        plugin.exportGraph = lambda os : exportGraph(plugin, os)
    plugin.name = lambda : pluginName
    plugin.group = lambda : group
    plugin.date = lambda : date
    plugin.info = lambda : info
    plugin.release = lambda : release
    plugin.tulipRelease = lambda : tlp.getTulipRelease()
    plugin.programmingLanguage = lambda : "Python"
    return plugin

def registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    """
    tulipplugins.registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

    Registers a Tulip plugin written in Python in the plugins database and inserts it in a specific group.

    :param pluginClassName: the name of the Python class implementing the plugin
    :type pluginClassName: string
    :param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
    :type pluginName: string
    :param author: the name of the plugin's author
    :type author: string
    :param date: the date of creation of the plugin
    :type date: string
    :param info: some information relative to the plugin
    :type info: string
    :param release: the version number of the plugin in the form X.Y
    :type release: string
    :param group: the name of the group in which the plugin will be inserted
    :type group: string
    """

    global pluginFactory
    removePlugin(pluginName)
    if testMode:
        return
    pluginModule = getCallingModuleName()
    pluginModules[pluginName] = pluginModule
    pluginFactory[pluginName] = type(pluginClassName + "Factory", (tlp.FactoryInterface,), { \
                                     "__init__": (lambda self: initFactory(self)), \
                                     "createPluginObject" : (lambda self, context: createPlugin(context, pluginModule, pluginClassName, pluginName, author, date, info, release, group))} \
                                     )()

def registerPlugin(pluginClassName, pluginName, author, date, info, release):

    """
    tulipplugins.registerPlugin(pluginClassName, pluginName, author, date, info, release)

    Registers a Tulip plugin written in Python in the plugins database.

    :param pluginClassName: the name of the Python class implementing the plugin
    :type pluginClassName: string
    :param pluginName: the name of the plugin as it will appear in the interface and that will be used to call it
    :type pluginName: string
    :param author: the name of the plugin's author
    :type author: string
    :param date: the date of creation of the plugin
    :type date: string
    :param info: some information relative to the plugin
    :type info: string
    :param release: the version number of the plugin in the form X.Y
    :type release: string
    """

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

# For backward compatibility with Tulip 3.x

def registerAlgorithmPlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerAlgorithmPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

def registerLayoutPlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerLayoutPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

def registerDoublePlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerDoublePluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

def registerIntegerPlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerIntegerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

def registerBooleanPlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerBooleanPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

def registerSizePlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerSizePluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

def registerColorPlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerColorPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

def registerImportPlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerImportPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

def registerExportPlugin(pluginClassName, pluginName, author, date, info, release):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")

def registerExportPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group):

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)


