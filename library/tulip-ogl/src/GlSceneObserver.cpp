/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/GlSceneObserver.h"

using namespace std;

namespace tlp {

  void GlObservableScene::notifyAddLayer(GlScene *scene, const string& name, GlLayer* layer) {
    set<GlSceneObserver *> copy(observers); //Used to preserve iteratros
    for (set<GlSceneObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
      (*itlObs)->addLayer(scene, name, layer);
  }

  void GlObservableScene::notifyDelLayer(GlScene *scene, const string& name, GlLayer* layer) {
    set<GlSceneObserver *> copy(observers); //Used to preserve iteratros
    for (set<GlSceneObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
      (*itlObs)->delLayer(scene, name, layer);
  }

  void GlObservableScene::notifyModifyLayer(GlScene *scene, const string& name, GlLayer* layer) {
    set<GlSceneObserver *> copy(observers); //Used to preserve iteratros
    for (set<GlSceneObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
      (*itlObs)->modifyLayer(scene, name, layer);
  }

  void GlObservableScene::notifyModifyEntity(GlScene *scene, GlSimpleEntity *entity) {
    set<GlSceneObserver *> copy(observers); //Used to preserve iteratros
    for (set<GlSceneObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
      (*itlObs)->modifyEntity(scene, entity);
  }
}

