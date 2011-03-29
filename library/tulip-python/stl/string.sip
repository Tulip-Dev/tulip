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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

%MappedType std::string
{
%TypeHeaderCode
	// Include the library interface to the type being mapped.
#include <string>
%End

%ConvertToTypeCode
	if (sipIsErr == NULL) {
		return PyString_Check(sipPy);
	}

	if (sipPy == Py_None) {
		*sipCppPtr = new std::string;
		return 1;
	}

	*sipCppPtr = new std::string(PyString_AS_STRING(sipPy));
	return 1;
%End

%ConvertFromTypeCode
	PyObject *obj = PyString_FromString(sipCpp->c_str());

	return obj;
%End
};