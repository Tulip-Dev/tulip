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
import platform
from datetime import date

from tulip import tlp

# fix an ubuntu bug when using the nvidia driver,
# when the python interpreter dynamically loads the GL library,
# it ends up loading the mesa one instead of the nvidia one.
# The use of the tulipgui module through the classical python intrepreter
# was quite unusable because of that ...
# Apply the workaround proposed here : https://bugs.launchpad.net/ubuntu/+source/python-qt4/+bug/941826
if hasattr(os, 'uname'):
  if os.uname()[3].lower().find('ubuntu') != -1:
    import ctypes.util
    ctypes.CDLL(ctypes.util.find_library('GL'), ctypes.RTLD_GLOBAL)

_tulipGuiNativeLibsPath = os.path.dirname(__file__) + '/native/'
_tulipGuiNativePluginsPath = _tulipGuiNativeLibsPath + 'plugins'
_tulipGuiPluginsPath = os.path.dirname(__file__) + '/plugins/'

sys.path.append(_tulipGuiNativeLibsPath)

if platform.system() == 'Windows':
  os.environ['PATH'] = _tulipGuiNativeLibsPath + ';' + _tulipGuiNativeLibsPath + '../../../;' + os.environ['PATH']

if not sys.argv[0] == 'tulip':
  # when the tulipgui module is installed from the Python Packaging Index
  # modify the TulipBitmapDir variable as it is different from the default one
  bitmapDir = os.path.dirname(__file__) + '/share/bitmaps/'
  if os.path.isdir(bitmapDir):
    tlp.TulipBitmapDir = bitmapDir
    tlp.TulipPluginsPath = _tulipGuiNativePluginsPath
    tlp.TulipViewSettings.instance().setDefaultFontFile(tlp.TulipBitmapDir + 'font.ttf')

import _tulipgui

sys.path.pop()

class tlpgui(_tulipgui.tlpgui):
  pass

def tulipguiExitFunc():
  import tulipgui
  tulipgui.tlpgui.runQtMainLoop()

# fix loading of Tulip plugins when the tulipgui module has been installed through pip
if platform.system() == 'Linux' and os.path.exists(_tulipGuiNativePluginsPath):
  dlOpenFlagsBackup = sys.getdlopenflags()
  if sys.version_info < (3, 6):
    import DLFCN
    dlOpenFlags = DLFCN.RTLD_NOW | DLFCN.RTLD_GLOBAL
  else:
    dlOpenFlags = os.RTLD_NOW | os.RTLD_GLOBAL
  sys.setdlopenflags(dlOpenFlags)

tlp.loadTulipPluginsFromDir(_tulipGuiPluginsPath)

if platform.system() == 'Linux' and os.path.exists(_tulipGuiNativePluginsPath):
  sys.setdlopenflags(dlOpenFlagsBackup)

# Check if we are in script execution mode (sys.ps1 is not defined in that case)
# If so, register an exit callback that will run the Qt event loop if some widgets are
# still in a visible state
if not hasattr(sys, 'ps1') and not sys.flags.interactive:
  import atexit
  atexit.register(tulipguiExitFunc)

import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

__all__ = ['tlpgui']
__author__ = "David Auber, Antoine Lambert and the Tulip development Team"
__copyright__ = "Copyright " + str(date.today().year) + ", LaBRI and University of Bordeaux"
__license__ = "LGPL-3.0"
__version__ = tlp.getTulipRelease()
__maintainer__ = "Antoine Lambert"
__email__ = "antoine.lambert33@gmail.com"
__status__ = "Production"
