# This file is part of Tulip (www.tulip-software.org)
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
import sys
import platform
from datetime import date

from tulip import *

_tulipOglNativeLibsPath = os.path.dirname(__file__) + '/native/'

sys.path.append(_tulipOglNativeLibsPath)

if platform.system() == 'Windows':
  os.environ['PATH'] = _tulipOglNativeLibsPath + ';' + os.environ['PATH']

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

_tulipOglNativePluginsPath = _tulipOglNativeLibsPath + 'plugins'

# fix loading of Tulip plugins when the tulipogl module has been installed through pip
if platform.system() == 'Linux' and os.path.exists(_tulipOglNativePluginsPath):
  import DLFCN
  dlOpenFlagsBackup = sys.getdlopenflags()
  sys.setdlopenflags(DLFCN.RTLD_NOW | DLFCN.RTLD_GLOBAL)

tlp.loadTulipPluginsFromDir(_tulipOglNativePluginsPath)

if platform.system() == 'Linux' and os.path.exists(_tulipOglNativePluginsPath):
  sys.setdlopenflags(dlOpenFlagsBackup)

__all__ = ['tlpogl']
__author__ = "David Auber, Antoine Lambert and the Tulip development Team"
__copyright__ = "Copyright " + str(date.today().year) + ", LaBRI and University of Bordeaux"
__license__ = "LGPL-3.0"
__version__ = tlp.getTulipRelease()
__maintainer__ = "Antoine Lambert"
__email__ = "antoine.lambert33@gmail.com"
__status__ = "Production"
