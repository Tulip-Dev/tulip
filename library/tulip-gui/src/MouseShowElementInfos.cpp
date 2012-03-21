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
#include "tulip/MouseShowElementInfos.h"

#include <QtCore/QPropertyAnimation>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <tulip/GraphElementModel.h>
#include <tulip/TulipItemDelegate.h>

#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>

using namespace std;
using namespace tlp;

MouseShowElementInfos::MouseShowElementInfos() {
  _informationsWidget=new QTableView();
  _informationsWidget->setModel(NULL);
  _informationsWidgetItem=new QGraphicsProxyWidget();
  _informationsWidgetItem->setWidget(_informationsWidget);
  _informationsWidgetItem->setVisible(false);
  _informationsWidgetItem->setMinimumSize(QSizeF(0,0));
  _informationsWidgetItem->setPreferredWidth(300);
}

void MouseShowElementInfos::clear() {
  _informationsWidgetItem->setVisible(false);
}

bool MouseShowElementInfos::eventFilter(QObject *widget, QEvent* e) {
  QMouseEvent * qMouseEv = (QMouseEvent *) e;


  GlMainWidget *glMainWidget = (GlMainWidget *) widget;

  if(qMouseEv != NULL) {
    if (e->type() == QEvent::MouseButtonPress) {
      if (qMouseEv->button() == Qt::LeftButton) {
        if(_informationsWidgetItem->isVisible()) {
          // Hide widget if we click outside it
          _informationsWidgetItem->setVisible(false);
        }

        if(!_informationsWidgetItem->isVisible()) {
          SelectedEntity selectedEntity;

          // Show widget if we click on node or edge
          if (pick(qMouseEv->x(), qMouseEv->y(),selectedEntity)) {
            if(selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED ||
                selectedEntity.getEntityType() == SelectedEntity::EDGE_SELECTED) {
              _informationsWidgetItem->setVisible(true);

              if(_informationsWidget->model()!=NULL)
                delete _informationsWidget->model();

              if(selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED)
                _informationsWidget->setModel(new GraphNodeElementModel(_view->graph(),selectedEntity.getComplexEntityId(),_informationsWidget));
              else
                _informationsWidget->setModel(new GraphEdgeElementModel(_view->graph(),selectedEntity.getComplexEntityId(),_informationsWidget));

              _informationsWidget->setItemDelegate(new TulipItemDelegate);
              _informationsWidget->resizeColumnsToContents();
              _informationsWidgetItem->adjustSize();
              QPoint position=qMouseEv->pos();

              if(position.x()+_informationsWidgetItem->rect().width()>_view->graphicsView()->sceneRect().width())
                position.setX(qMouseEv->pos().x()-_informationsWidgetItem->rect().width());

              if(position.y()+_informationsWidgetItem->rect().height()>_view->graphicsView()->sceneRect().height())
                position.setY(qMouseEv->pos().y()-_informationsWidgetItem->rect().height());

              _informationsWidgetItem->setPos(position);
              _informationsWidget->setColumnWidth(0,125);
              QPropertyAnimation *animation = new QPropertyAnimation(_informationsWidgetItem, "opacity");
              animation->setDuration(100);
              animation->setStartValue(0.);
              animation->setEndValue(1.);
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
  }

  return false;
}

bool MouseShowElementInfos::pick(int x, int y, SelectedEntity &selectedEntity) {
  GlMainView *glMainView=dynamic_cast<GlMainView*>(view());
  assert(glMainView);
  return glMainView->getGlMainWidget()->pickNodesEdges(x,y,selectedEntity);
}

void MouseShowElementInfos::viewChanged(View * view) {
  ViewWidget *viewWidget=dynamic_cast<ViewWidget*>(view);
  assert(viewWidget);
  _view=viewWidget;
  _view->graphicsView()->scene()->addItem(_informationsWidgetItem);
}
