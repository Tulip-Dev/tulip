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
#include "ImportWizard.h"
#include "ui_ImportWizard.h"

#include <QtGui/QAbstractButton>

#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/ImportModule.h>
#include <tulip/ForEach.h>

#include <tulip/GraphHierarchiesModel.h>

using namespace tlp;
using namespace std;

ImportWizard::ImportWizard(QWidget *parent): QWizard(parent), _ui(new Ui::ImportWizard) {
  _ui->setupUi(this);

  QSet<QString> groups;
  list<string> importModules = PluginLister::instance()->availablePlugins<ImportModule>();

  for(list<string>::const_iterator it = importModules.begin(); it != importModules.end(); ++it) {
    string algName(*it);
    groups.insert(PluginLister::pluginInformations(algName)->group().c_str());
  }

  _ui->categoryList->addItems(groups.toList());
  _ui->parametersList->setItemDelegate(new TulipItemDelegate);
  connect(_ui->parametersList, SIGNAL(destroyed()), _ui->parametersList->itemDelegate(), SLOT(deleteLater()));

  _ui->parametersFrame->hide();
  updateFinishButton();
}

ImportWizard::~ImportWizard() {
  delete _ui->parametersList->model();
  delete _ui;
}

void ImportWizard::algorithmSelected(const QString& alg) {
  _ui->parametersFrame->setVisible(!alg.isEmpty());
  QAbstractItemModel* oldModel = _ui->parametersList->model();
  QAbstractItemModel* newModel = NULL;

  if (PluginLister::pluginExists(alg.toStdString())) {
    newModel = new ParameterListModel(PluginLister::getPluginParameters(alg.toStdString()));
  }

  _ui->parametersList->setModel(newModel);

  delete oldModel;
  updateFinishButton();
}

void ImportWizard::groupSelected(const QString& group) {
  _ui->algorithmList->clear();

  list<string> importModules = PluginLister::instance()->availablePlugins<ImportModule>();

  for(list<string>::const_iterator it = importModules.begin(); it != importModules.end(); ++it) {
    string algName(*it);

    if (group == PluginLister::pluginInformations(algName)->group().c_str())
      _ui->algorithmList->addItem(algName.c_str());
  }

  _ui->algorithmFrame->setEnabled(_ui->algorithmList->count() > 0);
  updateFinishButton();
}

QString ImportWizard::algorithm() const {
  if (_ui->algorithmList->currentItem() != NULL)
    return _ui->algorithmList->currentItem()->text();

  return QString::null;
}

QString ImportWizard::group() const {
  if (_ui->categoryList->currentItem() != NULL)
    return _ui->categoryList->currentItem()->text();

  return QString::null;
}

tlp::DataSet ImportWizard::parameters() const {
  ParameterListModel* model = dynamic_cast<ParameterListModel*>(_ui->parametersList->model());

  if (model == NULL)
    return DataSet();

  return model->parametersValues();
}

void ImportWizard::updateFinishButton() {
  button(QWizard::FinishButton)->setEnabled(!algorithm().isNull() || (!_ui->algorithmFrame->isEnabled() && !group().isNull()));
}
