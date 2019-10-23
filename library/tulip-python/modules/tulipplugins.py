# This file is part of Tulip (http://tulip.labri.fr)
#
# Authors: David Auber and the Tulip development Team
# from LaBRI, University of Bordeaux
#
# Tulip is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3
# of the License, or (at your option) any later version.
#
# Tulip is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.

"""
This module provides utility functions to register Tulip plugins written
in Python in the plugins database.
"""

import sys
import traceback
if sys.version_info[0] == 3:
    from imp import reload # noqa

from tulip import tlp # noqa

pluginFactory = {}
pluginModules = {}
testMode = False


def setTestMode(mode):
    global testMode
    testMode = mode


def getCallingModuleName():
    import sys
    if sys.version_info[0] == 3:
        frames = list(sys._current_frames().values())
    else:
        frames = sys._current_frames().values()
    for i in range(len(frames)):
        f = frames[i]
        if f.f_globals['__name__'] == 'tulipplugins':
            f = f.f_back
            break
    while f.f_globals['__name__'] == 'tulipplugins':
        f = f.f_back
    return f.f_globals['__name__']


def reloadTulipPythonPlugin(pluginName):
    if pluginName in pluginModules:
        module = pluginModules[pluginName]
        code = 'import %s\n' % module
        code += 'reload(%s)\n' % module
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


def initFactory(self):
    tlp.PluginFactory.__init__(self)
    self.registerFactory()


def runPlugin(plugin):
    ret = False
    try:
        ret = plugin.real_run()
    except Exception:
        if plugin.pluginProgress:
            plugin.pluginProgress.setError(traceback.format_exc())
        # Case where the plugin execution has not been launched through the
        # Tulip GUI, so print the stack trace to stderr
        if type(plugin.pluginProgress) == tlp.SimplePluginProgress:
            sys.stdout.write(('There was an error when running Python plugin '
                              'named "%s". See stack trace below.\n') %
                             plugin.name())
            traceback.print_exc()
    return ret


def importGraph(plugin):
    ret = False
    try:
        ret = plugin.real_importGraph()
    except Exception:
        if plugin.pluginProgress:
            plugin.pluginProgress.setError(traceback.format_exc())
        # Case where the plugin execution has not been launched through
        # the Tulip GUI, so print the stack trace to stderr
        if type(plugin.pluginProgress) == tlp.SimplePluginProgress:
            sys.stdout.write(('There was an error when running Python plugin '
                              'named "%s". See stack trace below.\n') %
                             plugin.name())
            traceback.print_exc()
    return ret


def exportGraph(plugin, os):
    ret = False
    try:
        ret = plugin.real_exportGraph(os)
    except Exception:
        if plugin.pluginProgress:
            plugin.pluginProgress.setError(traceback.format_exc())
        # Case where the plugin execution has not been launched through
        # the Tulip GUI, so print the stack trace to stderr
        if type(plugin.pluginProgress) == tlp.SimplePluginProgress:
            sys.stdout.write(('There was an error when running Python plugin '
                              'named "%s". See stack trace below.\n') %
                             plugin.name())
            traceback.print_exc()
    return ret


def createPlugin(context, pluginModule, pluginClassName, pluginName,
                 author, date, info, release, group):
    plugin = eval('%s.%s(context)' % (pluginModule, pluginClassName),
                  globals(), locals())
    if hasattr(plugin, 'run'):
        plugin.real_run = plugin.run
        plugin.run = lambda: runPlugin(plugin)
    elif hasattr(plugin, 'importGraph'):
        plugin.real_importGraph = plugin.importGraph
        plugin.importGraph = lambda: importGraph(plugin)
    elif hasattr(plugin, 'exportGraph'):
        plugin.real_exportGraph = plugin.exportGraph
        plugin.exportGraph = lambda os: exportGraph(plugin, os)
    plugin.name = lambda: pluginName
    plugin.group = lambda: group
    plugin.author = lambda: author
    plugin.date = lambda: date
    plugin.info = lambda: info
    plugin.release = lambda: release
    plugin.tulipRelease = lambda: tlp.getTulipRelease()
    plugin.programmingLanguage = lambda: 'Python'
    return plugin


def registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group):
    """
    tulipplugins.registerPluginOfGroup(pluginClassName, pluginName, author, date, info, release, group)

    Registers a Tulip plugin written in Python in the plugins database and
    inserts it in a specific group.

    :param pluginClassName: the name of the Python class implementing the plugin
    :type pluginClassName: string
    :param pluginName: the name of the plugin as it will appear in the interface
        and that will be used to call it
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
    """ # noqa

    global pluginFactory
    removePlugin(pluginName)
    pluginModule = getCallingModuleName()
    try:
        globals()[pluginModule] = __import__(pluginModule, globals(), locals(),
                                             [], 0)
        eval('%s.%s' % (pluginModule, pluginClassName), globals(), locals())
        if testMode:
            return
        pluginModules[pluginName] = pluginModule
        pluginFactory[pluginName] = type(
            '%sFactory' % pluginClassName, (tlp.PluginFactory,),
            {
                '__init__': (lambda self: initFactory(self)),
                'createPluginObject': (lambda self, context: createPlugin(
                    context, pluginModule, pluginClassName, pluginName, author,
                    date, info, release, group))
            }
        )()

    except Exception:
        sys.stdout.write(('There was an error when registering Python plugin '
                          'named "%s". See stack trace below.\n') %
                         pluginName)
        raise


def registerPlugin(pluginClassName, pluginName, author, date, info, release):
    """
    tulipplugins.registerPlugin(pluginClassName, pluginName, author, date, info, release)

    Registers a Tulip plugin written in Python in the plugins database.

    :param pluginClassName: the name of the Python class implementing
        the plugin
    :type pluginClassName: string
    :param pluginName: the name of the plugin as it will appear in the
        interface and that will be used to call it
    :type pluginName: string
    :param author: the name of the plugin's author
    :type author: string
    :param date: the date of creation of the plugin
    :type date: string
    :param info: some information relative to the plugin
    :type info: string
    :param release: the version number of the plugin in the form X.Y
    :type release: string
    """ # noqa

    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


# For backward compatibility with Tulip 3.x
def registerAlgorithmPlugin(pluginClassName, pluginName, author, date, info,
                            release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerAlgorithmPluginOfGroup(pluginClassName, pluginName, author, date,
                                   info, release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)


def registerLayoutPlugin(pluginClassName, pluginName, author, date, info,
                         release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerLayoutPluginOfGroup(pluginClassName, pluginName, author, date,
                                info, release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)


def registerDoublePlugin(pluginClassName, pluginName, author, date, info,
                         release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerDoublePluginOfGroup(pluginClassName, pluginName, author, date,
                                info, release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)


def registerIntegerPlugin(pluginClassName, pluginName, author, date, info,
                          release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerIntegerPluginOfGroup(pluginClassName, pluginName, author, date,
                                 info, release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)


def registerBooleanPlugin(pluginClassName, pluginName, author, date, info,
                          release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerBooleanPluginOfGroup(pluginClassName, pluginName, author, date,
                                 info, release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)


def registerSizePlugin(pluginClassName, pluginName, author, date, info,
                       release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerSizePluginOfGroup(pluginClassName, pluginName, author, date, info,
                              release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)


def registerColorPlugin(pluginClassName, pluginName, author, date, info,
                        release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerColorPluginOfGroup(pluginClassName, pluginName, author, date, info,
                               release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)


def registerImportPlugin(pluginClassName, pluginName, author, date, info,
                         release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerImportPluginOfGroup(pluginClassName, pluginName, author, date,
                                info, release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)


def registerExportPlugin(pluginClassName, pluginName, author, date, info,
                         release):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, '')


def registerExportPluginOfGroup(pluginClassName, pluginName, author, date,
                                info, release, group):
    registerPluginOfGroup(pluginClassName, pluginName, author, date, info,
                          release, group)
