#include "PluginResultsList.h"

#include "PluginInformationsListItem.h"
#include "DetailedPluginInformationsWidget.h"

#include <tulip/PluginManager.h>
#include <tulip/PluginInformations.h>
#include <QtGui/QVBoxLayout>
#include <QtCore/QDebug>
#include <tulip/TulipSettings.h>

PluginResultsList::PluginResultsList(QWidget *parent)
  : QScrollArea(parent), _focusedItem(0), _resultsListCache(0) {
  foreach(const QString& remoteLocation, TulipSettings::instance().remoteLocations()) {
    tlp::PluginManager::addRemoteLocation(remoteLocation);
  }
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
  tlp::PluginInformations *infos;
  foreach(infos,_pluginWidgetsCache.keys()) {
    PluginInformationsListItem *item = _pluginWidgetsCache[infos];
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

  if (listItem)
    emit fetch(listItem->pluginInformations());
  else
    emit fetch(dynamic_cast<DetailedPluginInformationsWidget *>(sender())->pluginInformations());
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
  foreach(i,_pluginWidgetsCache.keys()) {
    delete i;
    delete _pluginWidgetsCache[i];
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
}
