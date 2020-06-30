/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <algorithm>

#include <QWidget>

#include <tulip/Interactor.h>
#include <tulip/PluginLister.h>
#include <tulip/View.h>

using namespace std;
using namespace tlp;

QMap<std::string, QList<std::string>> InteractorLister::_compatibilityMap =
    QMap<std::string, QList<std::string>>();

bool interactorLessThan(Interactor *a, Interactor *b) {
  return a->priority() > b->priority();
}

void InteractorLister::initInteractorsDependencies() {
  _compatibilityMap.clear();

  QMap<Interactor *, string> interactorToName;

  std::list<std::string> interactors(PluginLister::availablePlugins<Interactor>());

  for (const std::string &interactorName : interactors) {
    interactorToName[PluginLister::getPluginObject<Interactor>(interactorName, nullptr)] =
        interactorName;
  }

  std::list<std::string> views(PluginLister::availablePlugins<View>());

  for (const std::string &viewName : views) {
    QList<Interactor *> compatibleInteractors;

    for (auto i : interactorToName.keys()) {
      if (i->isCompatible(viewName))
        compatibleInteractors << i;
    }

    std::sort(compatibleInteractors.begin(), compatibleInteractors.end(), interactorLessThan);

    QList<string> compatibleNames;

    for (auto i : compatibleInteractors)
      compatibleNames << interactorToName[i];

    _compatibilityMap[viewName] = compatibleNames;
  }

  for (auto i : interactorToName.keys())
    delete i;
}

QList<string> InteractorLister::compatibleInteractors(const std::string &viewName) {
  return _compatibilityMap[viewName];
}

void Interactor::setupConfigWidget(QWidget *w) {
  w->setObjectName("interactorConfigWidget");
}
