#include "tulip/PushButtonItem.h"

#include <math.h>

#include <QtGui/QPainter>
#include <QtGui/QGraphicsColorizeEffect>
#include <QtGui/QAction>
#include <QtGui/QGraphicsLayout>

namespace tlp {
PushButtonItem::PushButtonItem(QAction *action, const QSize &iconSize, QGraphicsItem *parent):
  AnimatedGraphicsObject(parent),
  _iconSize(iconSize),
  _pressed(false), _hovered(false), _clicking(false), _fadeout(true),
  _action(0),
  _borderWidth(7), _borderColor(QColor(200,200,200,150)), _backgroundColor(QColor(230,230,230,150)), _backgroundShape(NoShape) {

  setAction(action);
  setAcceptHoverEvents(true);
  setGraphicsItem(this);
}
//==========================
PushButtonItem::~PushButtonItem() {
}
//==========================
QString PushButtonItem::text() const {
  return toolTip();
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
void PushButtonItem::setIconSize(const QSize &iconSize) {
  _iconSize = iconSize;
  prepareGeometryChange();
}
//==========================
void PushButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *) {
  if (!_pressed) {
    _pressed = true;
    update();
    _clicking = true;
    emit pressed();
  }
}
//==========================
void PushButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
  if (_pressed) {
    _pressed = false;
    update();
    emit released();
    if (_clicking) {
      if (_action)
        _action->trigger();
      emit clicked();
    }
    _clicking = false;
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
  QRectF brect = boundingRect();
  QPen pen(_borderColor);
  pen.setWidth(_borderWidth);
  painter->setPen(pen);
  painter->setBrush(_backgroundColor);
  // Border
  if (_backgroundShape == CircleShape)
    painter->drawEllipse(brect);
  else if (_backgroundShape == SquareShape)
    painter->drawRoundedRect(brect,4,4);


  // Pixmap
  QIcon::Mode mode = QIcon::Normal;
  if (!isEnabled())
    mode = QIcon::Disabled;
  if (isActive())
    mode = QIcon::Active;
  if (isSelected())
    mode = QIcon::Selected;

  if (!_hovered && _fadeout)
    painter->setOpacity(0.8);
  else
    painter->setOpacity(1);

  QPixmap pixmap = _action->icon().pixmap(_iconSize, mode);
  if (_pressed) {
    QImage img = pixmap.toImage();
    QImage alpha = img.alphaChannel();
    for (int x = 0; x < img.width(); ++x) {
      for (int y = 0; y < img.height(); ++y) {
        QColor col = img.pixel(x, y);
        col.setRed(std::min<int>(255, col.red() + 20));
        col.setGreen(std::min<int>(255, col.green() + 20));
        col.setBlue(std::min<int>(255, col.blue() + 20));
        img.setPixel(x, y, qRgba(col.red(), col.green(), col.blue(), col.alpha()));
      }
    }
    img.setAlphaChannel(alpha);
    pixmap = QPixmap::fromImage(img);
  }

  painter->drawPixmap(brect.center().x() - _iconSize.width()/2,brect.center().y() - _iconSize.height()/2,_iconSize.width(),_iconSize.height(),pixmap);
}
//==========================
QRectF PushButtonItem::boundingRect() const {
  qreal width=0,height=0;
  if (!_action->icon().isNull()) {
    width += _iconSize.width();
    height += _iconSize.height();
  }
  if (_backgroundShape == SquareShape) {
    width += 8;
    height += 8;
  }
  if (_backgroundShape == CircleShape) {
    width = sqrt(pow(_iconSize.width(),2) + pow(_iconSize.height(),2)) + 3;
    height = width;
  }

  return QRectF(0,0,width,height);
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
  _action = action;
  setText(action->text());
  update();
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
