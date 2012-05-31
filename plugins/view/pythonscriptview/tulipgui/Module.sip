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

%Module tulipgui

// Hack to avoid overriding the tlp Python class (containing all the module classes as
// nested classes and declared in each module : tulip, tulipogl, tulipqt) in the current 
// global namespace when importing the module the following way : from tulipqt import *		 
%PreInitialisationCode
    std::string tlpImportHook =
    "try:\n"
    "   _origimportfunc\n"
    "except NameError:\n"
    "   import sys\n"
#if PY_MAJOR_VERSION >= 3
    "   import builtins\n"
    "   _origimportfunc = builtins.__import__\n"
#else
    "   import __builtin__\n"
    "   _origimportfunc = __builtin__.__import__\n"
#endif
    "   def tlpimporthook(name, globals={}, locals={}, fromlist=[], level=-1):\n"
    "       fullname = name\n"
    "       m = _origimportfunc(name, globals, locals, fromlist, level)\n"
    "       if fullname.startswith(\"tulip\"):\n"
    "           if fromlist and len(fromlist) > 0 and fromlist[0] == \"*\":\n"
    "               curTlpDict = None\n"
    "               if \"tlp\" in locals:\n"
    "                   curTlpDict = locals[\"tlp\"].__dict__\n"
    "                   if curTlpDict:\n"
    "                       m = type(fullname, (object,), dict(m.__dict__))\n"
    "                       m.tlp = type(\"tlp\", (object,), dict(m.tlp.__dict__))\n"
    "                       for k in curTlpDict.keys():\n"
    "                           if not k.startswith(\"_\"):\n"
    "                               setattr(m.tlp, k, curTlpDict[k])\n"
    "       return m\n"
#if PY_MAJOR_VERSION >= 3
    "   builtins.__import__ = tlpimporthook\n"
#else
    "   __builtin__.__import__ = tlpimporthook\n"
#endif
     ;

    PyRun_SimpleString(tlpImportHook.c_str());
%End

%ModuleHeaderCode
#include <string>
extern void printErrorMessage(const std::string &errMsg);
extern void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td);
%End

%ModuleCode
void printErrorMessage(const std::string &errMsg) {
    std::string pythonCode = "import sys\n"
                             "sys.stderr.write(\"";
    pythonCode += errMsg;
    pythonCode += "\\n\")";
    PyRun_SimpleString(pythonCode.c_str());
}

void releaseSIPWrapper(void *wrappedCppObject, const sipTypeDef *td) {
    PyObject *pyObj = sipGetPyObject(wrappedCppObject, td);
    if (pyObj) {
        sipTransferTo(pyObj, pyObj);
        sipSimpleWrapper *wrapper = reinterpret_cast<sipSimpleWrapper *>(pyObj);
        sipCommonDtor(wrapper);
    }
}
%End

namespace tlp;

%Include View.sip
%Include NodeLinkDiagramComponent.sip
%Include TulipGui.sip

%Import tulip-ogl/Module.sip
