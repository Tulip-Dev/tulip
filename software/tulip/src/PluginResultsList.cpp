#include "PluginResultsList.h"

#include "PluginInformationsListItem.h"

#include <tulip/PluginManager.h>
#include <tulip/PluginInformations.h>
#include <QtGui/QVBoxLayout>

PluginResultsList::PluginResultsList(QWidget *parent)
  : QScrollArea(parent), _pluginManager(new tlp::PluginManager), _focusedItem(0) {
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
  _focusedItem = 0;
  emit hideHtml();
  QWidget *mainWidget = new QWidget();
  mainWidget->setObjectName(QString::fromUtf8("pluginsSearchListContent"));
  QVBoxLayout *searchLayout = new QVBoxLayout(mainWidget);
  mainWidget->setLayout(searchLayout);
  QList<tlp::PluginInformations *> lst = _pluginManager->pluginsList(tlp::PluginManager::All);
  tlp::PluginInformations *infos;
  foreach(infos,lst) {

    bool typeMatches=false, nameMatches=false;
    // Apply type filter
    if (_typeFilter.isEmpty())
      typeMatches = true;
    else {
      QString f;
      foreach(f,_typeFilter)
        if (infos->type() == f)
          typeMatches = true;
    }
    // Apply name filter
    if (_nameFilter.isEmpty() || infos->name().contains(_nameFilter))
      nameMatches = true;
    if (!typeMatches || !nameMatches)
      continue;

    PluginInformationsListItem *item = new PluginInformationsListItem(infos,mainWidget);
    connect(item,SIGNAL(gotFocus()),this,SLOT(changeFocus()));
    connect(item,SIGNAL(showInfos(QString)),this,SIGNAL(showHtml(QString)));
    searchLayout->addWidget(item);
  }
  searchLayout->addItem(new QSpacerItem(1,1,QSizePolicy::Maximum,QSizePolicy::Expanding));
  setWidget(mainWidget);
}

void PluginResultsList::changeFocus() {
  emit hideHtml();
  PluginInformationsListItem *oldFocus = qobject_cast<PluginInformationsListItem *>(_focusedItem),
      *newFocus = qobject_cast<PluginInformationsListItem *>(sender());
  _focusedItem = newFocus;
  if (oldFocus)
    oldFocus->collapse();
  newFocus->expand();
}
