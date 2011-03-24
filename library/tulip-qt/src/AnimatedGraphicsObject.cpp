#include "tulip/AnimatedGraphicsObject.h"

namespace tlp {
AnimatedGraphicsObject::AnimatedGraphicsObject(QGraphicsItem *parent): QGraphicsObject(parent), _currentPositionAnimation(0), _animated(true) {}

void AnimatedGraphicsObject::moveItem(const QPointF &from, const QPointF &to, int msec, const QEasingCurve &easing) {
  if (_currentPositionAnimation)
    _currentPositionAnimation->stop();

  if (_animated) {
    _currentPositionAnimation = new QPropertyAnimation(this, "pos");
    _currentPositionAnimation->setStartValue(from);
    _currentPositionAnimation->setEndValue(to);
    _currentPositionAnimation->setDuration(msec);
    _currentPositionAnimation->setEasingCurve(easing);
    _currentPositionAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(_currentPositionAnimation, SIGNAL(finished()), this, SLOT(animationFinished()));
  }

  else
    setPos(to);
}

void AnimatedGraphicsObject::animationFinished() {
  if (sender() == _currentPositionAnimation)
    _currentPositionAnimation = 0;
}
}
