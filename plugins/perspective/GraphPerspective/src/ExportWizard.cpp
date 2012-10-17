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
#include "ExportWizard.h"
#include "ui_ExportWizard.h"

#include <QtGui/QAbstractButton>
#include <QtGui/QFileDialog>

#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/ExportModule.h>
#include <tulip/ForEach.h>

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PluginModel.h>

using namespace tlp;
using namespace std;

ExportWizard::ExportWizard(Graph *g, QWidget *parent): QWizard(parent), _ui(new Ui::ExportWizard), _graph(g) {
  _ui->setupUi(this);
  button(QWizard::FinishButton)->setEnabled(false);

  PluginModel<tlp::ExportModule>* model = new PluginModel<tlp::ExportModule>(_ui->importModules);

  _ui->importModules->setModel(model);
  _ui->importModules->setRootIndex(model->index(0, 0));
  _ui->importModules->expandAll();
  connect(_ui->importModules->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(algorithmSelected(QModelIndex)));

  _ui->parametersList->setItemDelegate(new TulipItemDelegate);
  connect(_ui->parametersList, SIGNAL(destroyed()), _ui->parametersList->itemDelegate(), SLOT(deleteLater()));
  _ui->parametersList->verticalHeader()->setResizeMode(QHeaderView::Fixed);
  connect(_ui->importModules, SIGNAL(doubleClicked(QModelIndex)), button(QWizard::FinishButton), SLOT(click()));

  _ui->parametersFrame->hide();
  updateFinishButton();
}

ExportWizard::~ExportWizard() {
  delete _ui->parametersList->model();
  delete _ui;
}

void ExportWizard::algorithmSelected(const QModelIndex& index) {
  QString alg(index.data().toString());
  _ui->parametersFrame->setVisible(!alg.isEmpty());
  QAbstractItemModel* oldModel = _ui->parametersList->model();
  QAbstractItemModel* newModel = NULL;

  if (PluginLister::pluginExists(alg.toStdString())) {
    newModel = new ParameterListModel(PluginLister::getPluginParameters(alg.toStdString()),_graph);
  }

  _ui->parametersList->setModel(newModel);

  delete oldModel;
  updateFinishButton();
}

QString ExportWizard::algorithm() const {
  if (_ui->importModules->selectionModel()->hasSelection())
    return _ui->importModules->selectionModel()->selectedIndexes()[0].data().toString();

  return QString::null;
}

tlp::DataSet ExportWizard::parameters() const {
  ParameterListModel* model = dynamic_cast<ParameterListModel*>(_ui->parametersList->model());

  if (model == NULL)
    return DataSet();

  return model->parametersValues();
}

QString ExportWizard::outputFile() const {
  return _ui->pathEdit->text();
}

void ExportWizard::updateFinishButton() {
  button(QWizard::FinishButton)->setEnabled(_ui->parametersList->model() != NULL);
}

void ExportWizard::pathChanged(QString s) {
  QString selectedExport = QString::null;
  _ui->algFrame->setEnabled(!s.isEmpty());
  button(QWizard::FinishButton)->setEnabled(!s.isEmpty());

  std::list<std::string> modules = PluginLister::instance()->availablePlugins<ExportModule>();
  for(std::list<std::string>::iterator it = modules.begin(); it != modules.end(); ++it) {
    ExportModule* p = PluginLister::instance()->getPluginObject<ExportModule>(*it,NULL);
    if (s.endsWith(p->fileExtension().c_str())) {
      selectedExport = it->c_str();
      delete p;
      break;
    }
    delete p;
  }

  if (selectedExport.isNull())
    return;

  PluginModel<tlp::ExportModule>* model = static_cast<PluginModel<tlp::ExportModule>*>(_ui->importModules->model());
  QModelIndexList results = model->match(_ui->importModules->rootIndex(), Qt::DisplayRole, selectedExport, 1, Qt::MatchExactly | Qt::MatchRecursive);
  if (results.size()==0)
    return;
  _ui->importModules->setCurrentIndex(results[0]);
}

void ExportWizard::browseButtonClicked() {
  _ui->pathEdit->setText(QFileDialog::getSaveFileName(this));
}
