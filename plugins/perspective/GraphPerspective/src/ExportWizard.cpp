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
#include "ExportWizard.h"
#include "ui_ExportWizard.h"

#include <QAbstractButton>
#include <QFileDialog>

#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/ExportModule.h>
#include <tulip/ImportModule.h>
#include <tulip/ForEach.h>

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PluginModel.h>

using namespace tlp;
using namespace std;

ExportWizard::ExportWizard(Graph *g, const QString& exportFile, QWidget *parent): QWizard(parent), _ui(new Ui::ExportWizard), _graph(g) {
  _ui->setupUi(this);
  button(QWizard::FinishButton)->setEnabled(false);

  PluginModel<tlp::ExportModule>* model = new PluginModel<tlp::ExportModule>(_ui->exportModules);

  _ui->exportModules->setModel(model);
  _ui->exportModules->setRootIndex(model->index(0, 0));
  _ui->exportModules->expandAll();
  connect(_ui->exportModules->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(algorithmSelected(QModelIndex)));

  _ui->parametersList->setItemDelegate(new TulipItemDelegate);
  connect(_ui->parametersList, SIGNAL(destroyed()), _ui->parametersList->itemDelegate(), SLOT(deleteLater()));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  _ui->parametersList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
#else
  _ui->parametersList->verticalHeader()->setResizeMode(QHeaderView::Fixed);
#endif
  connect(_ui->exportModules, SIGNAL(doubleClicked(QModelIndex)), button(QWizard::FinishButton), SLOT(click()));

  _ui->parametersFrame->hide();
  updateFinishButton();

  _ui->pathEdit->setText(exportFile);
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

  if (PluginLister::pluginExists(tlp::QStringToTlpString(alg))) {
    newModel = new ParameterListModel(PluginLister::getPluginParameters(tlp::QStringToTlpString(alg)),_graph);
  }

  _ui->parametersList->setModel(newModel);

  delete oldModel;
  updateFinishButton();
}

QString ExportWizard::algorithm() const {
  if (_ui->exportModules->selectionModel()->hasSelection())
    return _ui->exportModules->selectionModel()->selectedIndexes()[0].data().toString();

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

  std::list<std::string> modules =
    PluginLister::instance()->availablePlugins<ExportModule>();
  std::list<std::string> imports =
    PluginLister::instance()->availablePlugins<ImportModule>();

  for(std::list<std::string>::iterator itm = modules.begin(); itm != modules.end(); ++itm) {
    ExportModule* p =
      PluginLister::instance()->getPluginObject<ExportModule>(*itm,NULL);
    std::string extension = p->fileExtension();

    if (s.endsWith(extension.c_str())) {
      selectedExport = itm->c_str();
      delete p;
      break;
    }

    // add special case to allow export *.gz
    extension += ".gz";

    if (s.endsWith((extension).c_str())) {
      // look for a corresponding import module supporting the gz extension
      for(std::list<std::string>::const_iterator it = imports.begin();
          it != imports.end(); ++it) {
        ImportModule* m = PluginLister::instance()->getPluginObject<ImportModule>(*it, NULL);
        std::list<std::string> extensions(m->fileExtensions());

        for(std::list<std::string>::const_iterator ite = extensions.begin();
            ite != extensions.end(); ++ite) {
          if (extension == *ite) {
            // found it
            selectedExport = itm->c_str();
            break;
          }
        }

        delete m;

        if (selectedExport != NULL) {
          break;
        }
      }

      if (selectedExport != NULL) {
        delete p;
        break;
      }
    }

    delete p;
  }

  if (selectedExport.isNull()) {
    _ui->exportModules->clearSelection();
    return;
  }

  PluginModel<tlp::ExportModule>* model = static_cast<PluginModel<tlp::ExportModule>*>(_ui->exportModules->model());
  QModelIndexList results = model->match(_ui->exportModules->rootIndex(), Qt::DisplayRole, selectedExport, 1, Qt::MatchExactly | Qt::MatchRecursive);

  if (results.empty())
    return;

  _ui->exportModules->setCurrentIndex(results[0]);
}

void ExportWizard::browseButtonClicked() {
  QString exportFile = QFileDialog::getSaveFileName(this, "Export file", _ui->pathEdit->text());

  if (!exportFile.isEmpty())
    _ui->pathEdit->setText(exportFile);
}
