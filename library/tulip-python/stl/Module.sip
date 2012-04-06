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

%Module stl

%ModuleHeaderCode
#include <string>
extern void printErrorMessage(const std::string &errMsg);

#if PY_MAJOR_VERSION >= 3
extern std::string convertPythonUnicodeObjectToStdString(PyObject *pyUnicodeObj);
#endif
%End

%ModuleCode
void printErrorMessage(const std::string &errMsg) {
    std::string pythonCode = "import sys\n"
                             "sys.stderr.write(\"";
    pythonCode += errMsg;
    pythonCode += "\\n\")";
    PyRun_SimpleString(pythonCode.c_str());
}

#if PY_MAJOR_VERSION >= 3
std::string convertPythonUnicodeObjectToStdString(PyObject *pyUnicodeObj) {
    std::string ret;
    Py_ssize_t dataSize = PyUnicode_GET_DATA_SIZE(pyUnicodeObj);
    const char* data = PyUnicode_AS_DATA(pyUnicodeObj);
    for (Py_ssize_t i = 0 ; i < dataSize ; ++i) {
        ret += std::string(data+i);
    }
    return ret;
}
#endif
%End

%Include list.sip
%Include pair.sip
%Include set.sip
%Include string.sip
%Include vector.sip
%Include map.sip
%Include ostream.sip

