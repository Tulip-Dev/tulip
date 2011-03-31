#ifndef QTANIMATIONSMANAGER_H
#define QTANIMATIONSMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QAbstractAnimation>
#include <QtCore/QMap>

class QtAnimationsManager: public QObject {
  Q_OBJECT
public:
  enum PreviousAnimationBehavior {
    ContinuePreviousAnimation,
    StopPreviousAnimation
  };

  static QtAnimationsManager &instance() {
    if (!_instance)
      _instance = new QtAnimationsManager();
    return *_instance;
  }
  virtual ~QtAnimationsManager();
  void startAnimation(void *targetObject, QAbstractAnimation *animation, PreviousAnimationBehavior behavior);

protected slots:
  void animationFinished();

private:
  static QtAnimationsManager *_instance;

  QMap<void *, QAbstractAnimation *> _objectToAnimation;
  QMap<QAbstractAnimation *, void *> _animationToObject;

  QtAnimationsManager();
};

#endif // QTANIMATIONSMANAGER_H
