#ifndef GRAPHICSITEMSGRAPH_H
#define GRAPHICSITEMSGRAPH_H

#include "tulip/GraphicsGraphMapper.h"
#include <QtGui/QGraphicsItem>

namespace tlp {
class GraphicsItemsGraphMapper: public GraphicsGraphMapper<QGraphicsItem> {
public:
  GraphicsItemsGraphMapper(const std::vector<QGraphicsItem *> &items);
  virtual ~GraphicsItemsGraphMapper();

protected:
  virtual void setItemPosition(QGraphicsItem *item, const QPointF &pos);
  virtual QRectF itemGeometry(QGraphicsItem *item);
};
}

#endif // GRAPHICSITEMSGRAPH_H
