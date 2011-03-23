#ifndef GRAPHICSGRAPH_H
#define GRAPHICSGRAPH_H

#include <tulip/GraphDecorator.h>
#include <QtCore/QRectF>
#include <vector>

namespace tlp {
template<typename TYPE>
  class GraphicsGraph {
public:
  GraphicsGraph(const std::vector<TYPE *> &items);
  virtual ~GraphicsGraph();

  void applyValuesToGraph();
  void applyLayoutToItems(const QPointF &center);

  Graph *getGraph() const { return _graph; }

protected:
  static const std::string LAYOUT_PROPNAME;
  static const std::string SIZE_PROPNAME;

  std::vector<TYPE *> _items;
  std::map<tlp::node,TYPE *> _nodeToItem;
  tlp::Graph *_graph;

  virtual void setItemPosition(TYPE *item, const QPointF &pos)=0;
  virtual QRectF itemGeometry(TYPE *item)=0;
};
}

#include "tulip/cxx/GraphicsGraph.cxx"

#endif // GRAPHICSGRAPH_H
