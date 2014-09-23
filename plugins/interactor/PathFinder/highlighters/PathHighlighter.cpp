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

  if (backupScene)
    backupScene->removeListener(this);

  backupScene = scene;
  backupScene->addListener(this);

  GlLayer *layer = getWorkingLayer(backupScene);
  entities[realName] = deleteOnExit;
  layer->addGlEntity(entity, realName);
}

void PathHighlighter::treatEvent(const Event &ev) {
  if (ev.type()==Event::TLP_DELETE) {
    if (ev.sender() == backupScene) {
      // reset backupScene to avoid segfault
      // on destructor invocation
      backupScene = NULL;
    }
  }
}
