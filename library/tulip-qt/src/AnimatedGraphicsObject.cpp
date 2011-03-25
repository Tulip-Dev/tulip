#include "tulip/AnimatedGraphicsObject.h"

namespace tlp {
AnimatedGraphicsObject::AnimatedGraphicsObject(QGraphicsItem *parent): QGraphicsObject(parent), _currentPositionAnimation(0), _currentSizeAnimation(0), _animated(true) {}

AnimatedGraphicsObject::~AnimatedGraphicsObject() {
  if (_currentPositionAnimation)
    _currentPositionAnimation->stop();
  if (_currentSizeAnimation)
    _currentSizeAnimation->stop();
}

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

void AnimatedGraphicsObject::resizeItem(const QSizeF &from, const QSizeF &to, int msec, const QEasingCurve &easing) {
  if (_currentSizeAnimation)
    _currentSizeAnimation->stop();

  if (_animated) {
    _currentSizeAnimation = new QPropertyAnimation(this, "itemSize");
    _currentSizeAnimation->setStartValue(from);
    _currentSizeAnimation->setEndValue(to);
    _currentSizeAnimation->setDuration(msec);
    _currentSizeAnimation->setEasingCurve(easing);
    _currentSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(_currentSizeAnimation, SIGNAL(finished()), this, SLOT(animationFinished()));
  }

  else
    setItemSize(to);
}

void AnimatedGraphicsObject::animationFinished() {
  if (sender() == _currentPositionAnimation)
    _currentPositionAnimation = 0;
  else if (sender() == _currentSizeAnimation)
    _currentSizeAnimation = 0;
}
}
