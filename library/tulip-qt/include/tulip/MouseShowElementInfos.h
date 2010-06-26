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
#ifndef _Tulip_MOUSESHOWELEMENTINFOS_H
#define _Tulip_MOUSESHOWELEMENTINFOS_H

#include <QtGui/QMouseEvent>

#include <tulip/InteractorComponent.h>
#include <tulip/Graph.h>

#include "tulip/GlMainWidget.h"
#include "tulip/NodeLinkDiagramComponent.h"

/** \addtogroup Mouse_interactor */
/*@{*/

namespace tlp {

/**
 * We define a specific interactor to show element graph infos in eltProperties
 */
class TLP_QT_SCOPE MouseShowElementInfos : public InteractorComponent {
public:
  ~MouseShowElementInfos(){}
  bool eventFilter(QObject *widget, QEvent *e) {
    if (e->type() == QEvent::MouseButtonPress &&
	((QMouseEvent *) e)->button()==Qt::LeftButton) {
      QMouseEvent *qMouseEv = (QMouseEvent *) e;
      GlMainWidget *g = (GlMainWidget *) widget;
      node tmpNode;
      edge tmpEdge;
      ElementType type;
      NodeLinkDiagramComponent *nodeLinkView=(NodeLinkDiagramComponent *)view;
      if (g->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge)) {
        switch(type) {
        case NODE: nodeLinkView->elementSelectedSlot(tmpNode.id, true); break;
        case EDGE: nodeLinkView->elementSelectedSlot(tmpEdge.id, false); break;
        }
        return true;
      }
    }
    return false;
  }
  InteractorComponent *clone() { MouseShowElementInfos *interactor =new MouseShowElementInfos();return interactor;}
};

}

#endif
