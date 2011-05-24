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
#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/NodeLinkDiagramComponent.h>


#include <tulip/MouseNodeBuilder.h>

using namespace tlp;
using namespace std;

bool MouseNodeBuilder::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent * qMouseEv = (QMouseEvent *) e;
  if(qMouseEv != NULL) {
    node tmpNode;
    edge tmpEdge;
    ElementType type;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if(e->type() == QEvent::MouseMove) {    
      if (glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge) && type == NODE) {
        glMainWidget->setCursor(Qt::ForbiddenCursor);
      }
      else {
        glMainWidget->setCursor(Qt::ArrowCursor);
      }
      return false;
    }
    if (e->type() == _eventType) {
      if (qMouseEv->button() == Qt::LeftButton) {
        if (glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge) && type == NODE) {
          return true;
        }
        Graph*_graph=glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
        LayoutProperty* mLayout=_graph->getProperty<LayoutProperty>(glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getElementLayoutPropName());
        // allow to undo
        _graph->push();
        Observable::holdObservers();
        node newNode;
        newNode = _graph->addNode();
        Coord point((double) glMainWidget->width() - (double) qMouseEv->x(),(double) qMouseEv->y(),0);
        point = glMainWidget->getScene()->getCamera().screenTo3DWorld(point);

        // This code is here to block z coordinate to 0 when we are in "2D mode"
        Coord cameraDirection=glMainWidget->getScene()->getCamera().getEyes()-glMainWidget->getScene()->getCamera().getCenter();
        if(cameraDirection[0]==0 && cameraDirection[1]==0)
          point[2]=0;

        mLayout->setNodeValue(newNode, point);
        Observable::unholdObservers();
        NodeLinkDiagramComponent *nodeLinkView=(NodeLinkDiagramComponent *)view;
        nodeLinkView->elementSelectedSlot(newNode.id, true);
        //glMainWidget->redraw();
        return true;
      }
    }
  }
  return false;
}
