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

%MappedType std::string
{
%TypeHeaderCode
// Include the library interface to the type being mapped.
#include <string>
%End

%ConvertToTypeCode
	if (sipIsErr == NULL) {
#if PY_MAJOR_VERSION >= 3
        return PyUnicode_Check(sipPy);
#else
        return PyString_Check(sipPy) || PyUnicode_Check(sipPy);
#endif
	}

	if (sipPy == Py_None) {
		*sipCppPtr = new std::string;
		return 1;
	}
#if PY_MAJOR_VERSION >= 3
    *sipCppPtr = new std::string(convertPythonUnicodeObjectToStdString(sipPy));
#else
    if (PyString_Check(sipPy)) {
        *sipCppPtr = new std::string(PyString_AS_STRING(sipPy));
    } else {
        *sipCppPtr = new std::string(convertPythonUnicodeObjectToStdString(sipPy));
    }
#endif
	return 1;
%End

%ConvertFromTypeCode
#if PY_MAJOR_VERSION >= 3
    PyObject *obj = PyUnicode_FromString(sipCpp->c_str());
#else
    PyObject *obj = PyString_FromString(sipCpp->c_str());
#endif
	return obj;
%End
};
