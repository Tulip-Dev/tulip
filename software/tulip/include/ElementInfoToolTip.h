//-*-c++-*-
#include <qtooltip.h>

class GlGraphWidget;

struct ElementInfoToolTip : public QToolTip {
  virtual ~ElementInfoToolTip(){}
  ElementInfoToolTip(QWidget* parent = NULL,const char* name = NULL,GlGraphWidget *glWidget = NULL);
  void maybeTip ( const QPoint & );

 private:
  GlGraphWidget *glWidget;
};
