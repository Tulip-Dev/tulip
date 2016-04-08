# -*- coding: utf-8 -*-

import os
import sys
import platform

from tulip import *

_tulipOglNativeLibsPath = os.path.dirname(__file__) + '/native/'

sys.path.append(_tulipOglNativeLibsPath)

if platform.system() == 'Windows':
  os.environ['PATH'] = _tulipOglNativeLibsPath + ';' + os.environ['PATH']
elif platform.system() == 'Linux' and os.path.exists(_tulipOglNativeLibsPath):
  # fix loading of Tulip plugins when the tulipogl module has been installed through pip
  import DLFCN
  sys.setdlopenflags(DLFCN.RTLD_NOW | DLFCN.RTLD_GLOBAL)

import _tulipogl

sys.path.pop()

class tlpogl(_tulipogl.tlpogl):
  pass

if not sys.argv[0] == 'tulip':
  # when the tulipogl module is installed from the Python Packaging Index
  # modify the TulipBitmapDir variable as it is different from the default one
  bitmapDir = os.path.dirname(__file__) + '/share/bitmaps/'
  if os.path.isdir(bitmapDir):
    tlp.TulipBitmapDir = bitmapDir
    tlp.TulipViewSettings.instance().setDefaultFontFile(tlp.TulipBitmapDir + 'font.ttf')

tlp.loadTulipPluginsFromDir(_tulipOglNativeLibsPath + 'plugins')

__all__ = ['tlpogl']
