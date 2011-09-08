#include "tulip/TulipParallelAnimationGroup.h"

#include <tulip/Observable.h>

using namespace tlp;

TulipParallelAnimationGroup::TulipParallelAnimationGroup(QObject *parent): QParallelAnimationGroup(parent) {
}

void TulipParallelAnimationGroup::updateCurrentTime(int currentTime) {
  Observable::holdObservers();
  QParallelAnimationGroup::updateCurrentTime(currentTime);
  Observable::unholdObservers();
}
