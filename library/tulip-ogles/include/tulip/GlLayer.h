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

#ifndef GLLAYER_H
#define GLLAYER_H

#include <tulip/GlComposite.h>

namespace tlp {

class GlScene;
class GlSceneVisitor;

class TLP_GLES_SCOPE GlLayer : public tlp::Observable {

  friend class GlScene;

public:

  GlLayer(const std::string& name, bool is3d = true);

  GlLayer(const std::string& name, Camera *camera, Light *light);

  ~GlLayer();

  GlScene *getScene() const {
    return _scene;
  }

  std::string getName() const {
    return _name;
  }

  void setCamera(Camera *camera);

  void setSharedCamera(Camera *camera);

  Camera *getCamera() const {
    return _camera;
  }

  void setLight(Light *light);

  void setSharedLight(Light *light);

  Light *getLight() const {
    return _light;
  }

  void setVisible(bool visible);

  bool isVisible() const {
    return _visible;
  }

  void addGlEntity(GlEntity *entity, const std::string &name);

  void deleteGlEntity(const std::string &key);

  void deleteGlEntity(GlEntity *entity);

  GlEntity* findGlEntity(const std::string &key);

  const std::map<std::string, GlEntity*> &getGlEntities() const;

  void clear(bool deleteEntities=false);

  bool useSharedCamera() const {
    return _sharedCamera;
  }

  void acceptVisitor(GlSceneVisitor *visitor);

  GlComposite* getComposite() const {
    return const_cast<GlComposite*>(&_composite);
  }

  bool is3d() const;

protected :

  void setScene(GlScene *_scene);

private:

  std::string _name;

  GlComposite _composite;
  GlScene *_scene;

  Camera *_camera;
  Light *_light;
  bool _sharedCamera;
  bool _sharedLight;
  bool _visible;

};

class GlLayerEvent : public tlp::Event {
public:

  enum GlLayerEventType {ENTITY_ADDED_IN_LAYER = 0,
                         ENTITY_REMOVED_FROM_LAYER,
                         LAYER_VISIBILITY_CHANGED
                        };

  GlLayerEvent(GlLayerEventType type, GlLayer *layer, GlEntity *entity=nullptr,
               Event::EventType evtType = Event::TLP_MODIFICATION)

    : Event(*layer, evtType), _type(type), _layer(layer), _entity(entity) {}

  GlLayer *getGlLayer() const {
    return _layer;
  }

  GlEntity *getGlEntity() const {
    return _entity;
  }

  GlLayerEventType getType() const {
    return _type;
  }

protected:

  GlLayerEventType _type;
  GlLayer *_layer;
  GlEntity *_entity;
};

}

#endif // GLLAYER_H
