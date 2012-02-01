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
void PanelSelectionItem::setSelected(bool f) {
  QString background = (f ? "qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(233, 239, 245, 255), stop:1 rgba(222, 228, 234, 255));" : "white;");
  _ui->frame->setStyleSheet("#frame { background-color: " + background + "border: 1px solid #C9C9C9; }");
  emit selected(f);
}
void PanelSelectionItem::mousePressEvent(QMouseEvent* ev) {
  setSelected(true);
}
void PanelSelectionItem::mouseDoubleClickEvent(QMouseEvent* ev) {
  emit opened();
}

// ************************************************
PanelSelectionWizard::PanelSelectionWizard(GraphHierarchiesModel* model, QWidget *parent)
  : QWizard(parent), _ui(new Ui::PanelSelectionWizard), _model(model), _view(NULL), _currentItem(NULL) {
  _ui->setupUi(this);
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
    connect(item,SIGNAL(selected(bool)),this,SLOT(panelSelected(bool)));
    connect(item,SIGNAL(opened()),this,SLOT(panelOpened()));
    panelsLayout->addWidget(item);

    if (firstItem == NULL) {
      firstItem = item;
    }
  }

  QMouseEvent* event = new QMouseEvent(QMouseEvent::MouseButtonPress,QPoint(5,5),Qt::LeftButton,Qt::NoButton,Qt::NoModifier);

  if (firstItem != NULL)
    qApp->postEvent(firstItem,event);

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

void PanelSelectionWizard::createView() {
  delete _view;

  _view = ViewLister::getPluginObject(panelName().toStdString(),NULL);
  _view->setupUi();
  _view->setGraph(graph());
  _view->setState(DataSet());
}

tlp::View* PanelSelectionWizard::panel() const {
  return _view;
}

void PanelSelectionWizard::done(int result) {
  if (result == QDialog::Accepted && _view != NULL) {
    _view->applySettings();
  }
  else if (result == QDialog::Accepted && _view == NULL) {
    createView();
  }
  else if (result != QDialog::Accepted) {
    delete _view;
    _view = NULL;

    foreach(int id, pageIds()) {
      if (id == startId())
        continue;

      QWizardPage* p = page(id);
      removePage(id);
      delete p;
    }
  }

  QWizard::done(result);
}

void PanelSelectionWizard::panelSelected(bool f) {
  PanelSelectionItem* item = static_cast<PanelSelectionItem*>(sender());

  if (_currentItem == item)
    return;

  if (f) {
    if (_currentItem != NULL)
      _currentItem->setSelected(false);

    _currentItem = item;
    createView();

    foreach(int id, pageIds()) {
      if (id == startId())
        continue;

      QWizardPage* p = page(id);
      removePage(id);
      delete p;
    }

    foreach(QWidget* w, _view->configurationWidgets()) {
      QWizardPage* p = new QWizardPage();
      p->setLayout(new QVBoxLayout);
      p->layout()->addWidget(w);
      addPage(p);
    }
    button(QWizard::NextButton)->setEnabled(_view->configurationWidgets().size()>0);
    validateCurrentPage();
  }
}

void PanelSelectionWizard::panelOpened() {
  PanelSelectionItem* item = static_cast<PanelSelectionItem*>(sender());

  if (_currentItem != item) {
    _currentItem = item;
    createView();
  }

  button(QWizard::FinishButton)->click();
}
