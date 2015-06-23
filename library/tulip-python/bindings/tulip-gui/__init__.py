# -*- coding: utf-8 -*-

import os
import sys

from tulip import *
from tulipogl import *

_tulipGuiNativeLibsPath = os.path.dirname(__file__) + '/native/'

sys.path.append(_tulipGuiNativeLibsPath)

import _tulipgui

class tlpgui(_tulipgui.tlpgui):
    pass

__all__ = ['tlpgui']

