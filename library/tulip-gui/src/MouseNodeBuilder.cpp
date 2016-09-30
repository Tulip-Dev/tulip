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

#include "tulip/MouseNodeBuilder.h"

#include <QMouseEvent>

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlGraph.h>
#include <tulip/GlMainView.h>
#include <tulip/Camera.h>
#include <tulip/GlLayer.h>

using namespace tlp;
using namespace std;

bool MouseNodeBuilder::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);

  if (qMouseEv != nullptr) {
    SelectedEntity selectedEntity;
    GlMainWidget *glMainWidget = static_cast<GlMainWidget *>(widget);

    if (e->type() == QEvent::MouseMove) {
      if (glMainWidget->pickNodesEdges(qMouseEv->x(), glMainWidget->height() - qMouseEv->y(), selectedEntity) &&
          selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
        glMainWidget->setCursor(Qt::ForbiddenCursor);
      } else {
        glMainWidget->setCursor(Qt::ArrowCursor);
      }

      return false;
    }

    if (e->type() == _eventType) {
      if (qMouseEv->button() == Qt::LeftButton) {
        if (glMainWidget->pickNodesEdges(qMouseEv->x(), glMainWidget->height() - qMouseEv->y(), selectedEntity) &&
            selectedEntity.getEntityType() == SelectedEntity::NODE_SELECTED) {
          return true;
        }

        GlGraphInputData *inputData = &(glMainWidget->getScene()->getMainGlGraph()->getInputData());
        Graph *_graph = inputData->getGraph();
        LayoutProperty *mLayout = inputData->getElementLayout();
        // allow to undo
        _graph->push();
        Observable::holdObservers();
        node newNode;
        newNode = _graph->addNode();
        Coord point(glMainWidget->width() - qMouseEv->x(), qMouseEv->y(), 0);
        point = glMainWidget->getScene()->getMainLayer()->getCamera()->viewportTo3DWorld(glMainWidget->screenToViewport(point));

        // This code is here to block z coordinate to 0 when we are in "2D mode"
        Coord cameraDirection =
            glMainWidget->getScene()->getMainLayer()->getCamera()->getEyes() - glMainWidget->getScene()->getMainLayer()->getCamera()->getCenter();

        if (cameraDirection[0] == 0 && cameraDirection[1] == 0)
          point[2] = 0;

        mLayout->setNodeValue(newNode, point);
        Observable::unholdObservers();

        return true;
      }
    }
  }

  return false;
}

void MouseNodeBuilder::clear() {
  GlMainView *glMainView = dynamic_cast<GlMainView *>(view());
  glMainView->getGlMainWidget()->setCursor(QCursor());
}
