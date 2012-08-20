"""
This module provides utility functions to register Tulip plugins written in Python in the plugins database.
"""

import sys
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

def updateTulipMenus():
    tulipUtilsOk = True
    try:
        import tuliputils
    except ImportError:
        tulipUtilsOk = False
    if tulipUtilsOk:
        tuliputils.updatePluginsMenus()

def initFactory(self):
    tlp.FactoryInterface.__init__(self)
    self.registerPlugin()

def createPlugin(context, pluginModule, pluginClassName, pluginName, author, date, info, release, group):
    globals()[pluginModule] = __import__(pluginModule, globals(), locals(), [], -1)
    plugin = eval(pluginModule + "." + pluginClassName + "(context)", globals(), locals())
    plugin.name = lambda : pluginName
    plugin.group = lambda : group
    plugin.date = lambda : date
    plugin.info = lambda : info
    plugin.release = lambda : release
    plugin.tulipRelease = lambda : tlp.getTulipRelease()
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
    :param info: some informations relative to the plugin
    :type info: string
    :param release: the version number of the plugin in the form X.Y
    :type release: string
    :param group: the name of the group in which the plugin will be inserted
    :type group: string
    """

    global pluginFactory
    removePlugin(pluginName)
    updateTulipMenus()
    if testMode:
        return
    pluginModule = getCallingModuleName()
    pluginModules[pluginName] = pluginModule
    pluginFactory[pluginName] = type(pluginClassName + "Factory", (tlp.FactoryInterface,), { \
                                     "__init__": (lambda self: initFactory(self)), \
                                     "createPluginObject" : (lambda self, context: createPlugin(context, pluginModule, pluginClassName, pluginName, author, date, info, release, group))} \
                                     )()
    updateTulipMenus()

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
    :param info: some informations relative to the plugin
    :type info: string
    :param release: the version number of the plugin in the form X.Y
    :type release: string
    """

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, "")


