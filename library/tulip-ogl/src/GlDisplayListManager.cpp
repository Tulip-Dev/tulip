#include "tulip/GlDisplayListManager.h"

#include <iostream>

//====================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
tlp::GlDisplayListManager* tlp::GlDisplayListManager::inst=0;
#endif
#else
tlp::GlDisplayListManager* tlp::GlDisplayListManager::inst=0;
#endif

using namespace std;

namespace tlp
{
  //====================================================================
  void GlDisplayListManager::createInst() {
    if(!inst)
      inst = new GlDisplayListManager();
  }
  //====================================================================
  void GlDisplayListManager::changeContext(unsigned int context) {
    currentContext=context;
  }
  //====================================================================
  bool GlDisplayListManager::beginNewDisplayList(const string& name) {
    if(displayListMap[currentContext].find(name)!=displayListMap[currentContext].end()) {
      return false;
    }else{
      GLuint id;
      glNewList(id = glGenLists(1), GL_COMPILE);
      (displayListMap[currentContext])[name] = id;
      return true;
      }
  }
  //====================================================================
  void GlDisplayListManager::endNewDisplayList() {
    glEndList();
  }
  //====================================================================
  bool GlDisplayListManager::callDisplayList(const std::string& name) {
    map<string,GLuint>::iterator it=displayListMap[currentContext].find(name);
    if(it==displayListMap[currentContext].end()){
      assert(false);
    }
    assert(glIsList((*it).second));
    glCallList((*it).second);
    return true;
  }
} 
