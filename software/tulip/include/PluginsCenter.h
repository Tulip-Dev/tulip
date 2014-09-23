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

#ifndef PLUGINSCENTER_
#define PLUGINSCENTER_

#include <tulip/TulipRelease.h>

#include <QWidget>
#include <QMap>
#include <QNetworkReply>

namespace Ui {
class PluginsCenterData;
}

class PluginInformationsListItem;

class PluginsCenter: public QWidget {
  Q_OBJECT

  Ui::PluginsCenterData* _ui;

  QStringList _categoryFilters;
  QString _nameFilter;

  PluginInformationsListItem* _currentItem;

public:
  explicit PluginsCenter(QWidget *parent=NULL);

public slots:
  void reportPluginErrors(const QMap<QString,QString>&errors);

  void showErrorsPage();
  void showWelcomePage();
  void showRepositoriesPage();

  void searchAll();
  void searchAlgorithms();
  void searchImportExport();
  void searchGlyphs();
  void searchViews();
  void searchInteractors();
  void searchPerspectives();

  void setNameFilter(const QString&filter);
  void setCategoryFilter(const QString&filter);
  void setCategoryFilters(const QStringList&filters);

  void refreshFilter();
protected slots:
  void sideListRowChanged(int i);
  void itemFocused();

  void testingChecked(bool);
  void stableChecked(bool);
  void repoAdded();
  void repoRemoved();

};

#endif // PLUGINSCENTER_

