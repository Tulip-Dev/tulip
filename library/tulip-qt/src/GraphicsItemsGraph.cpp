#include "tulip/GraphicsItemsGraph.h"

namespace tlp {
GraphicsItemsGraph::GraphicsItemsGraph(const std::vector<QGraphicsItem *> &items): GraphicsGraph<QGraphicsItem>(items) {
}

GraphicsItemsGraph::~GraphicsItemsGraph() {}

void GraphicsItemsGraph::setItemPosition(QGraphicsItem *item, const QPointF &pos) {
  item->setPos(pos);
}

QRectF GraphicsItemsGraph::itemGeometry(QGraphicsItem *item) {
  return QRectF(item->pos(), item->boundingRect().size());
}
}
