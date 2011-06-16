#include "PluginResultsList.h"

#include "PluginInformationsListItem.h"

#include <tulip/PluginManager.h>
#include <tulip/PluginInformations.h>
#include <QtGui/QVBoxLayout>

PluginResultsList::PluginResultsList(QWidget *parent)
  : QWidget(parent), _pluginManager(new tlp::PluginManager), _focusedItem(0) {
}

void PluginResultsList::setTypeFilter(const QString &) {

}

void PluginResultsList::setNameFilter(const QString &) {

}

void PluginResultsList::refreshResults() {
  QLayout *oldLayout = layout();
  delete oldLayout;
  QVBoxLayout *searchLayout = new QVBoxLayout;
  QList<tlp::PluginInformations *> lst = _pluginManager->pluginsList(tlp::PluginManager::All);
  tlp::PluginInformations *infos;
  foreach(infos,lst) {
    PluginInformationsListItem *item = new PluginInformationsListItem(infos);
    connect(item,SIGNAL(gotFocus()),this,SLOT(changeFocus()));
    searchLayout->addWidget(item);
  }
  setLayout(searchLayout);
}

void PluginResultsList::changeFocus() {
  PluginInformationsListItem *oldFocus = qobject_cast<PluginInformationsListItem *>(_focusedItem),
      *newFocus = qobject_cast<PluginInformationsListItem *>(sender());
  _focusedItem = newFocus;
  if (oldFocus)
    oldFocus->collapse();
  newFocus->expand();
}
