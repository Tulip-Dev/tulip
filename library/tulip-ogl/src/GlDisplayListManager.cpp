/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
#include <tulip/GlDisplayListManager.h>

#include <cassert>

//====================================================
tlp::GlDisplayListManager* tlp::GlDisplayListManager::inst=0;

using namespace std;

namespace tlp {
//====================================================================
void GlDisplayListManager::changeContext(uintptr_t context) {
  currentContext=context;
}
//====================================================================
void GlDisplayListManager::removeContext(uintptr_t context) {
  displayListMap.erase(context);
}
//====================================================================
bool GlDisplayListManager::beginNewDisplayList(const string& name) {
  if(displayListMap[currentContext].find(name)!=displayListMap[currentContext].end()) {
    return false;
  }
  else {
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

  if(it==displayListMap[currentContext].end()) {
    assert(false);
  }

  assert(glIsList(it->second));
  glCallList(it->second);
  return true;
}
}
