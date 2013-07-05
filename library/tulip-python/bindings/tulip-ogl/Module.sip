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

%Module tulipogl

%ModuleHeaderCode
#include <tulip/GlSceneObserver.h>
#include <tulip/Camera.h>
#include <string>
extern void printErrorMessage(const std::string &errMsg);

typedef tlp::Iterator<tlp::Observable*> IteratorObservable;
%End

typedef tlp::Iterator<tlp::Observable*> IteratorObservable;

%ModuleCode
void printErrorMessage(const std::string &errMsg) {
  std::string pythonCode = "import sys\n"
                                 "sys.stderr.write(\"";
  pythonCode += errMsg;
  pythonCode += "\\n\")";
  PyRun_SimpleString(pythonCode.c_str());
}


%End

%PostInitialisationCode
    // rename tlp namespace into tlpogl
    PyObject *moduleDict = PyModule_GetDict(sipModule);
    PyObject *tlpClass = PyDict_GetItemString(moduleDict, "tlp");
    PyDict_DelItemString(moduleDict, "tlp");
    PyDict_SetItemString(moduleDict, "tlpogl", tlpClass);
%End

//%Include GlSimpleEntity.sip
//%Include GlSceneVisitor.sip
//%Include GlComposite.sip
//%Include GlGraphComposite.sip
//%Include GlScene.sip
//%Include Camera.sip
//%Include GlLayer.sip
%Include GlGraphRenderingParameters.sip
%Include GlGraphInputData.sip
//%Include Glyph.sip
//%Include GlTools.sip
//%Include GlSceneObserver.sip
//%Include GlAbstractPolygon.sip
//%Include GlPolygon.sip
//%Include GlRegularPolygon.sip
//%Include GlComplexPolygon.sip
//%Include GlCircle.sip
//%Include GlRect.sip
%Include ParametricCurves.sip

%Import ../tulip-core/Module.sip
