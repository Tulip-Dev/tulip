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

