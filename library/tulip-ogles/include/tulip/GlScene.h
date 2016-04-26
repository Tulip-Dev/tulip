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

#ifndef GLSCENE_H
#define GLSCENE_H

#include <tulip/Vector.h>
#include <tulip/Color.h>
#include <tulip/BoundingBox.h>
#include <tulip/Observable.h>

#include <vector>
#include <set>

#include <tulip/GlLODCalculator.h>

namespace tlp {

class GlLayer;
class GlEntity;
class Graph;
class GlGraph;

struct TLP_GLES_SCOPE SelectedEntity {

  enum SelectedEntityType {
    NO_ENTITY_SELECTED = 0,
    ENTITY_SELECTED = 1,
    NODE_SELECTED = 2,
    EDGE_SELECTED = 3
  };

  SelectedEntity();

  SelectedEntity(GlEntity *entity);

  SelectedEntity(SelectedEntityType type, GlGraph *glGraph, unsigned int id);

  SelectedEntityType getEntityType() const;

  GlEntity *getGlEntity() const;

  GlGraph *getGlGraph() const;

  node getNode() const;

  edge getEdge() const;

protected :

  SelectedEntityType _entityType;
  GlEntity *_entity;
  unsigned int _graphEltId;
};

class TLP_GLES_SCOPE GlScene : public tlp::Observable {

public:

  GlScene(GlLODCalculator *calculator=nullptr);

  ~GlScene();

  void requestDraw();

  void draw();

  void centerScene(tlp::BoundingBox boundingBox = tlp::BoundingBox());

  void setViewport(const tlp::Vec4i &viewport);

  void setViewport(int x, int y, int width, int height);

  void setViewOrtho(bool viewOrtho);

  bool isViewOrtho() const;

  tlp::Vec4i getViewport() const {
    return _viewport;
  }

  void setBackgroundColor(const tlp::Color& color);

  tlp::Color getBackgroundColor() const {
    return _backgroundColor;
  }

  GlLayer *getMainLayer() const;

  GlLayer *createLayer(const std::string &name, bool is3d = true);

  GlLayer *createLayerBefore(const std::string &layerName, const std::string &beforeLayerWithName, bool is3d = true);

  GlLayer *createLayerBefore(const std::string &layerName, GlLayer *beforeLayer, bool is3d = true);

  GlLayer *createLayerAfter(const std::string &layerName, const std::string &afterLayerWithName, bool is3d = true);

  GlLayer *createLayerAfter(const std::string &layerName, GlLayer *afterLayer, bool is3d = true);

  void addExistingLayer(GlLayer *layer);

  bool addExistingLayerBefore(GlLayer *layer, const std::string &beforeLayerWithName);

  bool addExistingLayerBefore(GlLayer *layer, GlLayer *beforeLayer);

  bool addExistingLayerAfter(GlLayer *layer, const std::string &afterLayerWithName);

  bool addExistingLayerAfter(GlLayer *layer, GlLayer *afterLayer);

  GlLayer *getLayer(const std::string& name) const;

  void removeLayer(const std::string& name, bool deleteLayer=false);

  void removeLayer(GlLayer *layer, bool deleteLayer=false);

  const std::vector<GlLayer*> &getLayersList() const {
    return _layersList;
  }

  void clearLayersList();

  tlp::BoundingBox getBoundingBox();

  void setClearBufferAtDraw(bool clear) {
    _clearBufferAtDraw = clear;
  }

  bool getClearBufferAtDraw() const {
    return _clearBufferAtDraw;
  }

  void translate(int x, int y);

  void rotate(int x, int y, int z);

  void zoomXY(int x, int y, int step);

  void zoom(int step);

  void zoomFactor(float factor);

  bool selectEntities(RenderingEntitiesFlag type, int x, int y, int width, int height, std::vector<SelectedEntity>& selectedEntities, GlLayer *layer = nullptr);

  bool selectEntity(RenderingEntitiesFlag type, int x, int y, SelectedEntity &selectedEntity, GlLayer *layer = nullptr);

  std::set<GlEntity*> getEntities();

  void forceRedraw() { _sceneNeedRedraw = true;}

  void setSceneNeedRedraw(bool sceneNeedRedraw) {
    _sceneNeedRedraw = sceneNeedRedraw;
  }

  void initGlParameters(bool drawBackBufferBackup=false);

  void setBackupBackBuffer(bool backup) {_backupBackBuffer = backup;}

  unsigned int getBackBufferTexture() const {
    return _backBufferTexture;
  }

  std::string getBackBufferTextureName() const {
    return _backBufferTextureName;
  }

  void addMainGlGraph(tlp::Graph *graph);

  GlGraph *getMainGlGraph() const;

  void addGlGraph(tlp::Graph *graph, const std::string &glGraphName);

  GlGraph *getGlGraph(const std::string &glGraphName) const;

protected:

  void treatEvent(const tlp::Event &message);

private:

  void backupBackBuffer();
  void drawBackBufferBackup();
  void layerAddedInScene(GlLayer *layer);

  std::vector<GlLayer *> _layersList;
  GlLODCalculator *_lodCalculator;
  tlp::Vec4i _viewport, _selectionViewport;
  tlp::Color _backgroundColor;
  bool _clearBufferAtDraw;
  bool _sceneNeedRedraw;
  unsigned int _backBufferTexture;
  std::string _backBufferTextureName;
  unsigned char *_backBufferBackup;

  bool _pickingMode;
  bool _backupBackBuffer;

  BoundingBox _lastSceneBoundingBox;

};

class GlSceneEvent : public tlp::Event {
public:

  enum GlSceneEventType {DRAW_REQUEST=0,
                         LAYER_ADDED_IN_SCENE,
                         LAYER_REMOVED_FROM_SCENE
                        };

  GlSceneEvent(GlSceneEventType type, GlScene *scene, GlLayer *layer = nullptr,
               Event::EventType evtType = Event::TLP_MODIFICATION)

    : Event(*scene, evtType), _type(type), _scene(scene), _layer(layer) {}

  GlScene *getGlScene() const {
    return _scene;
  }

  GlLayer *getGlLayer() const {
    return _layer;
  }

  GlSceneEventType getType() const {
    return _type;
  }

protected:

  GlSceneEventType _type;
  GlScene *_scene;
  GlLayer *_layer;
};

}

#endif // GLSCENE_H
