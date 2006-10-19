//-*-c++-*-
#ifndef MOUSESELECTION_H
#define MOUSESELECTION_H

#include <tulip/GWInteractor.h>

class QMouseEvent;
class QKeyEvent;

namespace tlp {

class Graph;
/** \addtogroup Mouse_interactor */ 
/*@{*/
class TLP_QT_SCOPE MouseSelector:public GWInteractor
{
private:
  unsigned int x,y;
  int w,h;
  bool started;
  Graph *graph;
public:
  MouseSelector();
  ~MouseSelector(){}
  bool draw(GlGraphWidget *);
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MouseSelector(); }
};
/*@}*/

}
#endif
