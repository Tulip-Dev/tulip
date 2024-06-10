/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%Module(name=_tulipgui, keyword_arguments="Optional", all_raise_py_exception = True, py_ssize_t_clean = True)

%PostInitialisationCode
// rename tlp namespace into tlpgui
PyObject *moduleDict = PyModule_GetDict(sipModule);
PyObject *tlpClass = PyDict_GetItemString(moduleDict, "tlp");
PyDict_DelItemString(moduleDict, "tlp");
PyDict_SetItemString(moduleDict, "tlpgui", tlpClass);
%End

%ModuleHeaderCode
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>
#include <tulip/Workspace.h>

extern void printErrorMessage(const std::string &);
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
        sipInstanceDestroyed(wrapper);
    }
}

%End


%Include Interactor.sip
%Include View.sip
%Include GlMainView.sip
%Include GlGraphRenderingParameters.sip
%Include GlGraphInputData.sip
%Include NodeLinkDiagramComponent.sip
%Include TulipGui.sip
%Include ColorScalesManager.sip


%Import ../tulip-core/tulip.sip
