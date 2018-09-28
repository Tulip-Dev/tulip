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

# -*- coding: utf-8 -*-

import os
import os.path
import sys
import traceback
import platform
from datetime import date

_tulipNativeLibsPath = os.path.dirname(__file__) + '/native/'
sys.path.append(_tulipNativeLibsPath)

if platform.system() == 'Windows':
  os.environ['PATH'] = _tulipNativeLibsPath + ';' + _tulipNativeLibsPath + '../../../;' + os.environ['PATH']

import _tulip

sys.path.pop()

class tlpType(_tulip.tlp.__class__):

  def __getattr__(cls, name):
    if hasattr(_tulip.tlp, name):
      return _tulip.tlp.getTulipGlobalVar(name)
    else:
      raise AttributeError(name)

  def __setattr__(cls, name, value):
    if hasattr(_tulip.tlp, name):
      _tulip.tlp.setTulipGlobalVar(name, value)
    else:
      super(tlpType, cls).__setattr__(name, value)

# utility function from the 'six' module
def with_metaclass(meta, *bases):
  """Create a base class with a metaclass."""
  # This requires a bit of explanation: the basic idea is to make a dummy
  # metaclass for one level of class instantiation that replaces itself with
  # the actual metaclass.
  class metaclass(meta):
    def __new__(cls, name, this_bases, d):
      return meta(name, bases, d)
    
  return type.__new__(metaclass, 'temporary_class', (), {})

class tlp(with_metaclass(tlpType, _tulip.tlp)):
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

tlpPythonPluginsPath = tlp.TulipLibDir + '/tulip/python/tulip/plugins'
tlpPythonPluginsHomePath = os.path.expanduser('~') + '/.Tulip-' + tulipVersion + '/plugins/python'

tlp.loadTulipPluginsFromDir(tlpPythonPluginsPath, False)
tlp.loadTulipPluginsFromDir(tlpPythonPluginsHomePath, False)

_tulipNativePluginsPath = _tulipNativeLibsPath + 'plugins'

# fix loading of Tulip plugins when the tulip module has been installed through pip
if platform.system() == 'Linux' and os.path.exists(_tulipNativePluginsPath):
  dlOpenFlagsBackup = sys.getdlopenflags()
  if sys.version_info < (3, 6):
    import DLFCN
    dlOpenFlags = DLFCN.RTLD_NOW | DLFCN.RTLD_GLOBAL
  else:
    dlOpenFlags = os.RTLD_NOW | os.RTLD_GLOBAL
  sys.setdlopenflags(dlOpenFlags)

tlp.loadTulipPluginsFromDir(_tulipNativePluginsPath)

# load bundled Tulip Python plugins when the tulip module has been installed with pip
if not sys.argv[0] == 'tulip':
  tlp.loadTulipPluginsFromDir(os.path.dirname(__file__) + '/plugins/')

if platform.system() == 'Linux' and os.path.exists(_tulipNativePluginsPath):
  sys.setdlopenflags(dlOpenFlagsBackup)

__all__ = ['tlp']
__author__ = "David Auber and the Tulip development Team"
__license__ = "LGPLv3"
__version__ = tlp.getTulipRelease()
__email__ = "tulipdev@labri.fr"
__status__ = "Production"
