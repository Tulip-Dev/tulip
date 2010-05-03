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
  OpenGlConfigManager::OpenGlConfigManager():errorViewer(new OpenGlErrorViewer()),glewIsChecked(false),driversAreChecked(false),glewIsOk(false) {
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

#ifdef _UNIX
    bool nvidia=false;
    bool ati=false;
    string vendor(((const char*)glGetString(GL_VENDOR)));
    if(vendor.find("NVIDIA")!=string::npos)
      nvidia=true;
    if(vendor.find("ATI")!=string::npos)
      ati=true;

    if(!nvidia || !ati){
      errorViewer->displayErrorWithAskAgain("Video card or drivers warning","Warning :\n\nIt seems that your video card not powerful enough\nor your drivers are not properly configured to use\nevery capacity to Tulip\n\nIf you have an ATI or NVIDIA card, we recommend\nto install the official drivers");
    }
#endif
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
}
