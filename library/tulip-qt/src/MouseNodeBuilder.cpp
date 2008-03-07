#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>
#include "tulip/Qt3ForTulip.h"

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlMainWidget.h>


#include <tulip/MouseNodeBuilder.h>

using namespace tlp;

bool MouseNodeBuilder::eventFilter(QObject *widget, QEvent *e) {
  if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    if (qMouseEv->button() == Qt::LeftButton) {
      GlMainWidget *glw = (GlMainWidget *) widget;

      Graph*_graph=glw->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      LayoutProperty* mLayout=_graph->getProperty<LayoutProperty>("viewLayout");
      //  Colors* mColors=_graph->getProperty<ColorProperty>("viewColor");
      node newNode;
      newNode = _graph->addNode();
      //if (isViewStrahler()) orderedNode.push_front(newNode);
      Coord point((double) glw->width() - (double) qMouseEv->x(),
		  (double) qMouseEv->y(),
		  0);
      point = glw->getScene()->getCamera()->screenTo3DWorld(point);
      mLayout->setNodeValue(newNode, point);
      //      mColors->setNodeValue(newNode,((Application *)qApp)->nodeColor);
      glw->redraw();
      return true;
    }
  }
  return false;
}
