//-*-c++-*-
#include <QtGui/qtooltip.h>

namespace tlp {
  class GlMainWidget;
}

class viewGl;

struct ElementInfoToolTip : public QToolTip {
  virtual ~ElementInfoToolTip(){}
  ElementInfoToolTip(tlp::GlMainWidget *glWidget, viewGl *viewWidget);
  void maybeTip ( const QPoint & );

 private:
  tlp:: GlMainWidget *glWidget;
  viewGl *viewWidget;
};
