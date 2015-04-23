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
#include <tulip/GlSimpleEntity.h>

#include <tulip/GlComposite.h>

using namespace std;

namespace tlp {
GlSimpleEntity::~GlSimpleEntity() {
  for(std::vector<GlComposite*>::iterator it=parents.begin(); it!=parents.end(); ++it) {
    (*it)->deleteGlEntity(this,false);
  }
}

void GlSimpleEntity::addParent(GlComposite *composite) {
  parents.push_back(composite);
}

void GlSimpleEntity::setVisible(bool visible) {
  if(this->visible==visible)
    return;

  this->visible=visible;

  for(vector<GlComposite*>::iterator it=parents.begin(); it!=parents.end(); ++it) {
    (*it)->notifyModified(this);
  }
}
void GlSimpleEntity::removeParent(GlComposite *composite) {
  for(std::vector<GlComposite*>::iterator it=parents.begin(); it!=parents.end(); ++it) {
    if((*it)==composite) {
      parents.erase(it);
      return;
    }
  }
}
}
