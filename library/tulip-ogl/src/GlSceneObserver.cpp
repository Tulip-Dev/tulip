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
#include <cassert>
#include <tulip/GlSceneObserver.h>

#include <tulip/GlScene.h>

using namespace std;

namespace tlp {

GlSceneEvent::GlSceneEvent(const GlScene &scene,GlSceneEventType sceneEventType,const std::string &layerName,GlLayer *layer)
  :Event(scene, Event::TLP_MODIFICATION),
   sceneEventType(sceneEventType),
   layerName(layerName),
   layer(layer) {
}

GlSceneEvent::GlSceneEvent(const GlScene &scene,GlSceneEventType sceneEventType,GlSimpleEntity *entity)
  :Event(scene, Event::TLP_MODIFICATION),
   sceneEventType(sceneEventType),
   glSimpleEntity(entity) {
}

GlSimpleEntity *GlSceneEvent::getGlSimpleEntity() const {
  return glSimpleEntity;
}

std::string GlSceneEvent::getLayerName() const {
  return layerName;
}

GlLayer *GlSceneEvent::getLayer() const {
  return layer;
}

GlSceneEvent::GlSceneEventType GlSceneEvent::getSceneEventType() const {
  return sceneEventType;
}

}

