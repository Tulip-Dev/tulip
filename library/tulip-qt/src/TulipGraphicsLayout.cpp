#include "tulip/TulipGraphicsLayout.h"

#include "tulip/GraphicsLayoutItemGraphMapper.h"
#include <tulip/DrawingTools.h>
#include <tulip/DoubleProperty.h>
#include <QtCore/QVector>

using namespace std;

namespace tlp {
  TulipGraphicsLayout::TulipGraphicsLayout(const std::string &algName, DataSet *dataSet): _algName(algName), _dataSet(dataSet) {
}
// ===================================
TulipGraphicsLayout::~TulipGraphicsLayout() {
  delete _dataSet;
}
// ===================================
void TulipGraphicsLayout::addItem(QGraphicsLayoutItem *item) {
  addChildLayoutItem(item);
  _items.append(item);
}
// ===================================
QGraphicsLayoutItem *TulipGraphicsLayout::itemAt(int index) const {
  return _items.at(index);
}
// ===================================
void TulipGraphicsLayout::removeAt(int index) {
  if (index >= _items.size())
    return;
  _items.removeAt(index);
}
// ===================================
void TulipGraphicsLayout::setGeometry(const QRectF &rect) {
  GraphicsLayoutItemGraphMapper mapper(_items.toVector().toStdVector());
  mapper.applyValuesToGraph();
  Graph *g = mapper.getGraph();
  LayoutProperty *layout = g->getProperty<tlp::LayoutProperty>("viewLayout");
  string msg;
  g->computeProperty<LayoutProperty>(_algName,layout,msg,0,_dataSet);
  mapper.applyLayoutToItems(rect.center());
}
// ===================================
QSizeF TulipGraphicsLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
  GraphicsLayoutItemGraphMapper mapper(_items.toVector().toStdVector());
  mapper.applyValuesToGraph();
  Graph *g = mapper.getGraph();
  LayoutProperty *layout = g->getProperty<tlp::LayoutProperty>("viewLayout");
  SizeProperty *sizes = g->getProperty<tlp::SizeProperty>("viewSize");
  DoubleProperty *rotation = new DoubleProperty(g);

  string msg;
  bool result = g->computeProperty<LayoutProperty>(_algName,layout,msg,0,_dataSet);
  if (!result)
    qWarning("sizeHint() failed to compute the layout for the associated set of layout item.");

  BoundingBox bbox = computeBoundingBox(g,layout,sizes,rotation);
  delete rotation;
  Coord size(bbox[1] - bbox[0]);
  return QSizeF(size[0],size[1]);
}
// ===================================
int TulipGraphicsLayout::count() const {
  return _items.size();
}
// ===================================
void TulipGraphicsLayout::setData(DataSet *dataSet) {
  delete _dataSet;
  _dataSet = dataSet;
}
// ===================================
void TulipGraphicsLayout::setAlgorithm(const std::string &algName) {
  _algName = algName;
}
// ===================================
}
