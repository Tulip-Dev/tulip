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
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/GlLayer.h>
#include <tulip/Camera.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/GlScene.h>

using namespace std;
using namespace tlp;

GlLayer::GlLayer(const std::string &name, bool is3d)
    : _name(name), _scene(0), _camera(new Camera(is3d)), _light(new Light(_camera)), _sharedCamera(false), _sharedLight(false), _visible(true) {
}

GlLayer::GlLayer(const std::string &name, Camera *camera, Light *light)
    : _name(name), _scene(0), _camera(camera), _light(light), _sharedCamera(true), _sharedLight(true), _visible(true) {
}

GlLayer::~GlLayer() {
  if (!_sharedCamera)
    delete _camera;
  if (!_sharedLight)
    delete _light;
}

void GlLayer::setScene(GlScene *scene) {
  if (_scene) {
    removeListener(_scene);
    _camera->removeListener(_scene);
    _composite.removeListener(_scene);
    std::map<std::string, GlEntity *>::const_iterator it = getGlEntities().begin();
    for (; it != getGlEntities().end(); ++it) {
      it->second->removeListener(_scene);
    }
  }
  if (scene) {
    _scene = scene;
    addListener(_scene);
    _camera->addListener(_scene);
    _composite.addListener(_scene);
    std::map<std::string, GlEntity *>::const_iterator it = getGlEntities().begin();
    for (; it != getGlEntities().end(); ++it) {
      it->second->addListener(_scene);
    }
  }
}

void GlLayer::setCamera(Camera *camera) {
  Camera *oldCamera = _camera;
  _camera = camera;

  if (!_sharedCamera)
    delete oldCamera;

  _sharedCamera = false;
  setScene(_scene);
}

void GlLayer::setSharedCamera(Camera *camera) {
  Camera *oldCamera = _camera;
  _camera = camera;

  if (!_sharedCamera)
    delete oldCamera;

  _sharedCamera = true;
  setScene(_scene);
}

void GlLayer::setLight(Light *light) {
  Light *oldLight = _light;
  _light = light;

  if (!_sharedLight)
    delete oldLight;

  _sharedLight = false;
}

void GlLayer::setSharedLight(Light *light) {
  Light *oldLight = _light;
  _light = light;

  if (!_sharedLight)
    delete oldLight;

  _sharedLight = true;
}

void GlLayer::acceptVisitor(GlSceneVisitor *visitor) {
  assert(visitor);
  visitor->visit(this);
  if (isVisible()) {
    _composite.acceptVisitor(visitor);
  }
}

void GlLayer::addGlEntity(GlEntity *entity, const std::string &name) {
  assert(entity);
  entity->setLayer(this);
  _composite.addGlEntity(entity, name);
  if (_scene) {
    entity->addListener(_scene);
  }
  sendEvent(GlLayerEvent(GlLayerEvent::ENTITY_ADDED_IN_LAYER, this, entity));
}

void GlLayer::deleteGlEntity(const std::string &key) {
  GlEntity *entity = _composite.findGlEntity(key);
  if (entity) {
    if (_scene) {
      entity->removeListener(_scene);
    }
    sendEvent(GlLayerEvent(GlLayerEvent::ENTITY_REMOVED_FROM_LAYER, this, entity));
    entity->setLayer(nullptr);
    _composite.deleteGlEntity(key);
  }
}

void GlLayer::deleteGlEntity(GlEntity *entity) {
  assert(entity);
  if (_scene) {
    entity->removeListener(_scene);
  }
  sendEvent(GlLayerEvent(GlLayerEvent::ENTITY_REMOVED_FROM_LAYER, this, entity));
  entity->setLayer(nullptr);
  _composite.deleteGlEntity(entity);
}

void GlLayer::clear(bool deleteEntities) {
  if (_scene) {
    std::map<std::string, GlEntity *>::const_iterator it = getGlEntities().begin();
    for (; it != getGlEntities().end(); ++it) {
      sendEvent(GlLayerEvent(GlLayerEvent::ENTITY_REMOVED_FROM_LAYER, this, it->second));
      it->second->removeListener(_scene);
    }
  }
  _composite.reset(deleteEntities);
}

GlEntity *GlLayer::findGlEntity(const std::string &key) {
  return _composite.findGlEntity(key);
}

const std::map<std::string, GlEntity *> &GlLayer::getGlEntities() const {
  return _composite.getGlEntities();
}

bool GlLayer::is3d() const {
  return _camera->is3d();
}

void GlLayer::setVisible(bool visible) {
  if (_visible != visible) {
    _visible = visible;
    sendEvent(GlLayerEvent(GlLayerEvent::LAYER_VISIBILITY_CHANGED, this));
  }
}
