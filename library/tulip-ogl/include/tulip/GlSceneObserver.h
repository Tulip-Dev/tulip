/*
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
///@cond DOXYGEN_HIDDEN


#ifndef Tulip_GLSCENEOBSERVER_H
#define Tulip_GLSCENEOBSERVER_H

#include <string>

#include <tulip/tulipconf.h>
#include <tulip/Observable.h>

namespace tlp {

class GlLayer;
class GlScene;
class GlSimpleEntity;

/** \brief An observer to the scene
 * An observer to the scene who observe layers
 */
class TLP_GL_SCOPE GlSceneEvent : public Event {
public:

  enum GlSceneEventType {TLP_ADDLAYER=0, TLP_DELLAYER, TLP_MODIFYLAYER, TLP_MODIFYENTITY, TLP_DELENTITY};

  GlSceneEvent(const GlScene &scene,GlSceneEventType sceneEventType,const std::string &layerName,GlLayer *layer);

  GlSceneEvent(const GlScene &scene, GlSceneEventType sceneEventType, GlSimpleEntity *entity);

  GlSimpleEntity *getGlSimpleEntity() const;

  std::string getLayerName() const;

  GlLayer *getLayer() const;

  GlSceneEventType getSceneEventType() const;

protected :

  GlSceneEventType sceneEventType;
  std::string layerName;
  GlLayer *layer;
  GlSimpleEntity *glSimpleEntity;

};
}

#endif

///@endcond
