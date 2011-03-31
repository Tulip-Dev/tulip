#include "tulip/QtAnimationsManager.h"

#include <assert.h>

QtAnimationsManager *QtAnimationsManager::_instance = 0;

QtAnimationsManager::QtAnimationsManager(): QObject() {
}

QtAnimationsManager::~QtAnimationsManager() {
  for (QMap<QAbstractAnimation *,void *>::iterator it = _animationToObject.begin(); it != _animationToObject.end(); ++it)
    it.key()->stop();
}

void QtAnimationsManager::startAnimation(void *targetObject, QAbstractAnimation *animation, PreviousAnimationBehavior behavior) {
  // Check if the targeted object is already animated
  if (_objectToAnimation.contains(targetObject)) {
    if (behavior == ContinuePreviousAnimation) {
#ifndef NDEBUG
      qWarning("Prevented to animate an already animated object.");
#endif
      delete animation;
      return;
    }
    else if(behavior == StopPreviousAnimation) {
#ifndef NDEBUG
      qWarning("Request to animate an already animated object: deleting previous animation.");
#endif
      _objectToAnimation[targetObject]->stop();
    }
  }

  _objectToAnimation[targetObject] = animation;
  _animationToObject[animation] = targetObject;

  connect(animation,SIGNAL(finished()), this, SLOT(animationFinished()), Qt::DirectConnection);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtAnimationsManager::animationFinished() {
  QAbstractAnimation *animation = static_cast<QAbstractAnimation *>(sender());
  assert(_animationToObject.contains(animation));
  void *targetObject = _animationToObject[animation];
  _animationToObject.remove(animation);
  _objectToAnimation.remove(targetObject);
}
