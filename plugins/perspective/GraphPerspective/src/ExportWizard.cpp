/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <QMessageBox>

#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/ExportModule.h>
#include <tulip/ForEach.h>

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PluginModel.h>

using namespace tlp;
using namespace std;

ExportWizard::ExportWizard(Graph *g, const QString &exportFile, QWidget *parent)
    : QWizard(parent), _ui(new Ui::ExportWizard), _graph(g) {
  _ui->setupUi(this);
  button(QWizard::FinishButton)->setEnabled(false);

  PluginModel<tlp::ExportModule> *model = new PluginModel<tlp::ExportModule>(_ui->exportModules);

  _ui->exportModules->setModel(model);
  _ui->exportModules->setRootIndex(model->index(0, 0));
  _ui->exportModules->expandAll();
  connect(_ui->exportModules->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          this, SLOT(algorithmSelected(QModelIndex)));

  _ui->parametersList->setItemDelegate(new TulipItemDelegate(_ui->parametersList));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  _ui->parametersList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
#else
  _ui->parametersList->verticalHeader()->setResizeMode(QHeaderView::Fixed);
#endif
  connect(_ui->exportModules, SIGNAL(doubleClicked(QModelIndex)), button(QWizard::FinishButton),
          SLOT(click()));

  // display OK instead of Finish
  setButtonText(QWizard::FinishButton, "OK");
  _ui->parametersFrame->hide();
  updateFinishButton();

  _ui->pathEdit->setText(exportFile);
}

ExportWizard::~ExportWizard() {
  delete _ui->parametersList->model();
  delete _ui;
}

void ExportWizard::algorithmSelected(const QModelIndex &index) {
  QString alg(index.data().toString());
  string algs(tlp::QStringToTlpString(alg));
  _ui->parametersFrame->setVisible(!alg.isEmpty());
  QAbstractItemModel *oldModel = _ui->parametersList->model();
  QAbstractItemModel *newModel = NULL;

  if (PluginLister::pluginExists(algs)) {
    newModel = new ParameterListModel(PluginLister::getPluginParameters(algs), _graph);
  }

  _ui->parametersList->setModel(newModel);

  QString parametersText("<b>Parameters</b>");
  parametersText += "&nbsp;<font size=-2>[" + alg + "]</font>";
  _ui->parametersLabel->setText(parametersText);

  delete oldModel;
  updateFinishButton();
}

QString ExportWizard::algorithm() const {
  if (_ui->exportModules->selectionModel()->hasSelection())
    return _ui->exportModules->selectionModel()->selectedIndexes()[0].data().toString();

  return QString();
}

tlp::DataSet ExportWizard::parameters() const {
  ParameterListModel *model = dynamic_cast<ParameterListModel *>(_ui->parametersList->model());

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
  QString selectedExport;
  _ui->algFrame->setEnabled(!s.isEmpty());
  button(QWizard::FinishButton)->setEnabled(!s.isEmpty());

  std::list<std::string> modules = PluginLister::instance()->availablePlugins<ExportModule>();

  for (std::list<std::string>::iterator itm = modules.begin(); itm != modules.end(); ++itm) {
    ExportModule *p = PluginLister::instance()->getPluginObject<ExportModule>(*itm, NULL);
    std::list<std::string> extension = p->allFileExtensions();

    for (list<string>::const_iterator extit = extension.begin(); extit != extension.end();
         ++extit) {
      if (s.endsWith((*extit).c_str())) {
        selectedExport = itm->c_str();
        delete p;
        break;
      }
    }

    if (!selectedExport.isEmpty())
      break;
  }

  if (selectedExport.isEmpty()) {
    _ui->exportModules->clearSelection();
    return;
  }

  PluginModel<tlp::ExportModule> *model =
      static_cast<PluginModel<tlp::ExportModule> *>(_ui->exportModules->model());
  QModelIndexList results = model->match(_ui->exportModules->rootIndex(), Qt::DisplayRole,
                                         selectedExport, 1, Qt::MatchExactly | Qt::MatchRecursive);

  if (results.empty())
    return;

  _ui->exportModules->setCurrentIndex(results[0]);
}

void ExportWizard::browseButtonClicked() {
  QString filter;
  QString all = "all supported formats (";
  const std::list<std::string> modules = PluginLister::instance()->availablePlugins<ExportModule>();

  for (std::list<std::string>::const_iterator itm = modules.begin(); itm != modules.end(); ++itm) {
    ExportModule *p = PluginLister::instance()->getPluginObject<ExportModule>(*itm, NULL);
    const std::list<std::string> extension = p->allFileExtensions();
    filter += tlpStringToQString(p->name()) + " (";

    for (list<string>::const_iterator it = extension.begin(); it != extension.end(); ++it) {
      filter += (*it).c_str() + QString(" ");
      all += (*it).c_str() + QString(" ");
    }

    filter.resize(filter.length() - 1);
    filter += ");;";
    delete p;
  }

  filter.resize(filter.length() - 2);
  all.resize(all.length() - 1);
  all = all + ");;" + filter;
  QString exportFile =
      QFileDialog::getSaveFileName(this, "Export file", _ui->pathEdit->text(), all, NULL
// on MacOSX selectedFilter is ignored by the
// native dialog
#ifdef __APPLE__
                                   ,
                                   QFileDialog::DontUseNativeDialog
#endif
                                   );

  if (!exportFile.isEmpty()) {
    _ui->pathEdit->setText(exportFile);
  }
}

bool ExportWizard::validateCurrentPage() {
  QString exportFile = outputFile();

  // check correct extension
  ExportModule *p = PluginLister::instance()->getPluginObject<ExportModule>(
      tlp::QStringToTlpString(algorithm()), NULL);
  std::list<std::string> extension;

  if (p != NULL)
    extension = p->allFileExtensions();

  bool extok(false);
  QString ext;

  for (list<string>::const_iterator it = extension.begin(); it != extension.end(); ++it) {
    ext += tlp::tlpStringToQString(*it) + ", ";

    if (exportFile.endsWith(tlp::tlpStringToQString(*it))) {
      extok = true;
    }
  }

  delete p;

  if (!extok) {
    if (extension.size() == 1)
      _ui->pathEdit->setText(exportFile + "." + tlp::tlpStringToQString(*extension.begin()));
    else {
      ext.resize(ext.length() - 2);
      QString msg = "Filename does not terminate with a valid extension. ";

      if (!algorithm().isEmpty())
        msg += "Please add one.<br>Valid extensions for " + algorithm() + " are: " + ext;

      QMessageBox::warning(parentWidget(), "Filename not valid", msg);
      return false;
    }
  }

  // if file exists and is valid, check if user wants to overwrite it
  return (!exportFile.isEmpty() &&
          (!QFile::exists(exportFile) ||
           (QMessageBox::question(
                parentWidget(), "Overwriting an existing file",
                "The export file already exists.<br/>Do you really want to overwrite it?",
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)));
}
