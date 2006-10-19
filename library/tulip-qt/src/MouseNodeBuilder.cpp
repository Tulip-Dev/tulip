#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if (QT_REL == 3)
#include <qevent.h>
#else
#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>
#include "tulip/Qt3ForTulip.h"
#endif

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlGraphWidget.h>


#include <tulip/MouseNodeBuilder.h>

using namespace tlp;

bool MouseNodeBuilder::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    if (qMouseEv->button() == Qt::LeftButton) {
      GlGraphWidget *glw = (GlGraphWidget *) widget;

      Graph*_graph=glw->getGraph();
      LayoutProperty* mLayout=_graph->getProperty<LayoutProperty>("viewLayout");
      //  Colors* mColors=_graph->getProperty<ColorProperty>("viewColor");
      node newNode;
      float x1,y1,z1;
      newNode = _graph->addNode();
      //if (isViewStrahler()) orderedNode.push_front(newNode);
      x1 = (double) glw->width() - (double) qMouseEv->x() ;
      y1 = (double) qMouseEv->y();
      z1 = 0;
      glw->screenTo3DWorld(x1,y1,z1);
      mLayout->setNodeValue(newNode, Coord(x1,y1,z1));
      //      mColors->setNodeValue(newNode,((Application *)qApp)->nodeColor);
      glw->redraw();
      return true;
    }
  }
  return false;
}
