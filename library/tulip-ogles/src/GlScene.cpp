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

#include <GL/glew.h>

#include <algorithm>
#include <cstdlib>
#include <climits>
#include <cstdio>
#include <sstream>

#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/GlQuadTreeLODCalculator.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlLODSceneVisitor.h>
#include <tulip/GlRect2D.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlFrameBufferObject.h>
#include <tulip/GlUtils.h>
#include <tulip/GlGraph.h>
#include <tulip/GlTextureManager.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlBuffer.h>

using namespace std;
using namespace tlp;

SelectedEntity::SelectedEntity() : _entityType(NO_ENTITY_SELECTED), _entity(nullptr), _graphEltId(UINT_MAX) {}

SelectedEntity::SelectedEntity(GlEntity *entity) : _entityType(ENTITY_SELECTED), _entity(entity), _graphEltId(UINT_MAX) {}

SelectedEntity::SelectedEntity(SelectedEntityType type, GlGraph *glGraph, unsigned int id) : _entityType(type), _entity(glGraph), _graphEltId(id) {}

SelectedEntity::SelectedEntityType SelectedEntity::getEntityType() const {
  return _entityType;
}

GlEntity *SelectedEntity::getGlEntity() const {
  return _entity;
}

GlGraph *SelectedEntity::getGlGraph() const {
  return dynamic_cast<GlGraph*>(_entity);
}

node SelectedEntity::getNode()const {
  return node(_graphEltId);
}

edge SelectedEntity::getEdge()const {
  return edge(_graphEltId);
}

const string mainLayerName = "Main";

GlScene::GlScene(GlLODCalculator *calculator) : _lodCalculator(calculator), _backgroundColor(255, 255, 255, 255), _clearBufferAtDraw(true), _sceneNeedRedraw(true),
  _backBufferTexture(0), _backBufferBackup(nullptr), _pickingMode(false), _backupBackBuffer(true) {
  if (!_lodCalculator)
    _lodCalculator = new GlQuadTreeLODCalculator();
  _lodCalculator->setRenderingEntitiesFlag(RenderingGlEntities);
  addExistingLayer(new GlLayer(mainLayerName));
  ostringstream oss;
  oss << "backBufferTexture" << reinterpret_cast<unsigned long long>(this);
  _backBufferTextureName = oss.str();

}

GlScene::~GlScene() {

  for(GlLayer *layer: _layersList) {
    delete layer;
  }

  delete _lodCalculator;
  delete [] _backBufferBackup;
}

void GlScene::setViewport(const Vec4i &viewport) {
  if (_viewport != viewport) {
    _viewport=viewport;
    _sceneNeedRedraw = true;
    if (_backupBackBuffer) {
      delete [] _backBufferBackup;
      _backBufferBackup = new unsigned char[viewport[2]*viewport[3]*4];
    }
  }
}

void GlScene::setViewport(int x, int y, int width, int height) {
  setViewport(Vec4i(x, y, width, height));
}

void GlScene::setViewOrtho(bool viewOrtho) {
  getMainLayer()->getCamera()->setViewOrtho(viewOrtho);
}

bool GlScene::isViewOrtho() const {
  return getMainLayer()->getCamera()->isViewOrtho();
}

void GlScene::setBackgroundColor(const Color& color) {
  if (_backgroundColor != color) {
    _backgroundColor = color;
    _sceneNeedRedraw = true;
  }
}

void GlScene::initGlParameters(bool drawBackBufferBackup) {
  OpenGlConfigManager::instance().initExtensions();
  glViewport(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);
  glScissor(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);
  if (!drawBackBufferBackup) {
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0xFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    if (!_pickingMode) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
      glDisable(GL_BLEND);
    }
  }
}

void GlScene::requestDraw() {
  sendEvent(GlSceneEvent(GlSceneEvent::DRAW_REQUEST, this));
}

void GlScene::draw() {

  if (_sceneNeedRedraw || _pickingMode) {

    if (_clearBufferAtDraw) {
      if (!_pickingMode) {
        glClearColor(_backgroundColor.getRGL(), _backgroundColor.getGGL(), _backgroundColor.getBGL(), _backgroundColor.getAGL());
      } else {
        glClearColor(0.f, 0.f, 0.f, 0.f);
      }
      glClear(GL_COLOR_BUFFER_BIT);
    }

    BoundingBox sceneBoundingBox = getBoundingBox();

    if (_lastSceneBoundingBox[0] != sceneBoundingBox[0] || _lastSceneBoundingBox[1] != sceneBoundingBox[1]) {
      _lodCalculator->setSceneBoundingBox(sceneBoundingBox);
      _lastSceneBoundingBox = sceneBoundingBox;
    }

    initGlParameters();

    for(GlLayer *layer: _layersList) {

      if (!layer->isVisible()) {
        continue;
      }

      Camera &camera = *(layer->getCamera());
      Light &light = *(layer->getLight());
      GlLODSceneVisitor glLODSceneVisitor(_lodCalculator);
      layer->acceptVisitor(&glLODSceneVisitor);
      if (camera.is3d()) {
        glEnable(GL_DEPTH_TEST);
      } else {
        glDisable(GL_DEPTH_TEST);
      }
      camera.setViewport(_viewport);
      camera.initGl();
      if (!_pickingMode) {
        _lodCalculator->compute(&camera);
      } else {
        _lodCalculator->compute(&camera, _selectionViewport);
      }
      const vector<GlEntityLODUnit> &lodResult = _lodCalculator->getGlEntitiesResult(layer);
      if (!lodResult.empty()) {
        for (size_t i = 0 ; i < lodResult.size() ; ++i) {
          if (lodResult[i].lod < 0 || !lodResult[i].glEntity->isVisible()) {
            continue;
          }
          glStencilFunc(GL_LEQUAL, lodResult[i].glEntity->getStencil(), 0xFF);
          lodResult[i].glEntity->draw(camera, light, _pickingMode);
        }
      }

      glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);

    if (!_pickingMode && _backupBackBuffer && !GlFrameBufferObject::bufferBound()) {
      backupBackBuffer();
    }

  } else if (_backupBackBuffer && !GlFrameBufferObject::bufferBound()) {
    initGlParameters(true);
    drawBackBufferBackup();
  }

  _sceneNeedRedraw = false;

  if (GlShaderProgram::getCurrentActiveShader()) {
    GlShaderProgram::getCurrentActiveShader()->desactivate();
  }

  GlBuffer::release(GlBuffer::VertexBuffer);
  GlBuffer::release(GlBuffer::IndexBuffer);

}

void GlScene::backupBackBuffer() {
  if (_backBufferTexture == 0) {
    glGenTextures(1, &_backBufferTexture);
    glBindTexture(GL_TEXTURE_2D, _backBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    GlTextureManager::instance()->addExternalTexture(_backBufferTextureName, _backBufferTexture);
  }
  glBindTexture(GL_TEXTURE_2D, _backBufferTexture);
#ifdef __EMSCRIPTEN__
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _viewport[0], _viewport[1], _viewport[2], _viewport[3], 0);
#else
  // On desktop OpenGL, glCopyTexImage2D does not preserve antialiasing so we grab the back buffer content
  // and create a texture fro
  glReadBuffer(GL_BACK);
  glReadPixels(_viewport[0], _viewport[1], _viewport[2], _viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, _backBufferBackup);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _viewport[2], _viewport[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, _backBufferBackup);
#endif
  glBindTexture(GL_TEXTURE_2D, 0);
  checkOpenGLError();
}

void GlScene::drawBackBufferBackup() {
  glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Camera camera2d(false);
  camera2d.setViewport(_viewport);
  camera2d.initGl();
  Vec2f bl(0, 0);
  Vec2f tr(_viewport[2], _viewport[3]);
  GlRect2D rect(bl, tr, Color::White);
  rect.setTexture(_backBufferTextureName);
  rect.draw(camera2d);
}

GlLayer *GlScene::createLayer(const string &name, bool is3d) {
  GlLayer *oldLayer = getLayer(name);

  if(oldLayer != nullptr) {
    warning() << "Warning : You have a layer in the scene with same name, previous layer will be removed" << endl;
    removeLayer(oldLayer);
  }

  GlLayer *newLayer = nullptr;
  if (!is3d) {
    newLayer = new GlLayer(name, is3d);
  } else {
    newLayer = new GlLayer(name, getMainLayer()->getCamera(), getMainLayer()->getLight());
  }
  _layersList.push_back(newLayer);
  newLayer->setScene(this);

  sendEvent(GlSceneEvent(GlSceneEvent::LAYER_ADDED_IN_SCENE, this, newLayer));

  return newLayer;
}

GlLayer *GlScene::createLayerBefore(const string &layerName, const string &beforeLayerWithName, bool is3d) {
  GlLayer *newLayer = nullptr;
  GlLayer *oldLayer = getLayer(layerName);

  for(vector<GlLayer*>::iterator it = _layersList.begin() ; it != _layersList.end() ; ++it) {
    if((*it)->getName() == beforeLayerWithName) {
      if (!is3d) {
        newLayer = new GlLayer(layerName, is3d);
      } else {
        newLayer = new GlLayer(layerName, getMainLayer()->getCamera(), getMainLayer()->getLight());
      }
      _layersList.insert(it, newLayer);
      newLayer->setScene(this);

      if(oldLayer != nullptr) {
        removeLayer(oldLayer);
        warning() << "Warning : You have a layer in the scene with same name, previous layer will be removed" << endl;
      }

      break;
    }
  }

  if (newLayer) {
    sendEvent(GlSceneEvent(GlSceneEvent::LAYER_ADDED_IN_SCENE, this, newLayer));
  }

  return newLayer;
}

GlLayer *GlScene::createLayerBefore(const string &layerName, GlLayer *beforeLayer, bool is3d) {
  return createLayerBefore(layerName, beforeLayer->getName(), is3d);
}

GlLayer *GlScene::createLayerAfter(const string &layerName, const string &afterLayerWithName, bool is3d) {
  GlLayer *newLayer = nullptr;
  GlLayer *oldLayer = getLayer(layerName);

  for(vector<GlLayer*>::iterator it = _layersList.begin() ; it != _layersList.end(); ++it) {
    if((*it)->getName() == afterLayerWithName) {
      ++it;
      if (!is3d) {
        newLayer = new GlLayer(layerName, is3d);
      } else {
        newLayer = new GlLayer(layerName, getMainLayer()->getCamera(), getMainLayer()->getLight());
      }
      _layersList.insert(it, newLayer);
      newLayer->setScene(this);

      if(oldLayer != nullptr) {
        warning() << "Warning : You have a layer in the scene with same name, previous layer will be removed" << endl;
        removeLayer(oldLayer);
      }

      break;
    }
  }

  if (newLayer) {
    sendEvent(GlSceneEvent(GlSceneEvent::LAYER_ADDED_IN_SCENE, this, newLayer));
  }

  return newLayer;
}

GlLayer *GlScene::createLayerAfter(const string &layerName, GlLayer *afterLayer, bool is3d) {
  return createLayerAfter(layerName, afterLayer->getName(), is3d);
}

void GlScene::addExistingLayer(GlLayer *layer) {
  GlLayer *oldLayer=getLayer(layer->getName());

  if(oldLayer != nullptr) {
    warning() << "Warning : You have a layer in the scene with same name, previous layer will be removed" << endl;
    removeLayer(oldLayer);
  }

  _layersList.push_back(layer);
  layer->setScene(this);
  _sceneNeedRedraw = true;

  sendEvent(GlSceneEvent(GlSceneEvent::LAYER_ADDED_IN_SCENE, this, layer));
  layerAddedInScene(layer);

}

bool GlScene::addExistingLayerBefore(GlLayer *layer, const string &beforeLayerWithName) {
  bool insertionOk = false;
  GlLayer *oldLayer = getLayer(layer->getName());

  for(vector<GlLayer*>::iterator it = _layersList.begin() ; it != _layersList.end() ; ++it) {
    if((*it)->getName() == beforeLayerWithName) {
      _layersList.insert(it, layer);
      layer->setScene(this);
      if(oldLayer != nullptr) {
        warning() << "Warning : You have a layer in the scene with same name, previous layer will be removed" << endl;
        removeLayer(oldLayer);
      }
      insertionOk=true;
      _sceneNeedRedraw = true;

      sendEvent(GlSceneEvent(GlSceneEvent::LAYER_ADDED_IN_SCENE, this, layer));
      layerAddedInScene(layer);

      break;
    }
  }

  return insertionOk;
}

bool GlScene::addExistingLayerBefore(GlLayer *layer, GlLayer *beforeLayer) {
  return addExistingLayerBefore(layer, beforeLayer->getName());
}

bool GlScene::addExistingLayerAfter(GlLayer *layer, const string &afterLayerWithName) {
  bool insertionOk=false;
  GlLayer *oldLayer=getLayer(layer->getName());

  for(vector<GlLayer*>::iterator it = _layersList.begin() ; it!=_layersList.end() ; ++it) {
    if((*it)->getName() == afterLayerWithName) {
      ++it;
      _layersList.insert(it, layer);
      layer->setScene(this);
      if(oldLayer != nullptr) {
        warning() << "Warning : You have a layer in the scene with same name, previous layer will be removed" << endl;
        removeLayer(oldLayer);
      }
      insertionOk=true;
      _sceneNeedRedraw = true;

      sendEvent(GlSceneEvent(GlSceneEvent::LAYER_ADDED_IN_SCENE, this, layer));
      layerAddedInScene(layer);

      break;
    }
  }

  return insertionOk;
}

bool GlScene::addExistingLayerAfter(GlLayer *layer, GlLayer *afterLayer) {
  return addExistingLayerAfter(layer, afterLayer->getName());
}

void GlScene::layerAddedInScene(GlLayer *layer) {
  for (const pair<std::string, GlEntity*> &entity : layer->getGlEntities()) {
    _lodCalculator->addGlEntity(layer, entity.second);
  }
}

GlLayer *GlScene::getLayer(const string &name) const {
  for(GlLayer *layer : _layersList) {
    if(layer->getName() == name) {
      return layer;
    }
  }

  return nullptr;
}

GlLayer *GlScene::getMainLayer() const {
  return getLayer(mainLayerName);
}

void GlScene::removeLayer(const string &name, bool deleteLayer) {
  for(vector<GlLayer*>::iterator it=_layersList.begin() ; it!=_layersList.end() ; ++it) {
    if((*it)->getName() == name) {
      _lodCalculator->removeLayer(*it);
      _layersList.erase(it);
      _sceneNeedRedraw = true;

      sendEvent(GlSceneEvent(GlSceneEvent::LAYER_REMOVED_FROM_SCENE, this, *it));

      if(deleteLayer)
        delete *it;

      return;
    }
  }
}

void GlScene::removeLayer(GlLayer *layer, bool deleteLayer) {
  for(vector<GlLayer*>::iterator it = _layersList.begin() ; it != _layersList.end() ; ++it) {
    if (*it == layer) {
      _lodCalculator->removeLayer(layer);
      _layersList.erase(it);
      _sceneNeedRedraw = true;

      sendEvent(GlSceneEvent(GlSceneEvent::LAYER_REMOVED_FROM_SCENE, this, *it));

      if(deleteLayer)
        delete *it;

      return;
    }
  }
}

void GlScene::clearLayersList() {
  for(GlLayer *layer : _layersList) {
    _lodCalculator->removeLayer(layer);
    sendEvent(GlSceneEvent(GlSceneEvent::LAYER_REMOVED_FROM_SCENE, this, layer));
    delete layer;
  }
  _layersList.clear();
  _sceneNeedRedraw = true;
}

BoundingBox GlScene::getBoundingBox() {
  GlBoundingBoxSceneVisitor glBBSceneVisitor;
  for(GlLayer *layer: _layersList) {
    if (layer->is3d()) {
      layer->acceptVisitor(&glBBSceneVisitor);
    }
  }
  return glBBSceneVisitor.getBoundingBox();
}

void GlScene::centerScene(BoundingBox boundingBox) {
  BoundingBox sceneBB = boundingBox;
  if (!sceneBB.isValid()) {
    sceneBB = getBoundingBox();
  }
  for(GlLayer *layer: _layersList) {
    if (layer->is3d() && !layer->useSharedCamera()) {
      layer->getCamera()->setViewport(_viewport);
      layer->getCamera()->centerScene(sceneBB);
    }
  }
}

void GlScene::translate(int x, int y) {
  for(GlLayer *layer: _layersList) {
    Camera *camera = layer->getCamera();
    if (camera->is3d() && !layer->useSharedCamera()) {
      Coord v1(0, 0, 0);
      Coord v2(x, y, 0);
      v1 = camera->viewportTo3DWorld(v1);
      v2 = camera->viewportTo3DWorld(v2);
      Coord move = v2 - v1;
      camera->translate(move);
    }
  }
}

void GlScene::rotate(int x, int y, int z) {
  for(GlLayer *layer: _layersList) {
    Camera *camera = layer->getCamera();
    if (camera->is3d() && !layer->useSharedCamera()) {
      Coord center = getBoundingBox().center();
      camera->translate(center);
      camera->rotateX(static_cast<float>(x/360.0 * M_PI));
      camera->rotateY(static_cast<float>(y/360.0 * M_PI));
      camera->rotateZ(static_cast<float>(z/360.0 * M_PI));
      camera->translate(-center);
    }
  }
}

void GlScene::zoomXY(int x, int y, int step) {
  for(GlLayer *layer: _layersList) {
    Camera *camera = layer->getCamera();
    if (camera->is3d() && !layer->useSharedCamera()) {
      camera->setZoomFactor(camera->getZoomFactor() * pow(1.1,step));
    }
  }
  if (step < 0) step *= -1;
  int factX = static_cast<int>(step*(static_cast<double>(_viewport[2])/2.0-x)/ 7.0);
  int factY = static_cast<int>(step*(static_cast<double>(_viewport[3])/2.0-y)/ 7.0);
  translate(-factX,factY);
}

void GlScene::zoom(int step) {
  for(GlLayer *layer: _layersList) {
    if(layer->getCamera()->is3d() && (!layer->useSharedCamera())) {
      layer->getCamera()->setZoomFactor(layer->getCamera()->getZoomFactor() * pow(1.1, step));
    }
  }
}

void GlScene::zoomFactor(float factor) {
  for(GlLayer *layer: _layersList) {
    if(layer->getCamera()->is3d() && (!layer->useSharedCamera())) {
      layer->getCamera()->setZoomFactor(layer->getCamera()->getZoomFactor() * factor);
    }
  }
}

void GlScene::treatEvent(const Event &message) {
  if (message.type() != Event::TLP_MODIFICATION) return;
  Camera *camera = dynamic_cast<Camera*>(message.sender());
  GlEntity *entity = dynamic_cast<GlEntity*>(message.sender());
  GlLayer *layer = dynamic_cast<GlLayer*>(message.sender());
  if (camera || entity || layer) {
    _sceneNeedRedraw = true;
  }
  if (entity) {
    _lodCalculator->removeGlEntity(entity->getLayer(), entity);
  }
  const GlLayerEvent *layerEvt = dynamic_cast<const GlLayerEvent*>(&message);
  if (layerEvt) {
    if (layerEvt->getType() == GlLayerEvent::ENTITY_ADDED_IN_LAYER) {
      _lodCalculator->addGlEntity(layerEvt->getGlLayer(), layerEvt->getGlEntity());
    } else if (layerEvt->getType() == GlLayerEvent::ENTITY_REMOVED_FROM_LAYER) {
      _lodCalculator->removeGlEntity(layerEvt->getGlLayer(), layerEvt->getGlEntity());
    }
  }
}

bool GlScene::selectEntities(RenderingEntitiesFlag type, int x, int y, int width, int height, vector<SelectedEntity>& selectedEntities, GlLayer *layer) {
  _pickingMode = true;
  vector<SelectedEntity> selectedEntitiesInternal;
  selectedEntities.clear();
  GlFrameBufferObject *fbo = new GlFrameBufferObject(_viewport[2], _viewport[3], GlFrameBufferObject::NoAttachment);
  fbo->bind();
  unsigned int bufferSize = width*height*4;
  unsigned char *buffer = new unsigned char[bufferSize];
  bool done = false;
  _selectionViewport = Vec4i(x - _viewport[2], _viewport[3] - y, width, height);

  std::map<GlLayer *, bool> layersVisibility;
  for (GlLayer *layer : _layersList) {
    layersVisibility[layer] = layer->isVisible();
  }

  if (layer && layer->getScene() == this) {
    for (GlLayer *layer : _layersList) {
      layer->setVisible(false);
    }
    layer->setVisible(true);
  }

  while (!done) {
    set<GlEntity*> tmpSelectedEntities;
    draw();
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    for (unsigned int i = 0 ; i < bufferSize ; i += 4) {
      Color color(buffer[i], buffer[i+1], buffer[i+2], buffer[i+3]);
      unsigned int id = colorToUint(color);
      if (id == 0) continue;
      GlEntity *entity = GlEntity::fromId(id);
      if (entity) {
        tmpSelectedEntities.insert(entity);
        entity->setVisible(false);
      }
    }
    if (tmpSelectedEntities.empty()) {
      done = true;
    } else {
      for (set<GlEntity*>::iterator it = tmpSelectedEntities.begin() ; it != tmpSelectedEntities.end() ; ++it) {
        selectedEntitiesInternal.push_back(SelectedEntity(*it));
      }
    }
  }
  delete [] buffer;
  fbo->release();
  delete fbo;
  _pickingMode = false;

  for (size_t i = 0 ; i < selectedEntitiesInternal.size() ; ++i) {
    GlEntity *entity = selectedEntitiesInternal[i].getGlEntity();
    entity->setVisible(true);
    GlGraph *glGraph = dynamic_cast<GlGraph*>(entity);
    if (!glGraph && (type & RenderingGlEntities)) {
      selectedEntities.push_back(selectedEntitiesInternal[i]);
    } else if (glGraph && ((type & RenderingNodes) || (type & RenderingEdges))) {
      set<node> selectedNodes;
      set<edge> selectedEdges;
      Camera *camera = glGraph->getLayer()->getCamera();
      camera->initGl();
      glGraph->pickNodesAndEdges(*camera, x, y, width, height, selectedNodes, selectedEdges);
      if (!selectedNodes.empty() && (type & RenderingNodes)) {
        for (set<node>::iterator it = selectedNodes.begin() ; it != selectedNodes.end() ; ++it) {
          selectedEntities.push_back(SelectedEntity(SelectedEntity::NODE_SELECTED, glGraph, it->id));
        }
      }
      if (!selectedEdges.empty() && (type & RenderingEdges)) {
        for (set<edge>::iterator it = selectedEdges.begin() ; it != selectedEdges.end() ; ++it) {
          selectedEntities.push_back(SelectedEntity(SelectedEntity::EDGE_SELECTED, glGraph, it->id));
        }
      }
    }
  }

  if (layer && layer->getScene() == this) {
    for (GlLayer *layer : _layersList) {
      layer->setVisible(layersVisibility[layer]);
    }
  }

  return !selectedEntities.empty();
}

bool GlScene::selectEntity(RenderingEntitiesFlag type, int x, int y, SelectedEntity &selectedEntity, GlLayer *layer) {
  vector<SelectedEntity> selectedEntities;
  bool ret = selectEntities(type, x-1, y-1, 3, 3, selectedEntities, layer);
  if (!selectedEntities.empty()) {
    selectedEntity = selectedEntities.front();
  } else {
    selectedEntity = SelectedEntity();
  }
  return ret;
}

std::set<tlp::GlEntity*> GlScene::getEntities() {
  set<GlEntity*> ret;
  for(GlLayer *layer: _layersList) {
    const map<string, GlEntity*> &layerEntities = layer->getGlEntities();
    for (map<string, GlEntity*>::const_iterator it2 = layerEntities.begin() ; it2 != layerEntities.end() ; ++it2) {
      GlEntity *entity = it2->second;
      ret.insert(entity);
    }
  }
  return ret;
}

static const std::string mainGlGraphName = "graph";

void GlScene::addMainGlGraph(tlp::Graph *graph) {
  addGlGraph(graph, mainGlGraphName);
}

GlGraph *GlScene::getMainGlGraph() const {
  return getGlGraph(mainGlGraphName);
}

void GlScene::addGlGraph(tlp::Graph *graph, const std::string &glGraphName) {
  getMainLayer()->addGlEntity(new GlGraph(graph), glGraphName);
}

GlGraph *GlScene::getGlGraph(const std::string &glGraphName) const {
  return dynamic_cast<GlGraph*>(getMainLayer()->findGlEntity(glGraphName));
}
