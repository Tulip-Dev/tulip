#ifndef ANIMATEDGRAPHICSOBJECT_H
#define ANIMATEDGRAPHICSOBJECT_H

#include <QtGui/QGraphicsObject>
#include <QtCore/QPropertyAnimation>

namespace tlp {
class AnimatedGraphicsObject: public QGraphicsObject {
  Q_OBJECT
  Q_PROPERTY(bool animated READ animated WRITE setAnimated);
  Q_PROPERTY(QSizeF itemSize READ itemSize WRITE setItemSize);
public:
  AnimatedGraphicsObject(QGraphicsItem *parent=0);
  ~AnimatedGraphicsObject();

  void moveItem(const QPointF &from, const QPointF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);
  void moveItem(const QPointF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);
  void resizeItem(const QSizeF &from, const QSizeF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);
  void resizeItem(const QSizeF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);

  void setAnimated(bool f) { _animated = f; }
  bool animated() const { return _animated; }

  virtual void setItemSize(const QSizeF &s) {}
  virtual QSizeF itemSize() const { return QSizeF(); }

protected slots:
  void animationFinished();

private:
  QPropertyAnimation *_currentPositionAnimation;
  QPropertyAnimation *_currentSizeAnimation;
  bool _animated;
  QSizeF _itemSize;
};
}
#endif // ANIMATEDGRAPHICSOBJECT_H
