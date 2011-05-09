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

#ifndef Tulip_GLSCENEOBSERVER_H
#define Tulip_GLSCENEOBSERVER_H

#include <set>
#include <string>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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

    enum GlSceneEventType {TLP_ADDLAYER=0, TLP_DELLAYER, TLP_MODIFYLAYER, TLP_MODIFYENTITY};

    GlSceneEvent(const GlScene &scene,GlSceneEventType sceneEventType,const std::string &layerName,GlLayer *layer);

    GlSceneEvent(const GlScene &scene,GlSimpleEntity *entity);

    GlSimpleEntity *getGlSimpleEntity();

    std::string getLayerName();

    GlLayer *getLayer();

  protected :

    GlSceneEventType sceneEventType;
    std::string layerName;
    GlLayer *layer;
    GlSimpleEntity *glSimpleEntity;

  };
}

#endif

