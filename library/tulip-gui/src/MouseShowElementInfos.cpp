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
#include "ui_ElementInformationsWidget.h"

#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QHeaderView>
#include <QMouseEvent>
#include <QGraphicsProxyWidget>

#include <tulip/GraphElementModel.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlScene.h>
#include <tulip/MouseShowElementInfos.h>
#include <tulip/TulipMetaTypes.h>

using namespace std;
using namespace tlp;

MouseShowElementInfos::MouseShowElementInfos():_ui(new Ui::ElementInformationsWidget),_informationsWidget(new QWidget()),_informationsWidgetItem(new QGraphicsProxyWidget()) {
  _informationsWidget->installEventFilter(this);
  _ui->setupUi(_informationsWidget);
  tableView()->setItemDelegate(new TulipItemDelegate);
  _informationsWidgetItem->setWidget(_informationsWidget);
  _informationsWidgetItem->setVisible(false);
}

MouseShowElementInfos::~MouseShowElementInfos() {
  delete _ui;
}

void MouseShowElementInfos::clear() {
  _informationsWidgetItem->setVisible(false);
  GlMainView *glMainView=dynamic_cast<GlMainView*>(view());
  glMainView->getGlMainWidget()->setCursor(QCursor());
}

QTableView* MouseShowElementInfos::tableView() const {
  return _informationsWidget->findChild<QTableView*>();
}

bool MouseShowElementInfos::eventFilter(QObject *widget, QEvent* e) {
  if (widget == _informationsWidget && (e->type() == QEvent::Wheel || e->type() == QEvent::MouseButtonPress))
    return true;

  if(_informationsWidget->isVisible() && e->type()==QEvent::Wheel) {
    _informationsWidgetItem->setVisible(false);
    return false;
  }

  QMouseEvent * qMouseEv = dynamic_cast<QMouseEvent *>(e);

  if(qMouseEv != NULL) {
    GlMainView *glMainView=dynamic_cast<GlMainView*>(view());
    assert(glMainView);
    SelectedEntity selectedEntity;

    if(e->type() == QEvent::MouseMove) {
      if (pick(qMouseEv->x(), qMouseEv->y(),selectedEntity)) {
        glMainView->getGlMainWidget()->setCursor(Qt::WhatsThisCursor);
      }
      else {
        glMainView->getGlMainWidget()->setCursor(QCursor());
      }

      return false;
    }
    else if (e->type() == QEvent::MouseButtonPress && qMouseEv->button() == Qt::LeftButton) {
      if(_informationsWidgetItem->isVisible()) {
        // Hide widget if we click outside it
        _informationsWidgetItem->setVisible(false);
      }

      if(!_informationsWidgetItem->isVisible()) {

        // Show widget if we click on node or edge
        if (pick(qMouseEv->x(), qMouseEv->y(),selectedEntity)) {
          if(selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED ||
              selectedEntity.getEntityType() == SelectedEntity::EDGE_SELECTED) {

            QLabel* title = _informationsWidget->findChild<QLabel*>();

            ElementType eltType = selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED?NODE:EDGE;

            tableView()->setModel(buildModel(eltType,selectedEntity.getComplexEntityId(),_informationsWidget));
            title->setText(elementName(eltType,selectedEntity.getComplexEntityId()));

            QPoint position=qMouseEv->pos();

            if(position.x()+_informationsWidgetItem->rect().width()>_view->graphicsView()->sceneRect().width())
              position.setX(qMouseEv->pos().x()-_informationsWidgetItem->rect().width());

            if(position.y()+_informationsWidgetItem->rect().height()>_view->graphicsView()->sceneRect().height())
              position.setY(qMouseEv->pos().y()-_informationsWidgetItem->rect().height());

            _informationsWidgetItem->setPos(position);
            _informationsWidgetItem->setVisible(true);
            QPropertyAnimation *animation = new QPropertyAnimation(_informationsWidgetItem, "opacity");
            animation->setDuration(100);
            animation->setStartValue(0.);
            animation->setEndValue(0.99);
            animation->start();

            return true;
          }
          else {
            return false;
          }
        }
      }
    }
  }

  return false;
}

bool MouseShowElementInfos::pick(int x, int y, SelectedEntity &selectedEntity) {
  GlMainView *glMainView=dynamic_cast<GlMainView*>(view());
  assert(glMainView);
  return glMainView->getGlMainWidget()->pickNodesEdges(x,y,selectedEntity);
}

void MouseShowElementInfos::viewChanged(View * view) {
  if (view == NULL) {
    _view = NULL;
    return;
  }

  ViewWidget *viewWidget=dynamic_cast<ViewWidget*>(view);
  assert(viewWidget);
  _view=viewWidget;
  connect(_view,SIGNAL(graphSet(tlp::Graph*)),_informationsWidgetItem,SLOT(close()));
  _view->graphicsView()->scene()->addItem(_informationsWidgetItem);
}

QAbstractItemModel* MouseShowElementInfos::buildModel(ElementType elementType,unsigned int elementId,QObject* parent)const {
  if(elementType == NODE) {
    return new GraphNodeElementModel(view()->graph(),elementId,parent);
  }
  else {
    return new GraphEdgeElementModel(view()->graph(),elementId,parent);
  }
}

QString MouseShowElementInfos::elementName(ElementType elementType, unsigned int elementId)const {
  QString elementTypeLabel = elementType==NODE?QString("Node"):QString("Edge");
  return elementTypeLabel +" #" + QString::number(elementId);
}
