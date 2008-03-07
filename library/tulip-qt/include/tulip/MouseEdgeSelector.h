//-*-c++-*-
#ifndef MOUSEEDGESELECTION_H
#define MOUSEEDGESELECTION_H

#include <tulip/GWInteractor.h>

class QMouseEvent;
class QKeyEvent;

namespace tlp {

class Graph;
/** \addtogroup Mouse_interactor */ 
/*@{*/
class TLP_QT_SCOPE MouseEdgeSelector:public GWInteractor
{
private:
  unsigned int x,y;
  int w,h;
  bool started;
  Graph *graph;
public:
  MouseEdgeSelector();
  ~MouseEdgeSelector(){}
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MouseEdgeSelector(); }
};
/*@}*/

}
#endif
