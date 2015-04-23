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

%Module stl

%ModuleHeaderCode
#include <string>

extern void printErrorMessage(const std::string &errMsg);

extern std::string convertPythonUnicodeObjectToStdString(PyObject *pyUnicodeObj);

%End

%ModuleCode
#include <string>
void printErrorMessage(const std::string &errMsg) {
    std::string pythonCode = "import sys\n"
                             "sys.stderr.write(\"";
    pythonCode += errMsg;
    pythonCode += "\\n\")";
    PyRun_SimpleString(pythonCode.c_str());
}

std::string convertPythonUnicodeObjectToStdString(PyObject *pyUnicodeObj) {
    PyObject *utf8Str = PyUnicode_AsUTF8String(pyUnicodeObj);
    std::string ret(PyBytes_AsString(utf8Str));
    Py_DECREF(utf8Str);
    return ret;
}
%End

%ExportedHeaderCode
#define getSipType(sipTypename) (sipResolveTypedef(sipTypename) ? sipFindType(sipResolveTypedef(sipTypename)) : sipFindType(sipTypename))
%End

%Include std_list.sip
%Include std_pair.sip
%Include std_set.sip
%Include std_string.sip
%Include std_vector.sip
%Include std_map.sip
%Include std_ostream.sip

typedef std::vector<std::string> vectorStringType;
typedef std::list<std::string> listStringType;
typedef std::set<std::string> setStringType;

