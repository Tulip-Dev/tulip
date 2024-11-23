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
#include "ImportWizard.h"
#include "ui_ImportWizard.h"
#include "PluginDocDialog.h"

#include <QAbstractButton>
#include <QMessageBox>

#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/ImportModule.h>

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PluginModel.h>
#include <tulip/TulipSettings.h>

using namespace tlp;
using namespace std;

ImportWizard::ImportWizard(QWidget *parent) : QWizard(parent), _ui(new Ui::ImportWizard) {
#if !defined(__LINUX)
  setWizardStyle(QWizard::ClassicStyle);
#endif

  _ui->setupUi(this);

  bool darkBackground = _ui->importModules->palette().color(backgroundRole()) != QColor("white");
  // update foreground colors according to background color
  if (darkBackground) {
    auto ss = _ui->importModules->styleSheet();
    ss.replace("black", "white");
    _ui->importModules->setStyleSheet(ss);
    _ui->parameters->setStyleSheet("QHeaderView::section { color: white }");
  }
  PluginModel<tlp::ImportModule> *model = new PluginModel<tlp::ImportModule>(_ui->importModules);

  _ui->importModules->setModel(model);
  _ui->importModules->setRootIndex(model->index(0, 0));
  _ui->importModules->expandAll();
  connect(_ui->importModules->selectionModel(),
          SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this,
          SLOT(moduleSelected(const QModelIndex &)));

  connect(_ui->importModules, SIGNAL(doubleClicked(const QModelIndex &)),
          button(QWizard::FinishButton), SLOT(click()));
  connect(_ui->searchBox, SIGNAL(textChanged(const QString &)), this,
          SLOT(setFilter(const QString &)));
  // display OK instead of Finish
  setButtonText(QWizard::FinishButton, "OK");
  // Help button is used to display import plugin doc
  // as soon as an import plugin is selected
  button(QWizard::HelpButton)->setVisible(false);
  connect(this, SIGNAL(helpRequested()), this, SLOT(helpButtonClicked()));

  _ui->parametersFrame->hide();
  QString importLabel("<html><head/><body><p align=\"justify\">Import a graph hierarchy into your "
                      "project. First, select an import method, then adjust its parameters if "
                      "needed.<br/>Click <b>Ok</b> to import your graph, then visualize it using "
                      "the ");

  if (TulipSettings::displayDefaultViews())
    importLabel += "<b>Node Link Diagram</b> and <b>Spreadsheet</b> (automatically opened) views.";
  else
    importLabel += "<img src=\":/tulip/gui/icons/16/list-add.png\"/>&nbsp;<b>Add "
                   "panel</b> button to open specific views on it.";

  importLabel += "<br/><br/>See <b>Edit</b> menu, then <b>Preferences</b> for more options when "
                 "importing a graph.</p></body></html>";
  _ui->label->setText(importLabel);
  updateFinishButton();
}

ImportWizard::~ImportWizard() {
  delete _ui->parameters->model();
  delete _ui;
}

void ImportWizard::initWithModuleFile(const std::string &module, const std::string &file) {
  // used at perspective launch time
  // to initialize file::filename default value with file
  // we first have to select the module in _ui_>importModules
  auto model = _ui->importModules->model();
  // root is the "Import" item (non visible)
  auto root = model->index(0, 1);
  // Be warn, that we consider only two levels (groups, plugins of groups)
  // loop on groups (File, Graph ...)
  for (int i = 0; i < model->rowCount(root); ++i) {
    auto index = model->index(i, 1, root);
    // loop on current group import plugins
    int j = 0;
    for (; j < model->rowCount(index); j++) {
      auto subIndex = model->index(j, 1, index);
      if (subIndex.data().toString() == QString(module.c_str())) {
        // select module
        _ui->importModules->setCurrentIndex(subIndex);
        break;
      }
    }
    if (j < model->rowCount(index))
      break;
  }

  ParameterListModel *paramModel = static_cast<ParameterListModel *>(_ui->parameters->model());
  auto dataSet = paramModel->parametersValues();
  // finally set file as default filename
  dataSet.set("file::filename", file);
  paramModel->setParametersValues(dataSet);
}

void ImportWizard::moduleSelected(const QModelIndex &index) {
  QString alg(index.data().toString());
  string algs = tlp::QStringToTlpString(alg);
  _ui->parametersFrame->setVisible(!alg.isEmpty());
  bool isGroup = index.model()->index(0, index.column(), index).isValid();

  QString parametersText("<b>Parameters</b>");

  if (!isGroup && PluginLister::pluginExists(algs)) {
    parametersText += "&nbsp;<font size=-2>[" + alg + "]</font>";
    setButtonText(QWizard::HelpButton, QString("%1 documentation").arg(alg));
    button(QWizard::HelpButton)->setVisible(true);
    _ui->parametersLabel->setEnabled(true);
    QAbstractItemModel *oldModel = _ui->parameters->model();
    ParameterListModel::configureTableView(_ui->parameters,
                                           PluginLister::getPluginParameters(algs));
    delete oldModel;
  } else {
    button(QWizard::HelpButton)->setVisible(false);
    _ui->parametersLabel->setEnabled(false);
  }

  _ui->parametersLabel->setText(parametersText);

  updateFinishButton();
}

std::string ImportWizard::module() const {
  if (_ui->importModules->selectionModel()->hasSelection())
    return _ui->importModules->selectionModel()
        ->selectedIndexes()[0]
        .data()
        .toByteArray()
        .toStdString();

  return "";
}

tlp::DataSet ImportWizard::parameters() const {
  ParameterListModel *model = dynamic_cast<ParameterListModel *>(_ui->parameters->model());

  if (model == nullptr)
    return DataSet();

  return model->parametersValues();
}

void ImportWizard::helpButtonClicked() {
  // display current import plugin documentation
  ParameterListModel *model = static_cast<ParameterListModel *>(_ui->parameters->model());
  auto index = _ui->importModules->selectionModel()->currentIndex();
  PluginDocDialog::showDoc(parentWidget(), index.data().toString(),
                           index.data(Qt::ToolTipRole).toString(), model);
}

void ImportWizard::updateFinishButton() {
  button(QWizard::FinishButton)->setEnabled(_ui->parameters->model() != nullptr);
}

void ImportWizard::setFilter(const QString &filter) {
  auto model = _ui->importModules->model();
  // root is the "Import" item (non visible)
  auto root = model->index(0, 1);
  bool noFilter = filter.isEmpty();
  // Be warn, that we consider only two levels (groups, plugins of groups)
  // loop on groups (File, Graph ...)
  for (int i = 0; i < model->rowCount(root); ++i) {
    auto index = model->index(i, 1, root);
    auto name = index.data().toString();
    bool hidden = true;
    if (name.contains(filter, Qt::CaseInsensitive))
      hidden = false;
    // loop on current group import plugins
    for (int j = 0; j < model->rowCount(index); j++) {
      auto subIndex = model->index(j, 1, index);
      name = subIndex.data().toString();
      if (noFilter || name.contains(filter, Qt::CaseInsensitive))
        _ui->importModules->setRowHidden(j, index, hidden = false);
      else
        _ui->importModules->setRowHidden(j, index, true);
    }
    _ui->importModules->setRowHidden(i, root, hidden);
  }
}
