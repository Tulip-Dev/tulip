#include "tulip/QtAnimationsManager.h"

#include <assert.h>

QtAnimationsManager *QtAnimationsManager::_instance = 0;

QtAnimationsManager::QtAnimationsManager(): QObject() {
}

QtAnimationsManager::~QtAnimationsManager() {
  for (QMap<QAbstractAnimation *,QPair<int,void *> >::iterator it = _animationToObject.begin(); it != _animationToObject.end(); ++it)
    it.key()->stop();
}

void QtAnimationsManager::startAnimation(void *targetObject, QAbstractAnimation *animation, PreviousAnimationBehavior behavior, int slotId) {
  if (!animation)
    return;

  // Check if the targeted object is already animated
  if (_objectToAnimations.contains(targetObject)) {
    QMap<int,QAbstractAnimation *> animationSlots = _objectToAnimations[targetObject];
    if (animationSlots.contains(slotId)) {

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
        animationSlots[slotId]->stop();
      }
    }
  }

  _objectToAnimations[targetObject][slotId] = animation;
  _animationToObject[animation] = QPair<int,void *>(slotId,targetObject);
  connect(animation,SIGNAL(finished()), this, SLOT(animationFinished()), Qt::DirectConnection);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtAnimationsManager::animationFinished() {
  QAbstractAnimation *animation = static_cast<QAbstractAnimation *>(sender());
  assert(_animationToObject.contains(animation));

  // clean cache
  int slotId = _animationToObject[animation].first;
  void *targetObject = _animationToObject[animation].second;
  _animationToObject.remove(animation);
  _objectToAnimations[targetObject].remove(slotId);
  if (_objectToAnimations[targetObject].empty())
    _objectToAnimations.remove(targetObject);
}
