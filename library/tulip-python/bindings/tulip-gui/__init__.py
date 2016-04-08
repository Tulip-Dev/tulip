# -*- coding: utf-8 -*-

import os
import os.path
import sys
import platform

from tulip import *
from tulipogl import *

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

sys.path.append(_tulipGuiNativeLibsPath)

if platform.system() == 'Windows':
  os.environ['PATH'] = _tulipGuiNativeLibsPath + ';' + os.environ['PATH']
elif platform.system() == 'Linux' and os.path.exists(_tulipGuiNativeLibsPath):
  # fix loading of Tulip plugins when the tulipgui module has been installed through pip
  import DLFCN
  sys.setdlopenflags(DLFCN.RTLD_NOW | DLFCN.RTLD_GLOBAL)

import _tulipgui

sys.path.pop()

class tlpgui(_tulipgui.tlpgui):
  pass

def tulipguiExitFunc():
  import tulipgui
  tulipgui.tlpgui.runQtMainLoop()

tlp.loadTulipPluginsFromDir(_tulipGuiNativeLibsPath + 'plugins')

# Check if we are in script execution mode (sys.ps1 is not defined in that case)
# If so, register an exit callback that will run the Qt event loop if some widgets are
# still in a visible state
if not hasattr(sys, 'ps1') and not sys.flags.interactive:
  import atexit
  atexit.register(tulipguiExitFunc)

import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

if not sys.argv[0] == 'tulip':
  tlpgui.initInteractorsDependencies()

__all__ = ['tlpgui']
