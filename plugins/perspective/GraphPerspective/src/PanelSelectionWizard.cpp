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
#include "PanelSelectionWizard.h"
#include "ui_PanelSelectionWizard.h"
#include "ui_PanelSelectionItem.h"

#include <QtCore/QDebug>
#include <QtGui/QAbstractButton>

#include <tulip/PluginManager.h>
#include <tulip/View.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/GraphHierarchiesModel.h>

using namespace tlp;

/*
  Helper class
  */
PanelSelectionItem::PanelSelectionItem(tlp::PluginInformations* infos, QWidget *parent): QWidget(parent), _ui(new Ui::PanelSelectionItem) {
  _ui->setupUi(this);
  _viewName = infos->name();
  _ui->icon->setPixmap(QPixmap(infos->iconPath()));
  _ui->name->setText("<p><span style=\"font-size:large;\"><b>"
                     +
                     infos->name()
                     +
                     "</b></span></p>");
  _ui->description->setText("<p><span style=\"color:#626262;\">"
                            +
                            infos->shortDescription()
                            +
                            "</span></p>");
}

PanelSelectionItem::~PanelSelectionItem() {
  delete _ui;
}

QString PanelSelectionItem::viewName() const {
  return _viewName;
}

void PanelSelectionItem::focusInEvent(QFocusEvent *) {
  emit selected();
}

void PanelSelectionItem::setFocus(bool f) {
  QString background = (f ? "qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(243, 249, 255, 255), stop:1 rgba(232, 238, 244, 255));" : "white;");
  _ui->frame->setStyleSheet("#frame { background-color: " + background + "border: 1px solid #C9C9C9; }");
}

void PanelSelectionItem::mouseDoubleClickEvent(QMouseEvent *) {
  emit doubleClicked();
}

// ************************************************

PanelSelectionWizard::PanelSelectionWizard(GraphHierarchiesModel* model, QWidget *parent, bool canSelectGraph)
  : QWizard(parent), _ui(new Ui::PanelSelectionWizard),
    _model(model), _activeItem(NULL), _canSelectGraph(canSelectGraph), _view(NULL) {
  _ui->setupUi(this);
  _ui->selectGraphFrame->setVisible(_canSelectGraph);

  if (_canSelectGraph) {
    _ui->graphCombo->setModel(_model);
    _ui->graphCombo->selectIndex(_model->indexOf(_model->currentGraph()));
  }

  QVBoxLayout *panelsLayout = new QVBoxLayout;
  panelsLayout->setContentsMargins(6,6,6,6);
  QList<tlp::PluginInformations *> localPlugins = tlp::PluginManager::pluginsList(tlp::PluginManager::Local);
  tlp::PluginInformations *info;
  int i=0;
  foreach(info,localPlugins) {
    if (info->type() != "View")
      continue;

    PanelSelectionItem* item = new PanelSelectionItem(info);
    panelsLayout->addWidget(item);
    connect(item,SIGNAL(selected()),this,SLOT(panelSelected()));
    connect(item,SIGNAL(doubleClicked()),this,SLOT(panelDoubleClicked()));
    connect(item,SIGNAL(doubleClicked()),this,SIGNAL(wizardFinished()));

    if (i++ == 0) {
      _activeItem = item;
      _activeItem->setFocus(true);
    }
  }
  panelsLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Maximum,QSizePolicy::Expanding));
  _ui->panelSelector->setLayout(panelsLayout);

  button(QWizard::FinishButton)->setEnabled(false);
}

PanelSelectionWizard::~PanelSelectionWizard() {
  delete _ui;
}

void PanelSelectionWizard::panelSelected() {
  if (_activeItem != NULL)
    _activeItem->setFocus(false);

  _activeItem = static_cast<PanelSelectionItem*>(sender());
  _activeItem->setFocus(true);

  button(QWizard::NextButton)->setEnabled(true);
  connect(button(QWizard::NextButton),SIGNAL(clicked()),this,SLOT(nextButtonClicked()));
}

void PanelSelectionWizard::panelDoubleClicked() {
  createView();
  button(QWizard::FinishButton)->click();
}

tlp::Graph* PanelSelectionWizard::graph() const {
  return _model->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
}

QString PanelSelectionWizard::panelName() const {
  if (_activeItem)
    return _activeItem->viewName();
  else
    return QString::null;
}

void PanelSelectionWizard::setSelectedGraph(tlp::Graph* g) {
  if (!_canSelectGraph)
    return;

  _ui->graphCombo->selectIndex(_model->indexOf(g));
}

void PanelSelectionWizard::nextButtonClicked() {
  createView();
  disconnect(button(QWizard::NextButton),SIGNAL(clicked()),this,SLOT(nextButtonClicked()));
  foreach(QWidget* w, _view->configurationWidgets()) {
    QWizardPage* page = new QWizardPage();
    page->setLayout(new QVBoxLayout);
    page->layout()->setMargin(0);
    page->layout()->addWidget(w);
    addPage(page);
  }
  connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(pageChanged(int)));
  next();
}

void PanelSelectionWizard::createView() {
  _view = ViewLister::getPluginObject(panelName().toStdString(),NULL);
  _view->setupUi();
  _view->setGraph(graph());
  _view->setState(DataSet());
}

void PanelSelectionWizard::pageChanged(int id) {
  if (id == startId()) {
    foreach(int pageId, pageIds()) {
      if (pageId == startId())
        continue;
      removePage(pageId);
    }
    button(QWizard::NextButton)->setEnabled(true);
    connect(button(QWizard::NextButton),SIGNAL(clicked()),this,SLOT(nextButtonClicked()));
    disconnect(this,SIGNAL(currentIdChanged(int)),this,SLOT(pageChanged(int)));
    delete _view;
    _view = NULL;
  }
}

tlp::View* PanelSelectionWizard::panel() const {
  return _view;
}
