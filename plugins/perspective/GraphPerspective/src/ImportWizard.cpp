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
#include "ImportWizard.h"
#include "ui_ImportWizard.h"

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

  bool darkBackground =
      _ui->importModules->palette().color(backgroundRole()) != QColor(255, 255, 255);
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
  connect(_ui->importModules->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          this, SLOT(algorithmSelected(QModelIndex)));

  _ui->parameters->setItemDelegate(new TulipItemDelegate(_ui->parameters));
  _ui->parameters->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  connect(_ui->importModules, SIGNAL(doubleClicked(QModelIndex)), button(QWizard::FinishButton),
          SLOT(click()));
  // display OK instead of Finish
  setButtonText(QWizard::FinishButton, "OK");
  // Help button is used to display import plugin doc
  // as soon as an import plugin is selected
  auto helpButton = button(QWizard::HelpButton);
  helpButton->setVisible(false);
  connect(helpButton, SIGNAL(clicked(bool)), this, SLOT(helpButtonClicked()));

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

void ImportWizard::algorithmSelected(const QModelIndex &index) {
  QString alg(index.data().toString());
  string algs = tlp::QStringToTlpString(alg);
  _ui->parametersFrame->setVisible(!alg.isEmpty());
  QAbstractItemModel *oldModel = _ui->parameters->model();
  QAbstractItemModel *newModel = nullptr;
  bool isGroup = index.model()->index(0, index.column(), index).isValid();

  QString categoryText("<b>Category</b>");
  QString parametersText("<b>Parameters</b>");

  if (!isGroup && PluginLister::pluginExists(algs)) {
    _index = &index;
    newModel = new ParameterListModel(PluginLister::getPluginParameters(algs));
    parametersText += "&nbsp;<font size=-2>[" + alg + "]</font>";
    std::string group = PluginLister::pluginInformation(algs).group();

    if (!group.empty())
      categoryText += "&nbsp;<font size=-2>[" + tlpStringToQString(group) + "]</font>";

    setButtonText(QWizard::HelpButton, QString("%1 documentation").arg(alg));
    button(QWizard::HelpButton)->setVisible(true);

  } else {
    categoryText += "&nbsp;<font size=-2>[" + alg + "]</font>";
    button(QWizard::HelpButton)->setVisible(false);
  }

  _ui->categoryLabel->setText(categoryText);
  _ui->parametersLabel->setText(parametersText);

  _ui->parameters->setModel(newModel);

  delete oldModel;
  updateFinishButton();
}

QString ImportWizard::algorithm() const {
  if (_ui->importModules->selectionModel()->hasSelection())
    return _ui->importModules->selectionModel()->selectedIndexes()[0].data().toString();

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
  QMessageBox::information(this, _index->data().toString().append(" documentation"),
                           _index->data(Qt::ToolTipRole).toString());
}

void ImportWizard::updateFinishButton() {
  button(QWizard::FinishButton)->setEnabled(_ui->parameters->model() != nullptr);
}
