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

#include "PluginsCenter.h"

#include "PluginErrorReport.h"
#include "PluginInformationsListItem.h"

#include <tulip/Algorithm.h>
#include <tulip/TemplateAlgorithm.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/View.h>
#include <tulip/Perspective.h>
#include <tulip/Interactor.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>
#include <tulip/PluginManager.h>
#include <tulip/TulipSettings.h>

#include "ui_PluginsCenter.h"

static const int ALL_ROW          = 0;
static const int ALGORITHMS_ROW   = 1;
static const int IMPORTEXPORT_ROW = 2;
static const int GLYPHS_ROW       = 3;
static const int VIEWS_ROW        = 4;
static const int INTERACTORS_ROW  = 5;
static const int PERSPECTIVES_ROW = 6;
static const int ERRORS_ROW       = 8;

using namespace tlp;

PluginsCenter::PluginsCenter(QWidget *parent): QWidget(parent), _ui(new Ui::PluginsCenterData()), _currentItem(NULL) {
  _ui->setupUi(this);

  QStringList remoteLocs = TulipSettings::instance().remoteLocations();
  _ui->stableCheck->setChecked(remoteLocs.contains(PluginManager::STABLE_LOCATION));
  _ui->testingCheck->setChecked(remoteLocs.contains(PluginManager::TESTING_LOCATION));

  foreach(QString s,remoteLocs) {
    if (s != PluginManager::STABLE_LOCATION && s != PluginManager::TESTING_LOCATION)
      _ui->remoteLocationsList->addItem(s);
  }
}

void PluginsCenter::reportPluginErrors(const QMap<QString, QString>& errors) {
  if (!errors.empty())
    _ui->pluginsSideList->item(ERRORS_ROW)->setFlags(Qt::ItemIsEnabled | _ui->pluginsSideList->item(ERRORS_ROW)->flags());

  foreach(QString k, errors.keys()) {
    _ui->errorsLogAreaLayout->addWidget(new PluginErrorReport(k,errors[k]));
  }
  _ui->errorsLogAreaLayout->addItem(new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::Expanding));
}

void PluginsCenter::showErrorsPage() {
  _ui->pluginsContent->setCurrentWidget(_ui->errorsPage);
}

void PluginsCenter::showWelcomePage() {
  _ui->pluginsContent->setCurrentWidget(_ui->homePage);
}

void PluginsCenter::showRepositoriesPage() {
  _ui->pluginsContent->setCurrentWidget(_ui->reposPage);
}

void PluginsCenter::searchAll() {
  setCategoryFilter("");
}

void PluginsCenter::searchAlgorithms() {
  setCategoryFilters(QStringList()
                     << tlp::ALGORITHM_CATEGORY.c_str()
                     << tlp::BOOLEAN_ALGORITHM_CATEGORY.c_str()
                     << tlp::COLOR_ALGORITHM_CATEGORY.c_str()
                     << tlp::DOUBLE_ALGORITHM_CATEGORY.c_str()
                     << tlp::INTEGER_ALGORITHM_CATEGORY.c_str()
                     << tlp::LAYOUT_ALGORITHM_CATEGORY.c_str()
                     << tlp::STRING_ALGORITHM_CATEGORY.c_str()
                     << tlp::PROPERTY_ALGORITHM_CATEGORY.c_str());
}

void PluginsCenter::searchImportExport() {
  setCategoryFilters(QStringList()
                     << tlp::IMPORT_CATEGORY.c_str()
                     << tlp::EXPORT_CATEGORY.c_str());
}

void PluginsCenter::searchGlyphs() {
  setCategoryFilters(QStringList()
                     << tlp::GLYPH_CATEGORY.c_str()
                     << tlp::EEGLYPH_CATEGORY.c_str());
}

void PluginsCenter::searchViews() {
  setCategoryFilter(tlp::VIEW_CATEGORY.c_str());
}

void PluginsCenter::searchInteractors() {
  setCategoryFilter(tlp::INTERACTOR_CATEGORY.c_str());
}

void PluginsCenter::searchPerspectives() {
  setCategoryFilter(tlp::PERSPECTIVE_CATEGORY.c_str());
}

void PluginsCenter::setCategoryFilter(const QString& filter) {
  setCategoryFilters(QStringList() << filter);
}

void PluginsCenter::setCategoryFilters(const QStringList& filters) {
  _categoryFilters = filters;
  refreshFilter();
}

void PluginsCenter::setNameFilter(const QString& filter) {
  _nameFilter = filter;
  refreshFilter();
}

void PluginsCenter::refreshFilter() {
  _currentItem = NULL;

  if (_categoryFilters.isEmpty())
    _categoryFilters.push_back("");

  QVBoxLayout* lyt = new QVBoxLayout();

  foreach(QString cf,_categoryFilters) {
    foreach(PluginInformation info,PluginManager::listPlugins(PluginManager::Remote | PluginManager::Local,_nameFilter,cf)) {
      PluginInformationsListItem* item = new PluginInformationsListItem(info);
      connect(item,SIGNAL(focused()),this,SLOT(itemFocused()));
      lyt->addWidget(item);
    }
  }
  lyt->addItem(new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::Expanding));

  QString oldObjName = _ui->pluginsSearchListContent->objectName();
  _ui->pluginsSearchList->setWidget(NULL);
  _ui->pluginsSearchListContent = new QWidget();
  _ui->pluginsSearchListContent->setObjectName(oldObjName);
  _ui->pluginsSearchListContent->setLayout(lyt);
  _ui->pluginsSearchList->setWidget(_ui->pluginsSearchListContent);
  _ui->pluginsContent->setCurrentWidget(_ui->pluginsListPage);
}

void PluginsCenter::sideListRowChanged(int i) {
  switch(i) {
  case ALL_ROW:
    searchAll();
    break;

  case ALGORITHMS_ROW:
    searchAlgorithms();
    break;

  case IMPORTEXPORT_ROW:
    searchImportExport();
    break;

  case GLYPHS_ROW:
    searchGlyphs();
    break;

  case VIEWS_ROW:
    searchViews();
    break;

  case INTERACTORS_ROW:
    searchInteractors();
    break;

  case PERSPECTIVES_ROW:
    searchPerspectives();
    break;

  case ERRORS_ROW:
    showErrorsPage();
    break;
  }
}

void PluginsCenter::itemFocused() {
  if (_currentItem != NULL) {
    _currentItem->focusOut();
  }

  _currentItem = static_cast<PluginInformationsListItem*>(sender());
  _currentItem->focusIn();
}

void PluginsCenter::testingChecked(bool f) {
  if (f)
    TulipSettings::instance().addRemoteLocation(PluginManager::TESTING_LOCATION);
  else
    TulipSettings::instance().removeRemoteLocation(PluginManager::TESTING_LOCATION);
}

void PluginsCenter::stableChecked(bool f) {
  if (f)
    TulipSettings::instance().addRemoteLocation(PluginManager::STABLE_LOCATION);
  else
    TulipSettings::instance().removeRemoteLocation(PluginManager::STABLE_LOCATION);
}

void PluginsCenter::repoAdded() {
  QString location = _ui->remoteLocationText->text();
  TulipSettings::instance().addRemoteLocation(location);

  if (_ui->remoteLocationsList->findItems(location,Qt::MatchExactly).size() == 0)
    _ui->remoteLocationsList->addItem(location);
}

void PluginsCenter::repoRemoved() {
  QList<QListWidgetItem*> selected = _ui->remoteLocationsList->selectedItems();

  if (selected.empty())
    return;

  QString location = selected.first()->text();
  TulipSettings::instance().removeRemoteLocation(location);
  QList<QListWidgetItem*> lst = _ui->remoteLocationsList->findItems(location,Qt::MatchExactly);

  if (lst.size() > 0) {
    foreach(QListWidgetItem* i, lst) {
      delete i;
    }
  }
}
