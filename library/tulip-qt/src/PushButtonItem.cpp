#include "tulip/PushButtonItem.h"

#include <QtGui/QPainter>
#include <QtGui/QGraphicsColorizeEffect>

PushButtonItem::PushButtonItem(const QString &text, const QIcon &icon, const QSize &iconSize, QGraphicsItem *parent):
  QGraphicsObject(parent),
  _text(text), _icon(icon), _iconSize(iconSize), _pressed(false), _hovered(false) {
  setAcceptHoverEvents(true);
}
//==========================
PushButtonItem::~PushButtonItem() {
}
//==========================
QString PushButtonItem::text() const {
  return _text;
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
  _text = text;
  prepareGeometryChange();
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
    setGraphicsEffect(0);
  }
}
//==========================
void PushButtonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
  if (!_hovered) {
    _hovered = true;
    update();
  }
}
//==========================
void PushButtonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  if (_hovered) {
    _hovered = false;
    update();
  }
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

  painter->drawPixmap(boundingRect().width()/2 - _iconSize.width()/2, 0, _iconSize.width(), _iconSize.height(), _icon.pixmap(_iconSize, mode));
}
//==========================
QRectF PushButtonItem::boundingRect() const {
  qreal width=0,height=0;
  if (!_icon.isNull()) {
    width += _iconSize.width();
    height += _iconSize.height();
  }
  if (_text != "") {
    QGraphicsTextItem textItem(_text);
    width = std::max<qreal>(width,textItem.boundingRect().width());
    height += textItem.boundingRect().height();
  }

  return QRectF(x(),y(),width, height);
}
