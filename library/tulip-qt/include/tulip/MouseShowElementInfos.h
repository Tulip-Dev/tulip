//-*-c++-*-
#ifndef _Tulip_MOUSESHOWELEMENTINFOS_H
#define _Tulip_MOUSESHOWELEMENTINFOS_H

#include <QtGui/QMouseEvent>

#include <tulip/GWInteractor.h>
#include <tulip/Graph.h>

#include "tulip/NodeLinkDiagramComponent.h"

/** \addtogroup Mouse_interactor */ 
/*@{*/

namespace tlp {

/** 
 * We define a specific interactor to show element graph infos in eltProperties
 */
class TLP_QT_SCOPE MouseShowElementInfos : public Interactor {
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
  Interactor *clone() { MouseShowElementInfos *interactor =new MouseShowElementInfos();return interactor;}
};

}

#endif
