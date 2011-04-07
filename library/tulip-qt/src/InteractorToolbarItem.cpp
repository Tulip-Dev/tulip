#include "tulip/InteractorToolbarItem.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QApplication>

#include <tulip/Interactor.h>
#include "tulip/ToolbarItem.h"

using namespace std;

//FIXME: remove me
#include <iostream>

namespace tlp {
InteractorToolbarItem::InteractorToolbarItem(list<Interactor *> interactors, QGraphicsItem *parent,QGraphicsScene *scene):
    QGraphicsItemGroup(parent,scene), _interactors(interactors), _snapArea(Qt::TopToolBarArea), _margin(5), _spacing(3) {
  setFlag(QGraphicsItem::ItemIsMovable,true);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
  setHandlesChildEvents(false);

  _toolbar = new ToolbarItem(this,scene);
  _toolbar->setPos(_margin,_margin);

  for (list<Interactor *>::iterator it = _interactors.begin(); it != _interactors.end(); ++it)
    _toolbar->addAction((*it)->getAction());
}

InteractorToolbarItem::~InteractorToolbarItem() {
}

void InteractorToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QRectF bRect(boundingRect());
  { // Enclosing rect
    QColor fgColor = QApplication::palette().color(QPalette::Shadow);
    QColor bgColor = QApplication::palette().color(QPalette::Window);
    fgColor.setAlpha(150);
    bgColor.setAlpha(150);
    painter->setPen(QPen(fgColor));
    painter->setBrush(bgColor);

    painter->drawRoundedRect(bRect,7,7);
  }
}

QRectF InteractorToolbarItem::boundingRect() const {
  QRectF tbRect(_toolbar->boundingRect());
  QRectF result(0,0,_margin*2 + tbRect.width(),_margin*2 + tbRect.height());
  return result;
}

QVariant InteractorToolbarItem::itemChange(GraphicsItemChange change, const QVariant &value) {
  if (change == ItemSceneChange) {
    disconnect(scene(),SIGNAL(sceneRectChanged(QRectF)),this,SLOT(sceneResized()));
    return value;
  }

  if (change == ItemSceneHasChanged) {
    connect(scene(),SIGNAL(sceneRectChanged(QRectF)),this,SLOT(sceneResized()));
    return value;
  }

  if (change != ItemPositionChange || !scene())
    return value;

  QRectF brect = boundingRect();
  qreal bMax = std::max<qreal>(brect.width(),brect.height());
  QSizeF bsize(bMax,bMax);

  // Update toolbar orientation and position
  QPointF result = value.toPointF();
  if (result.y() <= 0) { // top
    _snapArea = Qt::TopToolBarArea;
    _toolbar->setOrientation(Qt::Horizontal);
    result.setY(0);
  }
  else if (result.x() <= 0) { // left
    _snapArea = Qt::LeftToolBarArea;
    _toolbar->setOrientation(Qt::Vertical);
    result.setX(0);
  }
  else if (result.y()+bsize.height() >= scene()->height()) {// bottom
    _snapArea = Qt::BottomToolBarArea;
    _toolbar->setOrientation(Qt::Horizontal);
    result.setY(scene()->height()-boundingRect().height());
  }
  else if (result.x()+bsize.width() >= scene()->width()){ // right
    _snapArea = Qt::RightToolBarArea;
    _toolbar->setOrientation(Qt::Vertical);
    result.setX(scene()->width()-boundingRect().width());
  }
  else {
    if (_toolbar->orientation() == Qt::Horizontal)
      result.setY(pos().y());
    else
      result.setX(pos().x());
  }

  // Compute new bounding size
  bsize = boundingRect().size();

  // Check that we are in the scene's rect
  result.setX(std::max<qreal>(0,result.x()));
  result.setY(std::max<qreal>(0,result.y()));
  result.setX(std::min<qreal>(scene()->width()-bsize.width(),result.x()));
  result.setY(std::min<qreal>(scene()->height()-bsize.height(),result.y()));
  return result;
}

void InteractorToolbarItem::sceneResized() {
  QPointF newPos(pos());
  if (_snapArea == Qt::TopToolBarArea)
    newPos.setY(0);
  else if (_snapArea == Qt::LeftToolBarArea)
    newPos.setX(0);
  else if (_snapArea == Qt::RightToolBarArea)
    newPos.setX(scene()->width()-boundingRect().width());
  else
    newPos.setY(scene()->height()-boundingRect().height());

  setPos(newPos);
}
}
