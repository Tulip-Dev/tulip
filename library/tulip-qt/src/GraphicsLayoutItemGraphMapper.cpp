#include "tulip/GraphicsLayoutItemGraphMapper.h"

namespace tlp {
GraphicsLayoutItemGraphMapper::GraphicsLayoutItemGraphMapper(const std::vector<QGraphicsLayoutItem *> &items): GraphicsGraphMapper<QGraphicsLayoutItem>(items) {
}

GraphicsLayoutItemGraphMapper::~GraphicsLayoutItemGraphMapper() {
}

void GraphicsLayoutItemGraphMapper::setItemPosition(QGraphicsLayoutItem *item, const QPointF &pos) {
  item->setGeometry(QRectF(pos, item->geometry().size()));
}

QRectF GraphicsLayoutItemGraphMapper::itemGeometry(QGraphicsLayoutItem *item) {
  return QRectF(QPointF(0,0),item->preferredSize());
}
}
