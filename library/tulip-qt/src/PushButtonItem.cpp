#include "tulip/PushButtonItem.h"

#include <QtGui/QPainter>
#include <QtGui/QGraphicsColorizeEffect>
#include <QtGui/QAction>
#include <QtGui/QGraphicsLayout>

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

  QPixmap pixmap = _icon.pixmap(_iconSize, mode);
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

  painter->drawPixmap(0, 0, _iconSize.width(), _iconSize.height(), pixmap);
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
  _action = action;
  setIcon(_action->icon());
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
