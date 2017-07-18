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
#include "ui_ElementInformationWidget.h"

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
#include <tulip/MouseShowElementInfo.h>
#include <tulip/TulipMetaTypes.h>

using namespace std;
using namespace tlp;

MouseShowElementInfo::MouseShowElementInfo():_ui(new Ui::ElementInformationWidget),_informationWidget(new QWidget()),_informationWidgetItem(new QGraphicsProxyWidget()), glMainWidget(NULL),_show(true) {
  _informationWidget->installEventFilter(this);
  _ui->setupUi(_informationWidget);
  tableView()->setItemDelegate(new TulipItemDelegate(tableView()));
  _informationWidgetItem->setWidget(_informationWidget);
  _informationWidgetItem->setVisible(false);
  connect(_ui->displayTulipProp, SIGNAL(stateChanged(int)), this, SLOT(showVisualProp(int)));
}

MouseShowElementInfo::~MouseShowElementInfo() {
  delete _ui;
}

void MouseShowElementInfo::showVisualProp(int show) {
        _show = show == Qt::Checked;
    static_cast<GraphElementModel*>(tableView()->model())->setShowVisualProp(_show);

}

void MouseShowElementInfo::clear() {
  _informationWidgetItem->setVisible(false);

  if (glMainWidget)
    glMainWidget->setCursor(QCursor());
}

QTableView* MouseShowElementInfo::tableView() const {
  return _informationWidget->findChild<QTableView*>();
}

bool MouseShowElementInfo::eventFilter(QObject *widget, QEvent* e) {
  if (widget == _informationWidget && (e->type() == QEvent::Wheel || e->type() == QEvent::MouseButtonPress))
    return true;

  if(_informationWidget->isVisible() && e->type()==QEvent::Wheel) {
    _informationWidgetItem->setVisible(false);
    return false;
  }

  QMouseEvent * qMouseEv = dynamic_cast<QMouseEvent *>(e);

  if(qMouseEv != NULL) {
    if (glMainWidget == NULL)
      glMainWidget=dynamic_cast<GlMainWidget*>(widget);

    assert(glMainWidget);

    SelectedEntity selectedEntity;

    if(e->type() == QEvent::MouseMove) {
      if (pick(qMouseEv->x(), qMouseEv->y(),selectedEntity)) {
        glMainWidget->setCursor(Qt::WhatsThisCursor);
      }
      else {
        glMainWidget->setCursor(QCursor());
      }

      return false;
    }
    else if (e->type() == QEvent::MouseButtonPress && qMouseEv->button() == Qt::LeftButton) {
      if(_informationWidgetItem->isVisible()) {
        // Hide widget if we click outside it
        _informationWidgetItem->setVisible(false);
      }

      if(!_informationWidgetItem->isVisible()) {

        // Show widget if we click on node or edge
        if (pick(qMouseEv->x(), qMouseEv->y(),selectedEntity)) {
          if(selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED ||
              selectedEntity.getEntityType() == SelectedEntity::EDGE_SELECTED) {

            QLabel* title = _informationWidget->findChild<QLabel*>();

            ElementType eltType = selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED?NODE:EDGE;

            tableView()->setModel(buildModel(eltType,selectedEntity.getComplexEntityId(),_informationWidget));
            title->setText(elementName(eltType,selectedEntity.getComplexEntityId()));

            QPoint position=qMouseEv->pos();

            if(position.x()+_informationWidgetItem->rect().width()>_view->graphicsView()->sceneRect().width() - 5)
              position.setX(_view->graphicsView()->sceneRect().width()-_informationWidgetItem->rect().width() - 5);

            if(position.y()+_informationWidgetItem->rect().height()>_view->graphicsView()->sceneRect().height() - 5)
              position.setY(_view->graphicsView()->sceneRect().height()-_informationWidgetItem->rect().height() - 5);

            _informationWidgetItem->setPos(position);
            _informationWidgetItem->setVisible(true);
            QPropertyAnimation *animation = new QPropertyAnimation(_informationWidgetItem, "opacity");
            animation->setDuration(100);
            animation->setStartValue(0.);
            animation->setEndValue(1);
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

bool MouseShowElementInfo::pick(int x, int y, SelectedEntity &selectedEntity) {
  assert(glMainWidget);
  return glMainWidget->pickNodesEdges(x,y,selectedEntity);
}

void MouseShowElementInfo::viewChanged(View * view) {
  if (view == NULL) {
    _view = NULL;
    return;
  }

  ViewWidget *viewWidget=dynamic_cast<ViewWidget*>(view);
  assert(viewWidget);
  _view=viewWidget;
  connect(_view,SIGNAL(graphSet(tlp::Graph*)),_informationWidgetItem,SLOT(close()));
  _view->graphicsView()->scene()->addItem(_informationWidgetItem);
}

QAbstractItemModel* MouseShowElementInfo::buildModel(ElementType elementType,unsigned int elementId,QObject* parent)const {
  if(elementType == NODE) {
    return new GraphNodeElementModel(view()->graph(),elementId,parent,_show);
  }
  else {
    return new GraphEdgeElementModel(view()->graph(),elementId,parent,_show);
  }
}

QString MouseShowElementInfo::elementName(ElementType elementType, unsigned int elementId)const {
  QString elementTypeLabel = elementType==NODE?QString("Node"):QString("Edge");
  return elementTypeLabel +" #" + QString::number(elementId);
}
