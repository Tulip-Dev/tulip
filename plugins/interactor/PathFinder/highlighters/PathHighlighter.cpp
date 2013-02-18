/*
 * PathHighlighter.cpp
 *
 *  Created on: Apr 14, 2010
 *      Author: Ludwig Fiolka
 */

#include "PathHighlighter.h"

using namespace tlp;
using namespace std;

#include <tulip/GlMainWidget.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphComposite.h>

PathHighlighter::PathHighlighter(const string &name) :
  name(name), backupScene(0), entityId(0) {
}

PathHighlighter::~PathHighlighter() {
  if (backupScene)
    backupScene->removeLayer(getWorkingLayer(backupScene), true);
}

GlGraphInputData *PathHighlighter::getInputData(GlMainWidget *glMainWidget) {
  return glMainWidget->getScene()->getGlGraphComposite()->getInputData();
}

GlLayer *PathHighlighter::getWorkingLayer(GlScene *scene) {
  GlLayer *result(scene->getLayer(getName()));

  if (result)
    return result;

  result = new GlLayer(getName(), false);
  result->setSharedCamera(&scene->getLayer("Main")->getCamera());
  scene->addExistingLayer(result);
  return result;
}

void PathHighlighter::clear() {
  if (backupScene) {
    GlLayer *layer = getWorkingLayer(backupScene);

    for (map<string, bool>::iterator it = entities.begin(); it != entities.end(); ++it) {
      string entityName(it->first);
      bool deleteOnExit(it->second);
      GlSimpleEntity *entity(layer->findGlEntity(entityName));

      if (entity) {
        layer->deleteGlEntity(entity);

        if (deleteOnExit)
          delete entity;
      }
    }

    entities.clear();
  }
}

void PathHighlighter::addGlEntity(GlScene *scene, GlSimpleEntity *entity, bool deleteOnExit, const string &name) {
  string realName = name;

  if (realName.compare("") == 0) {
    stringstream ss;
    ss << getName() << entityId++;
    realName = ss.str();
  }

  backupScene = scene;
  GlLayer *layer = getWorkingLayer(backupScene);
  entities[realName] = deleteOnExit;
  layer->addGlEntity(entity, realName);
}
