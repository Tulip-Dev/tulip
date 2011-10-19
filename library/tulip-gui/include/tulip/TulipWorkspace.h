#ifndef TULIPWORKSPACE_H
#define TULIPWORKSPACE_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>

namespace tlp {

class TLP_QT_SCOPE TulipWorkspace: public QWidget {
  Q_OBJECT

public:
  explicit TulipWorkspace(QWidget* parent=0);
  virtual ~TulipWorkspace();
};

}

#endif // TULIPWORKSPACE_H
