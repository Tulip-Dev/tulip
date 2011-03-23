#include "tulip/TulipGraphicsLayout.h"

#include "tulip/GraphicsLayoutItemGraph.h"
#include <tulip/DrawingTools.h>

//FIXME: remove me
#include <iostream>
#include <QtGui/QGraphicsRectItem>
#include "tulip/PushButtonItem.h"
#include <tulip/DoubleProperty.h>

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
  invalidate();
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
  invalidate();
}
// ===================================
void TulipGraphicsLayout::setGeometry(const QRectF &rect) {
  cout << __PRETTY_FUNCTION__ << "===" << rect.x() << " " << rect.y() << " " << rect.width() << " " << rect.height() << endl;
}
// ===================================
QSizeF TulipGraphicsLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
  GraphicsLayoutItemGraph *g = new GraphicsLayoutItemGraph(_items.toVector().toStdVector());
  g->applyValuesToGraph();
  LayoutProperty *layout = g->getGraph()->getProperty<tlp::LayoutProperty>("viewLayout");
  SizeProperty *sizes = g->getGraph()->getProperty<tlp::SizeProperty>("viewSize");
  DoubleProperty *rotation = new DoubleProperty(g->getGraph());

  string msg;
  bool result = g->getGraph()->computeProperty<LayoutProperty>(_algName,layout,msg,0,_dataSet);
#ifndef NDEBUG
  assert(result);
#endif
  if (!result)
    qWarning("sizeHint() failed to compute the layout for the associated set of layout item.");
  BoundingBox bbox = computeBoundingBox(g->getGraph(),layout,sizes,rotation);
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
  invalidate();
}
// ===================================
void TulipGraphicsLayout::setAlgorithm(const std::string &algName) {
  _algName = algName;
  invalidate();
}
// ===================================
}
