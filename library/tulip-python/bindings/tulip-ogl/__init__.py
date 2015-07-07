# -*- coding: utf-8 -*-

import os
import sys
import platform

from tulip import *

_tulipOglNativeLibsPath = os.path.dirname(__file__) + '/native/'

sys.path.append(_tulipOglNativeLibsPath)

if platform.system() == 'Windows':
        os.environ['PATH'] = _tulipOglNativeLibsPath + ';' + os.environ['PATH']
elif platform.system() == 'Darwin':
        os.environ['DYLD_LIBRARY_PATH'] = _tulipOglNativeLibsPath + ':' + os.environ['DYLD_LIBRARY_PATH']

import _tulipogl

class tlpogl(_tulipogl.tlpogl):
    pass

tlp.loadTulipPluginsFromDir(_tulipOglNativeLibsPath + 'plugins')

__all__ = ['tlpogl']

