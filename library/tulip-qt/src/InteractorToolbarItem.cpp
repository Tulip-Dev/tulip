#include "tulip/InteractorToolbarItem.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QApplication>

#include <tulip/Interactor.h>
#include "tulip/ToolbarItem.h"

using namespace std;

namespace tlp {
InteractorToolbarItem::InteractorToolbarItem(list<Interactor *> interactors, QGraphicsItem *parent,QGraphicsScene *scene):
    QGraphicsItemGroup(parent,scene), _interactors(interactors), _snapArea(Qt::TopToolBarArea), _round(6), _margin(3) {
  setFlag(QGraphicsItem::ItemIsMovable,true);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
  setHandlesChildEvents(false);

  _toolbar = new ToolbarItem(this,scene);

  for (list<Interactor *>::iterator it = _interactors.begin(); it != _interactors.end(); ++it)
    _toolbar->addAction((*it)->getAction());
}

InteractorToolbarItem::~InteractorToolbarItem() {
}

void InteractorToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  if (_snapArea == Qt::TopToolBarArea || _snapArea == Qt::LeftToolBarArea)
    _toolbar->setPos(_margin,_margin);
  else if (_snapArea == Qt::BottomToolBarArea)
    _toolbar->setPos(_margin,_margin+_round+20);
  else
    _toolbar->setPos(_margin+_round+20,_margin);

  painter->setBrush(QColor(0,255,0,50));
  painter->drawRect(boundingRect());

  QColor fgColor = QApplication::palette().color(QPalette::Shadow);
  QColor bgColor = QApplication::palette().color(QPalette::Window);
  fgColor.setAlpha(150);
  bgColor.setAlpha(150);
  painter->setPen(QPen(fgColor));
  painter->setBrush(bgColor);

  QRectF tbrect = toolbarBoundingRect();
  QRectF bgRect;
  if (_snapArea == Qt::TopToolBarArea) {
    bgRect = QRectF(0,-_round*1.5,tbrect.width(),tbrect.height()+_round);
    _configureRect = QRectF(tbrect.width()/2 - _toolbar->iconSize().width()/2, tbrect.height(), _toolbar->iconSize().width(),10);
  }
  else if (_snapArea == Qt::BottomToolBarArea) {
    bgRect = QRectF(0,_round/2,tbrect.width(),tbrect.height()+_round);
     _configureRect = QRectF(tbrect.width()/2 - _toolbar->iconSize().width()/2, 0, _toolbar->iconSize().width(),10);
  }
  else if (_snapArea == Qt::LeftToolBarArea) {
    bgRect = QRectF(-_round*1.5,0,tbrect.width()+_round,tbrect.height());
     _configureRect = QRectF(tbrect.width(), tbrect.height() / 2 - _toolbar->iconSize().height()/2, 10,_toolbar->iconSize().height());
  }
  else {
    bgRect = QRectF(_round/2,0,tbrect.width()+_round,tbrect.height());
    _configureRect = QRectF(0, tbrect.height() / 2 - _toolbar->iconSize().height()/2, 10,_toolbar->iconSize().height());
  }

  painter->drawRect(_configureRect);
  painter->drawRoundedRect(bgRect,_round,_round);
}

QRectF InteractorToolbarItem::boundingRect() const {
  QRectF result = QRectF(QPointF(0,0),toolbarBoundingRect().size());
  // Add space for configiuration button
  if (_snapArea == Qt::TopToolBarArea) {
    result.setHeight(result.height()+20);
  }
  else if (_snapArea == Qt::BottomToolBarArea) {
    result.setHeight(result.height()+20);
  }
  else if (_snapArea == Qt::LeftToolBarArea) {
    result.setWidth(result.width()+20);
  }
  else {
    result.setWidth(result.width()+20);
  }
  return result;
}

QRectF InteractorToolbarItem::toolbarBoundingRect() const {
  QRectF tbrect = _toolbar->boundingRect();
  QRectF result = tbrect;

  if (_snapArea == Qt::LeftToolBarArea || _snapArea == Qt::RightToolBarArea)
    result.setWidth(result.width()+_round);
  else
    result.setHeight(result.height()+_round);

  result.setWidth(result.width()+_margin);
  result.setHeight(result.height()+_margin);
  result.setX(_toolbar->pos().x());
  result.setY(_toolbar->pos().y());
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

void InteractorToolbarItem::displayConfigurationWidget(bool) {

}
}
