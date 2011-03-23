#ifndef GRAPHICSITEMSGRAPH_H
#define GRAPHICSITEMSGRAPH_H

#include "tulip/GraphicsGraph.h"
#include <QtGui/QGraphicsItem>

namespace tlp {
class GraphicsItemsGraph: public GraphicsGraph<QGraphicsItem> {
public:
  GraphicsItemsGraph(const std::vector<QGraphicsItem *> &items);
  virtual ~GraphicsItemsGraph();

protected:
  virtual void setItemPosition(QGraphicsItem *item, const QPointF &pos);
  virtual QRectF itemGeometry(QGraphicsItem *item);
};
}

#endif // GRAPHICSITEMSGRAPH_H
