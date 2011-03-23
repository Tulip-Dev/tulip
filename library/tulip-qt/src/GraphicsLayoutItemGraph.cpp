#include "tulip/GraphicsLayoutItemGraph.h"

namespace tlp {
GraphicsLayoutItemGraph::GraphicsLayoutItemGraph(const std::vector<QGraphicsLayoutItem *> &items): GraphicsGraph<QGraphicsLayoutItem>(items) {
}

GraphicsLayoutItemGraph::~GraphicsLayoutItemGraph() {
}

void GraphicsLayoutItemGraph::setItemPosition(QGraphicsLayoutItem *item, const QPointF &pos) {
  item->setGeometry(QRectF(pos, item->geometry().size()));
}

QRectF GraphicsLayoutItemGraph::itemGeometry(QGraphicsLayoutItem *item) {
  return item->geometry();
}
}
