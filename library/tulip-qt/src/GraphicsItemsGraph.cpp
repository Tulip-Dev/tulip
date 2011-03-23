#include "tulip/GraphicsItemsGraphMapper.h"

namespace tlp {
GraphicsItemsGraphMapper::GraphicsItemsGraphMapper(const std::vector<QGraphicsItem *> &items): GraphicsGraphMapper<QGraphicsItem>(items) {
}

GraphicsItemsGraphMapper::~GraphicsItemsGraphMapper() {}

void GraphicsItemsGraphMapper::setItemPosition(QGraphicsItem *item, const QPointF &pos) {
  item->setPos(pos);
}

QRectF GraphicsItemsGraphMapper::itemGeometry(QGraphicsItem *item) {
  return QRectF(item->pos(), item->boundingRect().size());
}
}
