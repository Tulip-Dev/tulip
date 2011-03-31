#ifndef ANIMATEDGRAPHICSOBJECT_H
#define ANIMATEDGRAPHICSOBJECT_H

#include <QtGui/QGraphicsObject>
#include <QtCore/QPropertyAnimation>
#include "tulip/QtAnimationsManager.h"

namespace tlp {
class AnimatedGraphicsObject: public QGraphicsObject {
  Q_OBJECT
  Q_PROPERTY(QSizeF itemSize READ itemSize WRITE setItemSize);
public:
  AnimatedGraphicsObject(QGraphicsItem *parent=0);
  ~AnimatedGraphicsObject();

  QAbstractAnimation *moveItem(const QPointF &from, const QPointF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);
  QAbstractAnimation *moveItem(const QPointF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);
  QAbstractAnimation *resizeItem(const QSizeF &from, const QSizeF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);
  QAbstractAnimation *resizeItem(const QSizeF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);

  virtual void setItemSize(const QSizeF &s) {}
  virtual QSizeF itemSize() const { return QSizeF(); }

  void setAnimated(bool f) { _animated = f; }
  bool animated() const { return _animated; }

private:
  bool _animated;
  QSizeF _itemSize;
};
}
#endif // ANIMATEDGRAPHICSOBJECT_H
