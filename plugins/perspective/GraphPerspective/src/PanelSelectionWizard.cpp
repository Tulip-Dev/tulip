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
#include "PanelSelectionWizard.h"
#include "ui_PanelSelectionWizard.h"

#include <QAbstractButton>
#include <QMouseEvent>

#include <tulip/PluginManager.h>
#include <tulip/View.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PluginModel.h>

using namespace tlp;
using namespace std;

PanelSelectionWizard::PanelSelectionWizard(GraphHierarchiesModel* model, QWidget *parent)
  : QWizard(parent), _ui(new Ui::PanelSelectionWizard), _model(model), _view(NULL), _currentItem(QString::null) {
  _ui->setupUi(this);
  connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(pageChanged(int)));
  _ui->graphCombo->setModel(_model);
  _ui->graphCombo->selectIndex(_model->indexOf(_model->currentGraph()));

  _ui->panelList->setModel(new SimplePluginListModel(QList<string>::fromStdList(PluginLister::instance()->availablePlugins<tlp::View>()),_ui->panelList));
  connect(_ui->panelList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(panelSelected(QModelIndex)));
  connect(_ui->panelList,SIGNAL(doubleClicked(QModelIndex)),button(QWizard::FinishButton),SLOT(click()));
  _ui->panelList->setCurrentIndex(_ui->panelList->model()->index(0,0));
}

PanelSelectionWizard::~PanelSelectionWizard() {
  delete _ui;
}

void PanelSelectionWizard::panelSelected (const QModelIndex& index) {
  _currentItem = index.data().toString();
  _ui->panelDescription->setHtml(PluginLister::pluginInformation(_currentItem.toStdString()).info().c_str());
  // NexButton is temporarily hidden
  // QWizard::HaveNextButtonOnLastPage has been removed
  // from options property in PanelSelectionWizard.ui
  button(QWizard::NextButton)->setEnabled(true);
}

tlp::Graph* PanelSelectionWizard::graph() const {
  return _model->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
}

void PanelSelectionWizard::setSelectedGraph(tlp::Graph* g) {
  _ui->graphCombo->selectIndex(_model->indexOf(g));
}

tlp::View* PanelSelectionWizard::panel() const {
  return _view;
}

void PanelSelectionWizard::createView() {
  _view = PluginLister::instance()->getPluginObject<View>(_currentItem.toStdString(),NULL);
  _view->setupUi();
  _view->setGraph(graph());
  _view->setState(DataSet());
}

void PanelSelectionWizard::clearView() {
  delete _view;
  _view = NULL;

  foreach(int id, pageIds()) {
    if (id == startId() || id == currentId())
      continue;

    QWizardPage* p = page(id);
    removePage(id);
    delete p;
  }
  _ui->placeHolder = new QWizardPage();
  addPage(_ui->placeHolder);
}

void PanelSelectionWizard::done(int result) {
  if (result == QDialog::Accepted && _view == NULL) {
    createView();
  }
  else if (result == QDialog::Rejected) {
    clearView();
  }

  QWizard::done(result);
}

void PanelSelectionWizard::pageChanged(int id) {
  // temporarily display OK instead of Finish
  setButtonText(QWizard::FinishButton, "OK");

  if (id == startId()) {
    clearView();
    button(QWizard::FinishButton)->setEnabled(true);
  }

  if (page(id) == _ui->placeHolder) {
    createView();
    bool inPlaceHolder = true;
    foreach(QWidget* w, _view->configurationWidgets()) {
      QWizardPage* p;

      if (inPlaceHolder) {
        p = _ui->placeHolder;
        inPlaceHolder = false;
      }
      else {
        p = new QWizardPage;
        addPage(p);
      }

      p->setLayout(new QVBoxLayout);
      p->layout()->addWidget(w);
    }
  }

  // NexButton is temporarily hidden
  // QWizard::HaveNextButtonOnLastPage has been removed
  // from options property in PanelSelectionWizard.ui
  button(QWizard::NextButton)->setEnabled(nextId()!=-1);
}
