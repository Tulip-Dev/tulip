#ifndef GRAPHICSGRAPH_CXX
#define GRAPHICSGRAPH_CXX

#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>

template<typename TYPE>
const std::string tlp::GraphicsGraphMapper<TYPE>::LAYOUT_PROPNAME = "viewLayout";

template<typename TYPE>
const std::string tlp::GraphicsGraphMapper<TYPE>::SIZE_PROPNAME = "viewSize";

template<typename TYPE>
tlp::GraphicsGraphMapper<TYPE>::GraphicsGraphMapper(const std::vector<TYPE *> &items): _items(items), _graph(0) {
  applyValuesToGraph();
}

template<typename TYPE>
tlp::GraphicsGraphMapper<TYPE>::~GraphicsGraphMapper() {
  delete _graph;
}

template<typename TYPE>
void tlp::GraphicsGraphMapper<TYPE>::applyValuesToGraph() {
  delete _graph;
  _graph = tlp::newGraph();

  tlp::LayoutProperty *layout = _graph->getProperty<LayoutProperty>(LAYOUT_PROPNAME);
  SizeProperty *sizes = _graph->getProperty<tlp::SizeProperty>(SIZE_PROPNAME);
  _nodeToItem.clear();
  _graph->clear();

  for(int i=0; i < _items.size(); ++i) {
    node n = _graph->addNode();
    _nodeToItem[n] = _items[i];
    QRectF rect = itemGeometry(_items[i]);
    layout->setNodeValue(n, Coord(rect.x(),rect.y(),0));
    sizes->setNodeValue(n, Size(rect.width(),rect.height(),1));
  }
}

template<typename TYPE>
void tlp::GraphicsGraphMapper<TYPE>::applyLayoutToItems(const QPointF &translateVector) {
  if (!_graph)
    return;

  tlp::LayoutProperty *layout = _graph->getProperty<tlp::LayoutProperty>(LAYOUT_PROPNAME);
  node n;
  forEach (n, _graph->getNodes()) {
    Coord coord3D = layout->getNodeValue(n);
    setItemPosition(_nodeToItem[n], QPointF(coord3D[0],coord3D[1]) + translateVector);
  }
}

#endif // GRAPHICSGRAPH_CXX
