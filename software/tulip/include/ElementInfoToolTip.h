//-*-c++-*-
#include <qtooltip.h>

class GlMainWidget;
class viewGl;

struct ElementInfoToolTip : public QToolTip {
  virtual ~ElementInfoToolTip(){}
  ElementInfoToolTip(GlMainWidget *glWidget, viewGl *viewWidget);
  void maybeTip ( const QPoint & );

 private:
  GlMainWidget *glWidget;
  viewGl *viewWidget;
};
