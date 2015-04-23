/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

namespace std {

class ostream /NoDefaultCtors/ {
%TypeHeaderCode
#include <iostream>
%End

public:

	std::ostream& operator<< (SIP_PYOBJECT val) /NoCopy/;
%MethodCode
	PyObject *objStr = PyObject_Str(a1);
	PyObject *objRepr = PyObject_Repr(a1);
	if (objStr) {
#if PY_MAJOR_VERSION >= 3
        sipRes = &(*a0 << convertPythonUnicodeObjectToStdString(objStr));
#else
        sipRes = &(*a0 << PyString_AS_STRING(objStr));
#endif
	} else if (objRepr) {
#if PY_MAJOR_VERSION >= 3
        sipRes = &(*a0 << convertPythonUnicodeObjectToStdString(objRepr));
#else
        sipRes = &(*a0 << PyString_AS_STRING(objRepr));
#endif
	} else {
		sipRes = a0;
	}
	
	Py_XDECREF(objStr);
	Py_XDECREF(objRepr);
%End	

};

};
