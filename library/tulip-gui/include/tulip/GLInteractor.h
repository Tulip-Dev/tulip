#ifndef GLINTERACTOR_H
#define GLINTERACTOR_H

#include <tulip/InteractorComposite.h>

namespace tlp {

class GlMainWidget;

class TLP_QT_SCOPE GLInteractorComponent: public tlp::InteractorComponent {
  Q_OBJECT
public slots:
  virtual bool draw(tlp::GlMainWidget*) {
    return false;
  }
  virtual bool compute(tlp::GlMainWidget*) {
    return false;
  }
};

class TLP_QT_SCOPE GLInteractorComposite: public tlp::InteractorComposite {
  Q_OBJECT

public:
  GLInteractorComposite(const QIcon& icon, const QString& text="");

public slots:
  virtual void compute(tlp::GlMainWidget*);
  virtual void draw(tlp::GlMainWidget*);
};

}

#endif // GLINTERACTOR_H
