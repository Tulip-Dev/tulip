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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/GlComposite.h>

using namespace std;
using namespace tlp;

//============================================================
GlComposite::GlComposite(bool deleteEntitiesInDestructor):_deleteEntitiesInDestructor(deleteEntitiesInDestructor) {}
//============================================================
GlComposite::~GlComposite() {
  reset(_deleteEntitiesInDestructor);
}
//============================================================
void GlComposite::reset(bool deleteElems) {
  for(const pair<string, GlEntity *> &entity : _entities) {
    entity.second->setParent(nullptr);
    if (deleteElems) {
      delete entity.second;
    }
  }
  _entities.clear();
  notifyModified();
}
//============================================================
void GlComposite::addGlEntity(GlEntity *entity, const string &key) {
  _entities[key] = entity;
  entity->setVisible(_visible);
  entity->setParent(this);
  notifyModified();
}
//============================================================
void GlComposite::deleteGlEntity(const string &key) {
  if(_entities.count(key)==0)
    return;
  _entities[key]->setParent(nullptr);
  _entities.erase(key);
  notifyModified();
}
//============================================================
void GlComposite::deleteGlEntity(GlEntity *entity) {
  std::map<string, GlEntity *>::const_iterator it;
  for(it = _entities.begin(); it != _entities.end(); ++it) {
    if(entity == (*it).second) {
      entity->setParent(nullptr);
      _entities.erase(it->first);
      notifyModified();
      return;
    }
  }
}
//============================================================
string GlComposite::findKey(GlEntity *entity) {
  for(const pair<string, GlEntity *> &ent : _entities) {
    if(entity == ent.second) {
      return ent.first;
    }
  }
  return string();
}
//============================================================
GlEntity* GlComposite::findGlEntity(const string &key) {
  std::map<string, GlEntity *>::const_iterator ite = _entities.find(key);
  if (ite == _entities.end())
    return nullptr;
  return (*ite).second;
}
//============================================================
void GlComposite::setStencil(int stencil) {
  GlEntity::setStencil(stencil);
  for(const pair<string, GlEntity *> &entity : _entities) {
    entity.second->setStencil(stencil);
  }
  notifyModified();
}
//============================================================
void GlComposite::setVisible(bool visible) {
  GlEntity::setVisible(visible);
  for(const pair<string, GlEntity *> &entity : _entities) {
    entity.second->setVisible(visible);
  }
  notifyModified();
}
//============================================================
void GlComposite::acceptVisitor(GlSceneVisitor *visitor) {
  for(const pair<string, GlEntity *> &entity : _entities) {
    if(entity.second->isVisible()) {
      entity.second->acceptVisitor(visitor);
    }
  }
}
