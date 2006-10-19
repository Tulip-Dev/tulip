//-*-c++-*-
#ifndef MOUSEZOOMBOX_H
#define MOUSEZOOMBOX_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <tulip/GWInteractor.h>

class QMouseEvent;
class QKeyEvent;

namespace tlp {

class Graph;
/** \addtogroup Mouse_interactor */ 
/*@{*/
class TLP_QT_SCOPE MouseBoxZoomer : public GWInteractor {

public:
  MouseBoxZoomer();
  ~MouseBoxZoomer();
  bool draw(GlGraphWidget *);
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MouseBoxZoomer(); }

private:
  unsigned int x, y;
  int w, h;
  bool started;
  Graph *graph;
};
/*@}*/

}
#endif //MOUSEZOOMBOX_H
