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

#include <QtGui/QAbstractButton>
#include <QtGui/QMouseEvent>

#include <tulip/PluginManager.h>
#include <tulip/View.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/GraphHierarchiesModel.h>

using namespace tlp;

PanelSelectionItem::PanelSelectionItem(tlp::PluginInformations* infos, QWidget *parent): QWidget(parent), _ui(new Ui::PanelSelectionItem) {
  _ui->setupUi(this);
  _viewName = infos->name();
  _ui->icon->setPixmap(QPixmap(infos->iconPath()));
  _ui->name->setText("<p><span style=\"font-size:large;\"><b>" + infos->name() + "</b></span></p>");
  _ui->description->setText("<p><span style=\"color:#626262;\">" + infos->shortDescription() + "</span></p>");
}
PanelSelectionItem::~PanelSelectionItem() {
  delete _ui;
}
QString PanelSelectionItem::viewName() const {
  return _viewName;
}
void PanelSelectionItem::setFocus(bool f) {
  QString background = (f ? "qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(233, 239, 245, 255), stop:1 rgba(222, 228, 234, 255));" : "white;");
  _ui->frame->setStyleSheet("#frame { background-color: " + background + "border: 1px solid #C9C9C9; }");
  emit focused(f);
}
void PanelSelectionItem::mousePressEvent(QMouseEvent*) {
  setFocus(true);
}
void PanelSelectionItem::mouseDoubleClickEvent(QMouseEvent*) {
  emit selected();
}
// ************************************************
PanelSelectionWizard::PanelSelectionWizard(GraphHierarchiesModel* model, QWidget *parent)
  : QWizard(parent), _ui(new Ui::PanelSelectionWizard), _model(model), _view(NULL), _currentItem(NULL) {
  _ui->setupUi(this);
  connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(pageChanged(int)));
  _ui->graphCombo->setModel(_model);
  _ui->graphCombo->selectIndex(_model->indexOf(_model->currentGraph()));

  PanelSelectionItem* firstItem = NULL;
  QVBoxLayout *panelsLayout = new QVBoxLayout;
  panelsLayout->setContentsMargins(6,6,6,6);
  QList<tlp::PluginInformations *> localPlugins = tlp::PluginManager::pluginsList(tlp::PluginManager::Local);
  tlp::PluginInformations *info;
  foreach(info,localPlugins) {
    if (info->type() != "View")
      continue;

    PanelSelectionItem* item = new PanelSelectionItem(info);
    connect(item,SIGNAL(focused(bool)),this,SLOT(panelFocused(bool)));
    connect(item,SIGNAL(selected()),button(QWizard::FinishButton),SLOT(click()));
    panelsLayout->addWidget(item);
    if (firstItem == NULL)
      firstItem = item;
  }
  firstItem->setFocus(true);

  panelsLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Maximum,QSizePolicy::Expanding));
  _ui->panelSelector->setLayout(panelsLayout);
}

PanelSelectionWizard::~PanelSelectionWizard() {
  delete _ui;
}

tlp::Graph* PanelSelectionWizard::graph() const {
  return _model->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
}

QString PanelSelectionWizard::panelName() const {
  return _currentItem->viewName();
}

void PanelSelectionWizard::setSelectedGraph(tlp::Graph* g) {
  _ui->graphCombo->selectIndex(_model->indexOf(g));
}

tlp::View* PanelSelectionWizard::panel() const {
  return _view;
}

QList<PanelSelectionItem*> PanelSelectionWizard::items() const {
  return findChildren<PanelSelectionItem*>();
}

void PanelSelectionWizard::createView() {
  _view = ViewLister::getPluginObject(panelName().toStdString(),NULL);
  _view->setupUi();
  _view->setGraph(graph());
  _view->setState(DataSet());
}

void PanelSelectionWizard::clearView() {
  delete _view;
  _view = NULL;

  foreach(int id, pageIds()) {
    if (id == startId())
      continue;
    QWizardPage* p = page(id);
    removePage(id);
    delete p;
  }
  _ui->placeHolder = new QWizardPage();
  addPage(_ui->placeHolder);
}

void PanelSelectionWizard::done(int result) {
  if (result == QDialog::Accepted && _view != NULL) {
    _view->applySettings();
  }
  else if (result == QDialog::Accepted && _view == NULL) {
    createView();
  }
  else if (result != QDialog::Accepted) {
    clearView();
  }
  QWizard::done(result);
}

void PanelSelectionWizard::panelFocused(bool f) {
  if (!f)
    return;
  _currentItem = static_cast<PanelSelectionItem*>(sender());
  foreach(PanelSelectionItem* i, items()) {
    if (i != _currentItem)
      i->setFocus(false);
  }
}

void PanelSelectionWizard::pageChanged(int id) {
  if (id == startId()) {
    clearView();
    button(QWizard::FinishButton)->setEnabled(true);
    button(QWizard::NextButton)->setEnabled(true);
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
}
