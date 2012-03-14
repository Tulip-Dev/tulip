"""
This module provides utility functions to register Tulip plugins written in Python in the plugins database.
"""

import sys
if sys.version_info[0] == 3:
    from imp import reload
from tulip import *
from tulipqt import *

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

def removePlugin(pluginName, pluginType):
    tulipUtilsOk = True
    try:
        import tuliputils
    except ImportError:
        tulipUtilsOk = False
    if tulipUtilsOk:
        tuliputils.removePlugin(pluginName, pluginType)

def updateNodeLinkDiagramViewInteractors(newInteractorName, interactorType):
    m = tlp.MainController.getCurrentMainController()
    if not m:
        return
    for v in m.getViews():
        if m.getNameOfView(v).find("Node Link Diagram") != -1:
            interactors = v.getInteractors()
            for i in interactors:
                if str(type(i)) == str(interactorType):
                    interactors.remove(i)
            interactor = tlp.InteractorManager.getInst().getInteractor(newInteractorName)
            interactors.append(interactor)
            v.setInteractors(interactors)
            v.setActiveInteractor(interactors[0])

def registerInteractorPlugin(pluginClassName, pluginName, author, date, info, release):

    """
    tulipqtplugins.registerInteractorPlugin(pluginClassName, pluginName, author, date, info, release)

    """

    removePlugin(pluginName, "Interactor")
    if testMode:
        return
    pluginModule = getCallingModuleName()
    pluginModules[pluginName] = pluginModule
    code = "class " + pluginClassName + "Factory(tlp.InteractorFactory):\n"
    code += "\tdef __init__(self):\n"
    code += "\t\ttlp.InteractorFactory.__init__(self)\n"
    code += "\t\tself.registerPlugin()\n"
    code += "\tdef createPluginObject(self, context):\n"
    code += "\t\timport " + pluginModule + "\n"
    code += "\t\treturn " + pluginModule + "." + pluginClassName + "()\n"
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
    code += "tlp.InteractorManager.getInst().addInteractor(\""+pluginName+"\")\n"
    code += "from " + pluginModule + " import " + pluginClassName + "\n"
    code += "updateNodeLinkDiagramViewInteractors(\"" + pluginName + "\"," + pluginClassName + ")\n"
    exec(code)
