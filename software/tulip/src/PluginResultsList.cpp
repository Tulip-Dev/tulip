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
#include "PluginResultsList.h"

#include "PluginInformationsListItem.h"
#include "DetailedPluginInformationsWidget.h"

#include <time.h>
#include <tulip/PluginManager.h>
#include <tulip/PluginInformations.h>
#include <QtGui/QVBoxLayout>
#include <QtCore/QDebug>
#include <tulip/TulipSettings.h>

bool PluginResultsList::PluginInformationsSorter::operator ()(const tlp::PluginInformations* i1,const tlp::PluginInformations* i2) {
  int score1 = 0,score2 = 0;

  if (i1->isInstalled())
    score1=2;

  if (i1->updateAvailable())
    score1=1;

  if (i2->isInstalled())
    score2=2;

  if (i2->updateAvailable())
    score2=1;

  if (score1 == score2)
    return i1->name() < i2->name();

  return score1 < score2;
}

PluginResultsList::PluginResultsList(QWidget *parent)
  : QScrollArea(parent), _focusedItem(0), _resultsListCache(0), _pluginWidgetsCache(std::map<tlp::PluginInformations *,PluginInformationsListItem *,PluginInformationsSorter>(PluginInformationsSorter())) {
  initPluginsCache();
}

void PluginResultsList::setTypeFilter(const QString &s,bool autoRefresh) {
  setTypeFilter(QStringList() << s,autoRefresh);
}

void PluginResultsList::setTypeFilter(const QStringList &lst,bool autoRefresh) {
  _typeFilter = lst;

  if (autoRefresh)
    refreshResults();
}

void PluginResultsList::setNameFilter(const QString &s,bool autoRefresh) {
  _nameFilter = s;

  if (autoRefresh)
    refreshResults();
}

void PluginResultsList::refreshResults() {
  QWidget *mainWidget = new QWidget();
  mainWidget->setObjectName(QString::fromUtf8("pluginsSearchListContent"));
  QVBoxLayout *searchLayout = new QVBoxLayout(mainWidget);
  mainWidget->setLayout(searchLayout);

  for (cache::iterator it = _pluginWidgetsCache.begin(); it != _pluginWidgetsCache.end(); ++it) {
    tlp::PluginInformations *infos = it->first;
    PluginInformationsListItem *item = it->second;
    item->setVisible(false);
    item->setParent(0);
    // apply filters
    bool typeMatches=false, nameMatches=false;

    if (_typeFilter.isEmpty()) typeMatches = true;
    else {
      QString f;
      foreach(f,_typeFilter)

      if (infos->type().startsWith(f)) typeMatches = true;
    }

    if (_nameFilter.isEmpty() || infos->name().contains(_nameFilter,Qt::CaseInsensitive)) nameMatches = true;

    if (!typeMatches || !nameMatches) {
      continue;
    }

    searchLayout->addWidget(item);
    item->setVisible(true);
  }

  searchLayout->addItem(new QSpacerItem(1,1,QSizePolicy::Maximum,QSizePolicy::Expanding));
  setWidget(mainWidget);

  if (_resultsListCache) {
    delete _resultsListCache;
    _resultsListCache=0;
  }
}

void PluginResultsList::changeFocus() {
  PluginInformationsListItem *oldFocus = qobject_cast<PluginInformationsListItem *>(_focusedItem),
                              *newFocus = qobject_cast<PluginInformationsListItem *>(sender());
  _focusedItem = newFocus;

  if (oldFocus)
    oldFocus->collapse();

  newFocus->expand();
}

void PluginResultsList::restoreResultsList() {
  if (!_resultsListCache)
    return;

  setWidget(_resultsListCache);
  _resultsListCache=0;
}

void PluginResultsList::switchToDetailedInformations() {
  PluginInformationsListItem *item = qobject_cast<PluginInformationsListItem *>(sender());
  _resultsListCache = takeWidget();
  DetailedPluginInformationsWidget *detailedInfosWidget = new DetailedPluginInformationsWidget(item->pluginInformations());
  setWidget(detailedInfosWidget);
  connect(detailedInfosWidget,SIGNAL(goBack()),this,SLOT(restoreResultsList()));
  connect(detailedInfosWidget,SIGNAL(fetch()),this,SLOT(pluginFetch()));
  connect(detailedInfosWidget,SIGNAL(remove()),this,SLOT(pluginRemove()));
}

void PluginResultsList::pluginFetch() {
  PluginInformationsListItem *listItem = dynamic_cast<PluginInformationsListItem *>(sender());

  if (listItem) {
    emit fetch(listItem->pluginInformations());
  }
  else {
    emit fetch(dynamic_cast<DetailedPluginInformationsWidget *>(sender())->pluginInformations());
  }
}

void PluginResultsList::pluginRemove() {
  PluginInformationsListItem *listItem = dynamic_cast<PluginInformationsListItem *>(sender());

  if (listItem)
    emit remove(listItem->pluginInformations());
  else
    emit remove(dynamic_cast<DetailedPluginInformationsWidget *>(sender())->pluginInformations());
}

void PluginResultsList::initPluginsCache() {
  tlp::PluginInformations *i;

  for(cache::iterator it = _pluginWidgetsCache.begin(); it != _pluginWidgetsCache.end(); ++it) {
    i = it->first;
    delete it->second;
    delete i;
  }

  _pluginWidgetsCache.clear();
  foreach(i, tlp::PluginManager::pluginsList(tlp::PluginManager::All)) {
    PluginInformationsListItem *item = new PluginInformationsListItem(i);
    connect(item,SIGNAL(gotFocus()),this,SLOT(changeFocus()));
    connect(item,SIGNAL(showDetailedInformations()),this,SLOT(switchToDetailedInformations()));
    connect(item,SIGNAL(fetch()),this,SLOT(pluginFetch()));
    connect(item,SIGNAL(remove()),this,SLOT(pluginRemove()));
    _pluginWidgetsCache[i] = item;
  }

  if (isVisible())
    refreshResults();
}

tlp::PluginInformations* PluginResultsList::featuredPlugin() {
  srand(time(NULL));
  unsigned int i=0,rnd=rand()%_pluginWidgetsCache.size();
  bool returnNext=false;

  for(cache::iterator it = _pluginWidgetsCache.begin(); it != _pluginWidgetsCache.end(); ++it) {
    tlp::PluginInformations* infos = it->first;
    returnNext = i++>=rnd;

    if (returnNext && (!infos->isInstalled() || i == _pluginWidgetsCache.size()-1))
      return infos;
  }

  return NULL;
}
