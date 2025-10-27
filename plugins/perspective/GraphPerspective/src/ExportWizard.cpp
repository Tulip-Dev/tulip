/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include "PluginDocDialog.h"

#include <QAbstractButton>
#include <QFileDialog>
#include <QMessageBox>

#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/ExportModule.h>

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PluginModel.h>

using namespace tlp;
using namespace std;

ExportWizard::ExportWizard(Graph *g, const QString &exportFile, QWidget *parent)
    : QWizard(parent), _ui(new Ui::ExportWizard), _graph(g) {
#if !defined(__LINUX)
  setWizardStyle(QWizard::ClassicStyle);
#endif

  _ui->setupUi(this);
  button(QWizard::FinishButton)->setEnabled(false);

  bool darkBackground =
      _ui->exportModules->palette().color(backgroundRole()) != QColor(255, 255, 255);
  // update foreground colors according to background color
  if (darkBackground) {
    auto ss = _ui->exportModules->styleSheet();
    ss.replace("black", "white");
    _ui->exportModules->setStyleSheet(ss);
    _ui->parameters->setStyleSheet("QHeaderView::section { color: white }");
  }

  PluginModel<tlp::ExportModule> *model = new PluginModel<tlp::ExportModule>(_ui->exportModules);

  _ui->exportModules->setModel(model);
  _ui->exportModules->setRootIndex(model->index(0, 0));
  _ui->exportModules->expandAll();
  connect(_ui->exportModules->selectionModel(),
          SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this,
          SLOT(algorithmSelected(const QModelIndex &)));

  connect(_ui->exportModules, SIGNAL(doubleClicked(const QModelIndex &)),
          button(QWizard::FinishButton), SLOT(click()));

  // display OK instead of Finish
  setButtonText(QWizard::FinishButton, "OK");
  _ui->parametersFrame->hide();
  updateFinishButton();

  // Help button is used to display export plugin doc
  // as soon as an import plugin is selected
  button(QWizard::HelpButton)->setVisible(false);
  connect(this, SIGNAL(helpRequested()), this, SLOT(helpButtonClicked()));

  _ui->pathEdit->setText(exportFile);
}

ExportWizard::~ExportWizard() {
  delete _ui->parameters->model();
  delete _ui;
}

void ExportWizard::algorithmSelected(const QModelIndex &index) {
  QString alg(index.data().toString());
  string algs(tlp::QStringToTlpString(alg));
  _ui->parametersFrame->setVisible(!alg.isEmpty());

  if (PluginLister::pluginExists(algs)) {
    setButtonText(QWizard::HelpButton, QString("%1 documentation").arg(alg));
    button(QWizard::HelpButton)->setVisible(true);
    QAbstractItemModel *oldModel = _ui->parameters->model();
    ParameterListModel::configureTableView(_ui->parameters, PluginLister::getPluginParameters(algs),
                                           _graph);
    delete oldModel;
  } else
    button(QWizard::HelpButton)->setVisible(false);

  QString parametersText("<b>Parameters</b>");
  parametersText += "&nbsp;<font size=-2>[" + alg + "]</font>";
  _ui->parametersLabel->setText(parametersText);

  updateFinishButton();
}

QString ExportWizard::algorithm() const {
  if (_ui->exportModules->selectionModel()->hasSelection())
    return _ui->exportModules->selectionModel()->selectedIndexes()[0].data().toString();

  return QString();
}

tlp::DataSet ExportWizard::parameters() const {
  ParameterListModel *model = dynamic_cast<ParameterListModel *>(_ui->parameters->model());

  if (model == nullptr)
    return DataSet();

  return model->parametersValues();
}

QString ExportWizard::outputFile() const {
  return _ui->pathEdit->text();
}

void ExportWizard::updateFinishButton() {
  button(QWizard::FinishButton)->setEnabled(_ui->parameters->model() != nullptr);
}

void ExportWizard::pathChanged(QString s) {
  QString selectedExport;
  _ui->algFrame->setEnabled(!s.isEmpty());
  button(QWizard::FinishButton)->setEnabled(!s.isEmpty());

  std::list<std::string> modules = PluginLister::availablePlugins<ExportModule>();

  for (std::list<std::string>::iterator itm = modules.begin(); itm != modules.end(); ++itm) {
    ExportModule *p = PluginLister::getPluginObject<ExportModule>(*itm);
    std::list<std::string> extension = p->fileExtensions();

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

  PluginModel<tlp::ExportModule> *model =
      static_cast<PluginModel<tlp::ExportModule> *>(_ui->exportModules->model());
  QModelIndexList results = model->match(_ui->exportModules->rootIndex(), Qt::DisplayRole,
                                         selectedExport, 1, Qt::MatchExactly | Qt::MatchRecursive);

  if (results.empty()) {
    _ui->exportModules->clearSelection();
    _ui->parametersFrame->setVisible(false);
    button(QWizard::HelpButton)->setVisible(false);
    return;
  }

  _ui->exportModules->setCurrentIndex(results[0]);
}

void ExportWizard::browseButtonClicked() {
  QString filter;
  QString all = "all supported formats (";
  const std::list<std::string> modules = PluginLister::availablePlugins<ExportModule>();

  for (std::list<std::string>::const_iterator itm = modules.begin(); itm != modules.end(); ++itm) {
    ExportModule *p = PluginLister::getPluginObject<ExportModule>(*itm);
    const std::list<std::string> extension = p->fileExtensions();
    filter += tlpStringToQString(p->name()) + " (";

    for (list<string>::const_iterator it = extension.begin(); it != extension.end(); ++it) {
      QString format("*.");
      format += QString("%1 ").arg((*it).c_str());
      filter += format;
      all += format;
    }

    filter.resize(filter.length() - 1);
    filter += ");;";
    delete p;
  }

  filter.resize(filter.length() - 2);
  all.resize(all.length() - 1);
  all = all + ");;" + filter;
  QString exportFile =
      QFileDialog::getSaveFileName(this, "Export file", _ui->pathEdit->text(), all, nullptr
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

void ExportWizard::helpButtonClicked() {
  // display current import plugin documentation
  ParameterListModel *model = static_cast<ParameterListModel *>(_ui->parameters->model());
  auto index = _ui->exportModules->selectionModel()->currentIndex();
  PluginDocDialog::showDoc(parentWidget(), index.data().toString(),
                           index.data(Qt::ToolTipRole).toString(), model);
}

bool ExportWizard::validateCurrentPage() {
  QString exportFile = outputFile();

  // check correct extension
  ExportModule *p =
      PluginLister::getPluginObject<ExportModule>(tlp::QStringToTlpString(algorithm()));
  std::list<std::string> extension;

  if (p != nullptr)
    extension = p->fileExtensions();

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
