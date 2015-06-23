# -*- coding: utf-8 -*-

import os
import sys

from tulip import *

_tulipOglNativeLibsPath = os.path.dirname(__file__) + '/native/'

sys.path.append(_tulipOglNativeLibsPath)

import _tulipogl

class tlpogl(_tulipogl.tlpogl):
    pass

__all__ = ['tlpogl']

