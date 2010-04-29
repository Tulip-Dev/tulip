#include <GL/glew.h>

#include "tulip/OpenGlConfigManager.h"

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
