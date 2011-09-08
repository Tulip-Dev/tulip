#ifndef TULIPPARALLELANIMATIONGROUP_H
#define TULIPPARALLELANIMATIONGROUP_H

#include <QtCore/QParallelAnimationGroup>
#include "tulip/tulipconf.h"

namespace tlp {

class TLP_QT_SCOPE TulipParallelAnimationGroup : public QParallelAnimationGroup {
public:
  explicit TulipParallelAnimationGroup(QObject *parent = 0);
protected:
  void updateCurrentTime(int currentTime);
};

}

#endif // TULIPPARALLELANIMATIONGROUP_H
