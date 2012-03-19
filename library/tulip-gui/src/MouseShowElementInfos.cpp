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

#include <tulip/GlMainWidget.h>

using namespace std;
using namespace tlp;

MouseShowElementInfos::MouseShowElementInfos() {
  _informationsWidget=new QTableView();
  _informationsWidgetItem=new QGraphicsProxyWidget();
  _informationsWidgetItem->setWidget(_informationsWidget);
  _informationsWidgetItem->setVisible(false);
  _informationsWidgetItem->setMinimumSize(QSizeF(0,0));
}

void MouseShowElementInfos::clear() {
  _informationsWidgetItem->setVisible(false);
}

bool MouseShowElementInfos::eventFilter(QObject *widget, QEvent* e) {
  QMouseEvent * qMouseEv = (QMouseEvent *) e;

  SelectedEntity selectedEntity;
  GlMainWidget *glMainWidget = (GlMainWidget *) widget;

  if(qMouseEv != NULL) {
    if (e->type() == QEvent::MouseButtonPress) {
      if (qMouseEv->button() == Qt::LeftButton) {
        if(_informationsWidgetItem->isVisible()) {
          // Hide widget if we click outside it
          if(!_informationsWidgetItem->boundingRect().contains(qMouseEv->pos())) {
            _informationsWidgetItem->setVisible(false);
          }
        }

        if(!_informationsWidgetItem->isVisible()) {
          // Show widget if we click on node or edge
          if (glMainWidget->pickNodesEdges(qMouseEv->x(), qMouseEv->y(), selectedEntity)) {
            if(selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED ||
                selectedEntity.getEntityType() == SelectedEntity::EDGE_SELECTED) {
              _informationsWidgetItem->setPos(qMouseEv->pos());
              _informationsWidgetItem->setVisible(true);
              QPropertyAnimation *animation = new QPropertyAnimation(_informationsWidgetItem, "size");
              animation->setDuration(100);
              animation->setStartValue(QSizeF(0,0));
              animation->setEndValue(_informationsWidgetItem->preferredSize());

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

void MouseShowElementInfos::viewChanged(View * view) {
  ViewWidget *viewWidget=dynamic_cast<ViewWidget*>(view);
  assert(viewWidget);
  _view=viewWidget;
  _view->graphicsView()->scene()->addItem(_informationsWidgetItem);
}
