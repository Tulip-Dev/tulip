/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
    layer(layer){
}

GlSceneEvent::GlSceneEvent(const GlScene &scene,GlSimpleEntity *entity)
  :Event(scene, Event::TLP_MODIFICATION),
    sceneEventType(TLP_MODIFYENTITY),
    glSimpleEntity(entity){
}

GlSimpleEntity *GlSceneEvent::getGlSimpleEntity(){
  assert(sceneEventType==TLP_MODIFYENTITY);
  return glSimpleEntity;
}

std::string GlSceneEvent::getLayerName(){
  assert(sceneEventType==TLP_ADDLAYER || sceneEventType==TLP_DELLAYER || sceneEventType==TLP_MODIFYLAYER);
  return layerName;
}

GlLayer *GlSceneEvent::getLayer(){
  assert(sceneEventType==TLP_ADDLAYER || sceneEventType==TLP_DELLAYER || sceneEventType==TLP_MODIFYLAYER);
  return layer;
}
}

