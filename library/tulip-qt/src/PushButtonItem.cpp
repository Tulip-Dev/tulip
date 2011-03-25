#include "tulip/PushButtonItem.h"

#include <QtGui/QPainter>
#include <QtGui/QGraphicsColorizeEffect>
#include <QtGui/QAction>
#include <QtGui/QGraphicsLayout>

//FIXME: remove me
#include <iostream>

namespace tlp {
PushButtonItem::PushButtonItem(const QString &text, const QIcon &icon, const QSize &iconSize, QGraphicsItem *parent):
  AnimatedGraphicsObject(parent),
  _icon(icon), _iconSize(iconSize),
  _pressed(false), _hovered(false), _clicking(false), _action(0) {
  setAcceptHoverEvents(true);
  setGraphicsItem(this);
  setToolTip(text);
}
//==========================
PushButtonItem::PushButtonItem(QAction *action, const QSize &iconSize, QGraphicsItem *parent):
  AnimatedGraphicsObject(parent),
  _iconSize(iconSize),
  _pressed(false), _hovered(false), _clicking(false),
  _action(0) {

  setAction(action);
  setAcceptHoverEvents(true);
  setGraphicsItem(this);
}
//==========================
PushButtonItem::~PushButtonItem() {
  delete _action;
}
//==========================
QString PushButtonItem::text() const {
  return toolTip();
}
//==========================
QIcon PushButtonItem::icon() const {
  return _icon;
}
//==========================
QSize PushButtonItem::iconSize() const {
  return _iconSize;
}
//==========================
void PushButtonItem::setText(const QString &text) {
  setToolTip(text);
}
//==========================
void PushButtonItem::setIcon(const QIcon &icon) {
  _icon = icon;
  update();
}
//==========================
void PushButtonItem::setIconSize(const QSize &iconSize) {
  _iconSize = iconSize;
  prepareGeometryChange();
}
//==========================
void PushButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *) {
  if (!_pressed) {
    _pressed = true;
    _clicking = true;
    emit pressed();
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
    effect->setColor(QColor(255,255,255));
    effect->setStrength(0.3);
    setGraphicsEffect(effect);
  }
}
//==========================
void PushButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
  if (_pressed) {
    _pressed = false;
    emit released();
    if (_clicking) {
      if (_action)
        _action->trigger();
      emit clicked();
    }
    _clicking = false;
    setGraphicsEffect(0);
  }
}
//==========================
void PushButtonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
  if (!_hovered) {
    _hovered = true;
    emit hovered(true);
  }
  update();
}
//==========================
void PushButtonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  if (_hovered) {
    _hovered = false;
    _clicking = false;
    emit hovered(false);
  }
  update();
}
//==========================
void PushButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QIcon::Mode mode = QIcon::Normal;
  if (!isEnabled())
    mode = QIcon::Disabled;
  if (isActive())
    mode = QIcon::Active;
  if (isSelected())
    mode = QIcon::Selected;

  painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);
  if (!_hovered)
    painter->setOpacity(0.8);

  painter->drawPixmap(0, 0, _iconSize.width(), _iconSize.height(), _icon.pixmap(_iconSize, mode));
}
//==========================
QRectF PushButtonItem::boundingRect() const {
  qreal width=0,height=0;
  if (!_icon.isNull()) {
    width += _iconSize.width();
    height += _iconSize.height();
  }
  return QRectF(0,0,width, height);
}
//==========================
void PushButtonItem::setGeometry(const QRectF &rect) {
  QGraphicsLayoutItem::setGeometry(rect);
  moveItem(pos(), QPointF(rect.x(),rect.y()));
}
//==========================
QSizeF PushButtonItem::sizeHint(Qt::SizeHint, const QSizeF &) const {
  return boundingRect().size();
}
//==========================
void PushButtonItem::setAction(QAction *action) {
  delete _action;
  _action = action;
  setIcon(_action->icon());
  setText(action->text());
}
//==========================
void PushButtonItem::setItemSize(const QSizeF &s) {
  setIconSize(QSize(s.width(),s.height()));
}
//==========================
QSizeF PushButtonItem::itemSize() const {
  return _iconSize;
}
}
