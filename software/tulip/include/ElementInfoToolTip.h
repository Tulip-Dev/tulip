//-*-c++-*-
#include <QtGui/qtooltip.h>

namespace tlp {
  class GlMainWidget;
}

class TulipApp;

struct ElementInfoToolTip : public QToolTip {
  virtual ~ElementInfoToolTip(){}
  ElementInfoToolTip(tlp::GlMainWidget *glWidget, TulipApp *viewWidget);
  void maybeTip ( const QPoint & );

 private:
  tlp:: GlMainWidget *glWidget;
  TulipApp *viewWidget;
};
