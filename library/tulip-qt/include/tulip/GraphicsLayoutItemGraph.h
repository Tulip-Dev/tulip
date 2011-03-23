#ifndef GRAPHICSLAYOUTITEMGRAPH_H
#define GRAPHICSLAYOUTITEMGRAPH_H

#include "tulip/GraphicsGraph.h"
#include <QtGui/QGraphicsLayoutItem>

namespace tlp {
class GraphicsLayoutItemGraph: public GraphicsGraph<QGraphicsLayoutItem> {
public:
  GraphicsLayoutItemGraph(const std::vector<QGraphicsLayoutItem *> &items);
  virtual ~GraphicsLayoutItemGraph();

protected:
  virtual void setItemPosition(QGraphicsLayoutItem *item, const QPointF &pos);
  virtual QRectF itemGeometry(QGraphicsLayoutItem *item);
};
}

#endif // GRAPHICSLAYOUTITEMGRAPH_H
