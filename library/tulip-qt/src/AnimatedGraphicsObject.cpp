#include "tulip/AnimatedGraphicsObject.h"

namespace tlp {
AnimatedGraphicsObject::AnimatedGraphicsObject(QGraphicsItem *parent)
  : QGraphicsObject(parent),
  _animated(true) {
}

AnimatedGraphicsObject::~AnimatedGraphicsObject() {
}

QAbstractAnimation *AnimatedGraphicsObject::moveItem(const QPointF &from, const QPointF &to, int msec, const QEasingCurve &easing) {
//  if (from == to)
//    return 0;

  if (_animated) {
    QPropertyAnimation *result = new QPropertyAnimation(this, "pos");
    result->setStartValue(from);
    result->setEndValue(to);
    result->setDuration(msec);
    result->setEasingCurve(easing);
    return result;
  }

  else
    setPos(to);

  return 0;
}

QAbstractAnimation *AnimatedGraphicsObject::moveItem(const QPointF &to, int msec, const QEasingCurve &easing) {
  return moveItem(pos(), to, msec, easing);
}

QAbstractAnimation *AnimatedGraphicsObject::resizeItem(const QSizeF &from, const QSizeF &to, int msec, const QEasingCurve &easing) {
//  if (from == to)
//    return 0;

  if (_animated) {
    QPropertyAnimation *result = new QPropertyAnimation(this, "itemSize");
    result->setStartValue(from);
    result->setEndValue(to);
    result->setDuration(msec);
    result->setEasingCurve(easing);
    return result;
  }

  else
    setItemSize(to);

  return 0;
}

QAbstractAnimation *AnimatedGraphicsObject::resizeItem(const QSizeF &to, int msec, const QEasingCurve &easing) {
  return resizeItem(itemSize(),to,msec,easing);
}
}
