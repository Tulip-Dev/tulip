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

%Module tulipqt 0

// Hack to avoid overriding the tlp Python class (containing all the module classes as
// nested classes and declared in each module : tulip, tulipogl, tulipqt) in the current 
// global namespace when importing the module the following way : from tulipqt import *		 
%PreInitialisationCode
    std::string tlpImportHook = 
     "try:\n"
	 "	_origimportfunc\n"	
	 "except NameError:\n"	
	 "	import __builtin__\n"
	 "	import sys\n"
	 "	_origimportfunc = __builtin__.__import__\n"
	  
	 "	def tlpimporthook(*args):\n"
     "		fullname = args[0]\n"
     "		m = _origimportfunc(*args)\n"
     "		if fullname == \"tulip\" or fullname == \"tulipogl\" or fullname == \"tulipqt\":\n"
	 "			if args[3] and args[3][0] == \"*\":\n"
	 "				curTlpDict = None\n"			
	 "				if \"tlp\" in args[2]:\n"
     "					curTlpDict = args[2][\"tlp\"].__dict__\n"
	 "		 		if curTlpDict:\n"
	 "					m = type(fullname, (object,), dict(m.__dict__))\n"
	 "					m.tlp = type(\"tlp\", (object,), dict(m.tlp.__dict__))\n"
	 "					for k in curTlpDict.keys():\n"
	 "						if not k.startswith(\"_\"):\n"
	 "							setattr(m.tlp, k, curTlpDict[k])\n"	 
     "		return m\n"
     
     "	__builtin__.__import__ = tlpimporthook\n"
     ;


    PyRun_SimpleString(tlpImportHook.c_str());
%End

namespace tlp;

%Import QtCore/QtCoremod.sip
%Import QtGui/QtGuimod.sip
%Import QtOpenGL/QtOpenGLmod.sip

%Include View.sip
%Include AbstractView.sip
%Include GlMainWidget.sip
%Include GlMainView.sip
%Include MouseInteractors.sip
%Include Interactor.sip
%Include InteractorComponent.sip
%Include InteractorChainOfResponsibility.sip
%Include InteractorManager.sip
%Include NodeLinkDiagramComponent.sip
%Include Controller.sip
%Include ControllerViewsManager.sip
%Include MainController.sip


%Import ../tulip/Module.sip
%Import ../tulip-ogl/Module.sip