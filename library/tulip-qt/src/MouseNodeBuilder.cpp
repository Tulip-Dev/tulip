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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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
  if (e->type() == _eventType) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    if (qMouseEv->button() == Qt::LeftButton) {
      GlMainWidget *glw = (GlMainWidget *) widget;

      Graph*_graph=glw->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      LayoutProperty* mLayout=_graph->getProperty<LayoutProperty>(glw->getScene()->getGlGraphComposite()->getInputData()->getElementLayoutPropName());
      // allow to undo
      _graph->push();
      Observable::holdObservers();
      node newNode;
      newNode = _graph->addNode();
      Coord point((double) glw->width() - (double) qMouseEv->x(),
		  (double) qMouseEv->y(),
		  0);
      point = glw->getScene()->getCamera()->screenTo3DWorld(point);
      mLayout->setNodeValue(newNode, point);
      Observable::unholdObservers();
      NodeLinkDiagramComponent *nodeLinkView=(NodeLinkDiagramComponent *)view;
      nodeLinkView->elementSelectedSlot(newNode.id, true);
      //glw->redraw();
      return true;
    }
  }
  return false;
}
