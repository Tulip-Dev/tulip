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

using namespace std;

namespace tlp {

bool MouseShowElementInfos::eventFilter(QObject *widget, QEvent *e) {
  QMouseEvent *qMouseEv = (QMouseEvent *) e;
  if(e != NULL) {
    node tmpNode;
    edge tmpEdge;
    ElementType type;
    GlMainWidget *g = (GlMainWidget *) widget;
    if(e->type() == QEvent::MouseMove) {    
      if (g->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge)) {
        g->setCursor(Qt::WhatsThisCursor);
      }
      else {
        g->setCursor(QCursor());
      }
      return false;
    }
    else if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->button()==Qt::LeftButton) {
      NodeLinkDiagramComponent *nodeLinkView=(NodeLinkDiagramComponent *)view;
      if (g->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge)) {
        switch(type) {
        case NODE: nodeLinkView->elementSelectedSlot(tmpNode.id, true); break;
        case EDGE: nodeLinkView->elementSelectedSlot(tmpEdge.id, false); break;
        }
        return true;
      }
    }
  }
  
  return false;
}
  
InteractorComponent* MouseShowElementInfos::clone() {
  return new MouseShowElementInfos();
}  

}
