# -*- coding: utf-8 -*-

import os
import sys
import traceback

_tulipNativeLibsPath = os.path.dirname(__file__) + '/native/'
sys.path.append(_tulipNativeLibsPath)

import _tulip

class tlp(_tulip.tlp):
  @staticmethod
  def loadTulipPythonPlugin(pluginFilePath):
    if not os.path.isfile(pluginFilePath) or not pluginFilePath.endswith('.py'):
      return False

    try:
      pluginFile = open(pluginFilePath)
      pluginFileContent = pluginFile.read()
    except:
      return False

    if not 'tulipplugins.register' in pluginFileContent:
      return False

    modulePath = os.path.dirname(pluginFilePath)
    moduleName = os.path.basename(pluginFilePath)[:-3]

    if not modulePath in sys.path:
      sys.path.append(modulePath)

    try:
      mod = __import__(moduleName)
    except ImportError:
      sys.stdout.write('There was an error when trying to load the Tulip Python plugin from the file ' + pluginFilePath + '. See stack trace below.\\n')
      traceback.print_exc()
      return False

    return True

  @staticmethod
  def loadTulipPluginsFromDir(pluginsDirPath, loadCppPlugin=True, pluginLoader=None):
    if not os.path.exists(pluginsDirPath):
      return False

    if loadCppPlugin:
      tlp.loadPluginsFromDir(pluginsDirPath, pluginLoader, False)

    files = os.listdir(pluginsDirPath)

    for file in files:
      filePath = pluginsDirPath+'/'+file
      if not os.path.isdir(filePath):
        tlp.loadTulipPythonPlugin(filePath)

    for file in files:
      filePath = pluginsDirPath+'/'+file
      if os.path.isdir(filePath):
        tlp.loadTulipPluginsFromDir(filePath, loadCppPlugin, pluginLoader)

    return True

tulipVersion = tlp.getTulipRelease()
tulipVersion = tulipVersion[:tulipVersion.rfind('.')]

startupScriptsPath = tlp.TulipLibDir + '/tulip/python/startup'
startupScriptsHomePath = os.path.expanduser('~') + '/.Tulip-' + tulipVersion + '/python/startup'

def runStartupScripts(scriptsPath):
  if not os.path.exists(scriptsPath):
    return

  files = os.listdir(scriptsPath)

  for file in files:
    filePath = scriptsPath+'/'+file
    if os.path.isfile(filePath) and filePath.endswith('.py'):
      exec(compile(open(filePath).read(), filePath, 'exec'), globals(), locals())

runStartupScripts(startupScriptsPath)
runStartupScripts(startupScriptsHomePath)

tlpPythonPluginsPath = tlp.TulipLibDir + '/tulip/python'
tlpPythonPluginsHomePath = os.path.expanduser('~') + '/.Tulip-' + tulipVersion + '/plugins/python'

tlp.loadTulipPluginsFromDir(tlpPythonPluginsPath, False)
tlp.loadTulipPluginsFromDir(tlpPythonPluginsHomePath, False)

__all__ = ['tlp']
