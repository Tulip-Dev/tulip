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
#include <GL/glew.h>

#include "tulip/OpenGlConfigManager.h"

#include "tulip/OpenGlErrorViewer.h"

#include <iostream>

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::OpenGlConfigManager* tlp::OpenGlConfigManager::inst=0;
#endif
#else
tlp::OpenGlConfigManager* tlp::OpenGlConfigManager::inst=0;
#endif

using namespace std;

namespace tlp
{
OpenGlConfigManager::OpenGlConfigManager():errorViewer(new OpenGlErrorViewer()),glewIsChecked(false),driversAreChecked(false),glewIsOk(false),antialiased(true) {
}

OpenGlErrorViewer *OpenGlConfigManager::setErrorViewer(OpenGlErrorViewer *errorViewer){
	OpenGlErrorViewer *oldErrorViewer=this->errorViewer;
	this->errorViewer=errorViewer;
	return oldErrorViewer;
}

void OpenGlConfigManager::checkDrivers() {
	if(driversAreChecked)
		return;
	driversAreChecked=true;

	bool nvidia=false;
	bool ati=false;
	string vendor(((const char*)glGetString(GL_VENDOR)));
	if(vendor.find("NVIDIA")!=string::npos)
		nvidia=true;
	if(vendor.find("ATI")!=string::npos)
		ati=true;

	if(!nvidia && !ati){
		errorViewer->displayErrorWithAskAgain("Graphics card warning","Warning :\n\nYour graphics card is not powerful enough\nor it is not configured with the correct driver\nto suit the Tulip graphics rendering needs.\n\nIf you have an ATI or NVIDIA graphics card,\nwe recommend to install the official driver\nto benefit from an optimal graphics rendering.");
	}
}

void OpenGlConfigManager::initGlew() {
	if(glewIsChecked)
		return;

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		glewIsOk=false;
	}else{
		if(((void*)(glGenBuffers))!=NULL){
			glewIsOk=true;
		}else{
			glewIsOk=false;
		}
	}
	glewIsChecked=true;
}

void OpenGlConfigManager::activateLineAndPointAntiAliasing() {
	if (antialiased) {
		glDisable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	}
}

void OpenGlConfigManager::desactivateLineAndPointAntiAliasing() {
	if (antialiased) {
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
	}
}

void OpenGlConfigManager::activatePolygonAntiAliasing() {
	if (antialiased) {
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glEnable(GL_MULTISAMPLE);
	}
}

void OpenGlConfigManager::desactivatePolygonAntiAliasing() {
	if (antialiased) {
		glDisable(GL_MULTISAMPLE);
	}
}

}


