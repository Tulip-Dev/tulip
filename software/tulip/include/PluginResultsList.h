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
#ifndef PLUGINRESULTSLIST_H
#define PLUGINRESULTSLIST_H

#include <QtGui/QScrollArea>
#include <QtCore/QMap>

namespace tlp {
class PluginManager;
class PluginInformations;
}
class PluginInformationsListItem;

class PluginResultsList : public QScrollArea {
  Q_OBJECT
public:
  explicit PluginResultsList(QWidget *parent=NULL);
  tlp::PluginInformations* featuredPlugin();

public slots:
  void setTypeFilter(const QStringList &,bool autoRefresh=true);
  void setTypeFilter(const QString &,bool autoRefresh=true);
  void setNameFilter(const QString &,bool autoRefresh=true);
  void initPluginsCache();
  void refreshResults();

protected slots:
  void changeFocus();
  void switchToDetailedInformations();
  void restoreResultsList();

  void pluginFetch();
  void pluginRemove();

signals:
  void fetch(tlp::PluginInformations *);
  void remove(tlp::PluginInformations *);

private:
  //Used to sort plugin informations into the cache
  struct PluginInformationsSorter {
    bool operator()(const tlp::PluginInformations*, const tlp::PluginInformations*);
  };

  QWidget *_focusedItem;
  QWidget *_resultsListCache;
  typedef std::map<tlp::PluginInformations *,PluginInformationsListItem *,PluginInformationsSorter> cache;
  cache _pluginWidgetsCache;

  QStringList _typeFilter;
  QString _nameFilter;
};

#endif // PLUGINRESULTSLIST_H
