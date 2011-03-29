#include "tulip/ToolbarItem.h"

#include "tulip/PushButtonItem.h"

#include <QtGui/QPainter>
#include <QtGui/QGraphicsEffect>

//FIXME: remove me
#include <iostream>
using namespace std;

class ToolbarButtonGraphicsEffect: public QGraphicsEffect {
public:
  ToolbarButtonGraphicsEffect(int yoffset=0, int height=0): _yoffset(yoffset), _height(height) {

  }

  virtual QRectF boundingRectFor(const QRectF &rect) const {
    return rect;
  }
protected:
  virtual void draw(QPainter *painter) {
    // Original pixmap
    painter->drawPixmap(0,0,sourcePixmap());
    QImage img = sourcePixmap().toImage();

    // Mirror effect
    QLinearGradient gradient(0,0,0,img.height());
    gradient.setColorAt(0.4,Qt::black);
    gradient.setColorAt(0,Qt::white);
    QImage reflexion = img.mirrored();
    QImage mask = img;
    QPainter p(&mask);
    p.fillRect(img.rect(), gradient);
    p.end();
    reflexion.setAlphaChannel(mask);

    // paint
    int h = (_height == 0 ? reflexion.height() : _height);
    painter->drawPixmap(0,sourcePixmap().height()+_yoffset,QPixmap::fromImage(reflexion.copy(0,0,reflexion.width(),h)));
  }

private:
  int _yoffset;
  int _height;
};

namespace tlp {
ToolbarItem::ToolbarItem(QGraphicsItem *parent,QGraphicsScene *scene)
  : QGraphicsItemGroup(parent,scene),
  _activeAction(0), _activeButton(0),
  _iconSize(32,32), _margin(6), _orientation(Qt::Horizontal), _animationMsec(100), _animationEasing(QEasingCurve::Linear),
  _outerOutline(0x9c,0x9a,0x94,0x00), _innerOutline(0xb3,0xaf,0xaf), _backgroundGradientStep1(0xdc,0xda,0xd5,0xd2), _backgroundGradientStep2(0xdc,0xda,0xd5,0x64) {
  setHandlesChildEvents(false);
}
//==========================
ToolbarItem::~ToolbarItem() {
}
//==========================
void ToolbarItem::addAction(QAction *action) {
  if (_actions.contains(action))
    return;
  _actions.push_back(action);
  _actionButton[action] = buildButton(action);

  if (!_activeAction)
    setActiveAction(action);
}
//==========================
void ToolbarItem::removeAction(QAction *action) {
  if (!_actions.contains(action))
    return;
  _actions.remove(_actions.indexOf(action));

  PushButtonItem *btn = _actionButton[action];
  delete btn;
  _actionButton.remove(action);

  if (action == _activeAction) {
    if (_actions.size() > 0)
      setActiveAction(_actions[0]);
    else
      setActiveAction(0);
  }
}
//==========================
void ToolbarItem::setActiveAction(QAction *action) {
  _activeAction = action;
  if (_activeAction) {
    if (_activeButton)
      _activeButton->setAction(action);
    else
      _activeButton = buildButton(_activeAction);
  }
  else {
    delete _activeButton;
    _activeButton=0;
  }
}
//==========================
void ToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);

  QRectF outerRect = computeBoundingRect();

  { // Enclosing rect
    QPen pen;
    pen.setColor(_outerOutline);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawRoundRect(outerRect,10,30);

    QRectF innerRect(1,1,outerRect.width()-2,outerRect.height()-2);
    pen.setColor(_innerOutline);
    pen.setWidth(1);
    painter->setPen(pen);
    QLinearGradient gradient(0,0,0,innerRect.height());
    gradient.setColorAt(0,_backgroundGradientStep1);
    gradient.setColorAt(1,_backgroundGradientStep2);
    painter->setBrush(QBrush(gradient));
    painter->drawRoundRect(outerRect,10,30);
  }
  QGraphicsItemGroup::paint(painter,option,widget);
}
//==========================
QRectF ToolbarItem::boundingRect() const {
  QRectF result = computeBoundingRect();

  // Position items
  QPointF pos(0,_margin);
  QPointF marginVector = _margin * translationVector();
  QPointF iconVector(_iconSize.width() * translationVector().x(), _iconSize.height() * translationVector().y());

  pos += marginVector;
  _activeButton->moveItem(pos, _animationMsec, _animationEasing);
  pos += iconVector + marginVector * 2;
  for (int i=0;i < _actions.size(); ++i) {
    _actionButton[_actions[i]]->moveItem(pos,_animationMsec, _animationEasing);
    pos += iconVector + marginVector;
  }

  return result;
}
//==========================
QPointF ToolbarItem::translationVector() const {
  switch (_orientation) {
  case Qt::Horizontal:
    return QPointF(1,0);
    break;
  case Qt::Vertical:
    return QPointF(0,1);
    break;
  }
}
//==========================
QSizeF ToolbarItem::hoveredIconSize() const {
  return QSizeF(_iconSize.width() + 2 * (_margin - 1), _iconSize.height() + 2 * (_margin - 1));
}
//==========================
QRectF ToolbarItem::computeBoundingRect() const {
  return QRectF(0,0,(_actions.size() + 1) * (_margin + _iconSize.width()) + _margin*2, hoveredIconSize().height());
}
//==========================
PushButtonItem *ToolbarItem::buildButton(QAction *action) {
  PushButtonItem *result = new PushButtonItem(action,_iconSize,this);
  result->setGraphicsEffect(new ToolbarButtonGraphicsEffect(-1 * _margin, _margin+3));
  return result;
}
}
