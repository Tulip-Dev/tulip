//-*-c++-*-
#include <qtooltip.h>

class GlGraphWidget;
class viewGl;

struct ElementInfoToolTip : public QToolTip {
  virtual ~ElementInfoToolTip(){}
  ElementInfoToolTip(GlGraphWidget *glWidget, viewGl *viewWidget);
  void maybeTip ( const QPoint & );

 private:
  GlGraphWidget *glWidget;
  viewGl *viewWidget;
};
