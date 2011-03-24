#ifndef ANIMATEDGRAPHICSOBJECT_H
#define ANIMATEDGRAPHICSOBJECT_H

#include <QtGui/QGraphicsObject>
#include <QtCore/QPropertyAnimation>

namespace tlp {
class AnimatedGraphicsObject: public QGraphicsObject {
  Q_OBJECT
  Q_PROPERTY(bool animated READ animated WRITE setAnimated);
public:
  AnimatedGraphicsObject(QGraphicsItem *parent=0);
  void moveItem(const QPointF &from, const QPointF &to, int msec = 1000, const QEasingCurve &easing = QEasingCurve::OutElastic);

  void setAnimated(bool f) { _animated = f; }
  bool animated() const { return _animated; }

protected slots:
  void animationFinished();

private:
  QPropertyAnimation *_currentPositionAnimation;
  bool _animated;
};
}
#endif // ANIMATEDGRAPHICSOBJECT_H
